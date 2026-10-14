use std::env;
use std::fs;
use std::path::{Path, PathBuf};

const LVGL_ENV_VARS: [&str; 6] = [
    "DEP_LVGL_INCLUDE",
    "DEP_LVGL_CONF_PATH",
    "LVGL_INCLUDE_DIR",
    "LV_CONF_PATH",
    "LVGL_LIB_DIR",
    "DOCS_RS",
];

struct Lvgl {
    include: PathBuf,
    conf: PathBuf,
}

fn main() {
    for var in LVGL_ENV_VARS {
        println!("cargo::rerun-if-env-changed={var}");
    }
    println!("cargo::rustc-check-cfg=cfg(lvgl_theme_shadcn_linked)");

    let manifest_dir =
        PathBuf::from(env::var_os("CARGO_MANIFEST_DIR").expect("CARGO_MANIFEST_DIR"));
    let c_dir = manifest_dir.join("c");
    for path in ["src", "include", "sources.cmake"] {
        println!("cargo::rerun-if-changed={}", c_dir.join(path).display());
    }
    println!("cargo::rerun-if-changed=src/bindings.rs");
    println!(
        "cargo::metadata=include={}",
        c_dir.join("include").display()
    );

    if env::var_os("DOCS_RS").is_some() {
        #[cfg(feature = "bindgen")]
        copy_committed_bindings(&manifest_dir);
        return;
    }

    let lvgl = locate_lvgl();
    let include_dirs = lvgl_include_dirs(&lvgl.include);

    #[cfg(feature = "bindgen")]
    generate_bindings(&c_dir, &include_dirs, &lvgl.conf);

    let mut build = cc::Build::new();
    build
        .files(theme_sources(&c_dir))
        .include(c_dir.join("include"))
        .include(c_dir.join("src"))
        .include(&lvgl.conf)
        .includes(&include_dirs)
        .define("LV_CONF_INCLUDE_SIMPLE", None)
        .flag_if_supported("-std=c99")
        .compile("lvgl_theme_shadcn");

    if let Some(lib_dir) = env::var_os("LVGL_LIB_DIR") {
        println!(
            "cargo::rustc-link-search=native={}",
            Path::new(&lib_dir).display()
        );
        println!("cargo::rustc-link-lib=static=lvgl");
        println!("cargo::rustc-cfg=lvgl_theme_shadcn_linked");
    }
}

fn locate_lvgl() -> Lvgl {
    let pair = |include: &str, conf: &str| match (env::var_os(include), env::var_os(conf)) {
        (Some(include), Some(conf)) => Some(Lvgl {
            include: PathBuf::from(include),
            conf: PathBuf::from(conf),
        }),
        _ => None,
    };
    pair("DEP_LVGL_INCLUDE", "DEP_LVGL_CONF_PATH")
        .or_else(|| pair("LVGL_INCLUDE_DIR", "LV_CONF_PATH"))
        .unwrap_or_else(|| {
            panic!(
                "LVGL not found: depend on a crate declaring `links = \"lvgl\"` that exports \
                 DEP_LVGL_INCLUDE and DEP_LVGL_CONF_PATH, or set LVGL_INCLUDE_DIR (LVGL root \
                 directory containing lvgl.h) and LV_CONF_PATH (directory containing lv_conf.h)"
            )
        })
}

fn lvgl_include_dirs(include: &Path) -> Vec<PathBuf> {
    let mut dirs = vec![include.to_path_buf()];
    if include.join("lvgl.h").is_file() {
        let nested = include.join("include");
        if nested.is_dir() {
            dirs.push(nested);
        }
    } else if let Some(parent) = include.parent() {
        dirs.push(parent.to_path_buf());
    }
    dirs
}

fn theme_sources(c_dir: &Path) -> Vec<PathBuf> {
    let list = c_dir.join("sources.cmake");
    let text = fs::read_to_string(&list)
        .unwrap_or_else(|error| panic!("cannot read {}: {error}", list.display()));
    let sources: Vec<PathBuf> = text
        .split_whitespace()
        .filter_map(|token| token.strip_prefix("${CMAKE_CURRENT_LIST_DIR}/"))
        .map(|path| c_dir.join(path.trim_end_matches(')')))
        .collect();
    assert!(
        !sources.is_empty(),
        "no sources listed in {}",
        list.display()
    );
    sources
}

#[cfg(feature = "bindgen")]
fn out_bindings() -> PathBuf {
    PathBuf::from(env::var_os("OUT_DIR").expect("OUT_DIR")).join("bindings.rs")
}

#[cfg(feature = "bindgen")]
fn copy_committed_bindings(manifest_dir: &Path) {
    fs::copy(manifest_dir.join("src").join("bindings.rs"), out_bindings())
        .expect("copy committed bindings");
}

#[cfg(feature = "bindgen")]
#[derive(Debug)]
struct PaletteDerives;

#[cfg(feature = "bindgen")]
impl bindgen::callbacks::ParseCallbacks for PaletteDerives {
    fn add_derives(&self, info: &bindgen::callbacks::DeriveInfo<'_>) -> Vec<String> {
        if info.name == "lv_theme_shadcn_palette_t" {
            ["Clone", "Copy", "Debug", "Default", "PartialEq", "Eq"]
                .map(String::from)
                .to_vec()
        } else {
            Vec::new()
        }
    }
}

#[cfg(feature = "bindgen")]
fn generate_bindings(c_dir: &Path, include_dirs: &[PathBuf], conf: &Path) {
    let opaque = ["lv_obj_t", "lv_display_t", "lv_font_t", "lv_theme_t"]
        .map(|name| format!("#[repr(C)]\npub struct {name} {{\n    _unused: [u8; 0],\n}}"))
        .join("\n");
    let builder = bindgen::Builder::default()
        .header(
            c_dir
                .join("include")
                .join("lv_theme_shadcn.h")
                .to_string_lossy(),
        )
        .use_core()
        .disable_header_comment()
        .generate_comments(false)
        .layout_tests(false)
        .allowlist_function("lv_theme_shadcn_.*")
        .allowlist_type("lv_theme_shadcn_.*")
        .allowlist_var("LV_THEME_SHADCN_.*")
        .newtype_enum("lv_theme_shadcn_.*_t")
        .allowlist_recursively(false)
        .parse_callbacks(Box::new(PaletteDerives))
        .raw_line(
            "#[repr(C)]\n#[derive(Clone, Copy, Debug, Default, PartialEq, Eq)]\n\
             pub struct lv_color_t {\n    pub blue: u8,\n    pub green: u8,\n    pub red: u8,\n}",
        )
        .raw_line(opaque)
        .clang_args(
            include_dirs
                .iter()
                .map(|dir| format!("-I{}", dir.display())),
        )
        .clang_arg(format!("-I{}", conf.display()))
        .clang_arg("-DLV_CONF_INCLUDE_SIMPLE");
    builder
        .generate()
        .expect("generate bindings")
        .write_to_file(out_bindings())
        .expect("write bindings");
}
