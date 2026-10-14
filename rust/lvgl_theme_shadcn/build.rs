use std::env;

fn main() {
    println!("cargo::rerun-if-env-changed=LVGL_LIB_DIR");
    println!("cargo::rerun-if-env-changed=DOCS_RS");
    println!("cargo::rustc-check-cfg=cfg(lvgl_theme_shadcn_linked)");
    if env::var_os("LVGL_LIB_DIR").is_some() && env::var_os("DOCS_RS").is_none() {
        println!("cargo::rustc-cfg=lvgl_theme_shadcn_linked");
    }
}
