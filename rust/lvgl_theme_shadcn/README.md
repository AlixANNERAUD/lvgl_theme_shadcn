# lvgl_theme_shadcn

Safe `no_std` Rust bindings to [`lvgl_theme_shadcn`](https://github.com/AlixANNERAUD/lvgl_theme_shadcn), a
shadcn/ui-inspired theme for LVGL 9.6+. No allocation, no dependency on any LVGL Rust crate.

```rust,ignore
use lvgl_theme_shadcn::{Color, Display, Flags, Fonts, Object, Palette, Theme, Variant};

let palette = Palette::zinc_dark().from_primary(Color::hex(0x2563eb), Color::hex(0xffffff));
let theme = unsafe {
    let display = Display::from_ptr(lv_display_create(480, 320)).unwrap();
    Theme::init(display, &palette, &Fonts::default(), Flags::NONE).unwrap()
};
let button = unsafe { Object::from_ptr(lv_button_create(lv_screen_active())).unwrap() };
unsafe { lvgl_theme_shadcn::set_variant(&button, Variant::Destructive) };
theme.set_palette(&Palette::zinc_light());
```

## Interoperability

Handles are built from raw pointers of any type, so they work with any LVGL binding (for example `lvgl_rust_sys`):
`Display::from_ptr`, `Object::from_ptr`, `Font::from_ptr` and `Font::from_const_ptr` (for
`&raw const lv_font_montserrat_14`). Use `as_ptr().cast()` to go back to your binding's types.

## Locating LVGL

The C theme is compiled by [`lvgl_theme_shadcn_sys`](https://crates.io/crates/lvgl_theme_shadcn_sys), which needs
the LVGL headers and `lv_conf.h`:

- `LVGL_INCLUDE_DIR`: the LVGL root directory (the one containing `lvgl.h`);
- `LV_CONF_PATH`: the directory containing `lv_conf.h`, the same one used by the LVGL you link against.

Set them for example in `.cargo/config.toml`:

```toml
[env]
LVGL_INCLUDE_DIR = { value = "third_party/lvgl", relative = true }
LV_CONF_PATH = { value = "config", relative = true }
```

`DEP_LVGL_INCLUDE` and `DEP_LVGL_CONF_PATH` take precedence, but Cargo only sets them when the sys crate is built as
a direct dependent of a crate declaring `links = "lvgl"`, which is rare.

Linking LVGL itself is the application's job (through its LVGL binding or build system). For tests and CI,
`LVGL_LIB_DIR` links a prebuilt `liblvgl.a`; see the sys crate README.

## Safety

LVGL is not thread-safe: use every handle on the thread running LVGL, after `lv_init`. `Theme` is `!Send` and
`!Sync`; dropping it restores the display's previous theme. The display and fonts must outlive the `Theme`.

## License

MIT
