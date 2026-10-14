//! Raw FFI bindings to `lv_theme_shadcn`, a shadcn/ui-inspired theme for LVGL 9.
//!
//! Every LVGL type except [`lv_color_t`] is opaque, so these bindings do not depend on `lv_conf.h`.
#![no_std]
#![allow(non_camel_case_types, non_upper_case_globals, non_snake_case)]

#[cfg(not(feature = "bindgen"))]
include!("bindings.rs");

#[cfg(feature = "bindgen")]
include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

const _: () = {
    assert!(core::mem::size_of::<lv_color_t>() == 3);
    assert!(core::mem::size_of::<lv_theme_shadcn_palette_t>() == 73);
    assert!(core::mem::align_of::<lv_theme_shadcn_palette_t>() == 1);
    assert!(core::mem::offset_of!(lv_theme_shadcn_palette_t, chart) == 57);
    assert!(core::mem::offset_of!(lv_theme_shadcn_palette_t, dark) == 72);
};
