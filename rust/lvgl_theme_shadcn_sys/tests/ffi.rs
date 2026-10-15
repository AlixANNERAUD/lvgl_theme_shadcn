#![cfg(lvgl_theme_shadcn_linked)]

use core::mem::{offset_of, size_of};
use core::ptr;

use lvgl_theme_shadcn_sys::*;

#[link(name = "lvgl_theme_shadcn_layout_probe", kind = "static")]
unsafe extern "C" {
    fn lv_theme_shadcn_probe_palette_size() -> usize;
    fn lv_theme_shadcn_probe_palette_primary_offset() -> usize;
    fn lv_theme_shadcn_probe_palette_chart_offset() -> usize;
    fn lv_theme_shadcn_probe_palette_dark_offset() -> usize;
    fn lv_theme_shadcn_probe_fonts_size() -> usize;
    fn lv_theme_shadcn_probe_fonts_h1_offset() -> usize;
}

unsafe extern "C" {
    fn lv_init();
    fn lv_display_create(hor_res: i32, ver_res: i32) -> *mut lv_display_t;
}

#[test]
fn zinc_dark_palette_is_dark() {
    let palette = unsafe { lv_theme_shadcn_palette_zinc_dark() };
    assert!(!palette.is_null());
    assert!(unsafe { (*palette).dark });
}

#[test]
fn zinc_light_palette_is_light() {
    let palette = unsafe { lv_theme_shadcn_palette_zinc_light() };
    assert!(!palette.is_null());
    assert!(!unsafe { (*palette).dark });
}

#[test]
fn palette_layout_matches_c() {
    unsafe {
        assert_eq!(
            size_of::<lv_theme_shadcn_palette_t>(),
            lv_theme_shadcn_probe_palette_size()
        );
        assert_eq!(
            offset_of!(lv_theme_shadcn_palette_t, primary),
            lv_theme_shadcn_probe_palette_primary_offset()
        );
        assert_eq!(
            offset_of!(lv_theme_shadcn_palette_t, chart),
            lv_theme_shadcn_probe_palette_chart_offset()
        );
        assert_eq!(
            offset_of!(lv_theme_shadcn_palette_t, dark),
            lv_theme_shadcn_probe_palette_dark_offset()
        );
    }
}

#[test]
fn fonts_layout_matches_c() {
    unsafe {
        assert_eq!(
            size_of::<lv_theme_shadcn_fonts_t>(),
            lv_theme_shadcn_probe_fonts_size()
        );
        assert_eq!(
            offset_of!(lv_theme_shadcn_fonts_t, h1),
            lv_theme_shadcn_probe_fonts_h1_offset()
        );
    }
}

#[test]
fn init_activates_and_deinit_deactivates_theme() {
    unsafe {
        lv_init();
        let display = lv_display_create(480, 320);
        assert!(!display.is_null());
        assert!(!lv_theme_shadcn_is_active(display));
        let theme = lv_theme_shadcn_init(
            display,
            lv_theme_shadcn_palette_zinc_dark(),
            ptr::null(),
            lv_theme_shadcn_flags_t::LV_THEME_SHADCN_FLAG_NONE,
        );
        assert!(!theme.is_null());
        assert!(lv_theme_shadcn_is_active(display));
        lv_theme_shadcn_deinit(display);
        assert!(!lv_theme_shadcn_is_active(display));
    }
}
