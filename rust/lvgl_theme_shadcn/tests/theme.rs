#![cfg(lvgl_theme_shadcn_linked)]

use core::ffi::c_void;

use lvgl_theme_shadcn::{
    Color, Display, Flags, Fonts, Object, Palette, Surface, Theme, Variant, set_error, set_surface,
    set_variant, sys,
};

const LV_PART_MAIN: u32 = 0;
const LV_STYLE_BG_COLOR: u8 = 73;

#[repr(C)]
#[derive(Clone, Copy)]
union StyleValue {
    num: i32,
    ptr: *const c_void,
    color: sys::lv_color_t,
}

unsafe extern "C" {
    fn lv_init();
    fn lv_display_create(hor_res: i32, ver_res: i32) -> *mut sys::lv_display_t;
    fn lv_display_get_screen_active(display: *mut sys::lv_display_t) -> *mut sys::lv_obj_t;
    fn lv_button_create(parent: *mut sys::lv_obj_t) -> *mut sys::lv_obj_t;
    fn lv_obj_get_style_prop(obj: *const sys::lv_obj_t, part: u32, prop: u8) -> StyleValue;
}

fn bg_color(object: &Object) -> Color {
    let value = unsafe { lv_obj_get_style_prop(object.as_ptr(), LV_PART_MAIN, LV_STYLE_BG_COLOR) };
    Color::from(unsafe { value.color })
}

#[test]
fn color_hex_splits_channels() {
    assert_eq!(Color::hex(0x18181b), Color::rgb(0x18, 0x18, 0x1b));
    assert_eq!(Color::hex(0xff123456), Color::rgb(0x12, 0x34, 0x56));
    assert_eq!(Color::rgb(0x12, 0x34, 0x56).to_hex(), 0x123456);
    let raw = sys::lv_color_t::from(Color::hex(0xabcdef));
    assert_eq!((raw.red, raw.green, raw.blue), (0xab, 0xcd, 0xef));
    assert_eq!(Color::from(raw), Color::hex(0xabcdef));
}

#[test]
fn zinc_palettes_match_c() {
    let dark = Palette::zinc_dark();
    let light = Palette::zinc_light();
    assert!(dark.is_dark());
    assert!(!light.is_dark());
    assert_eq!(dark.as_raw(), unsafe {
        &*sys::lv_theme_shadcn_palette_zinc_dark()
    });
    assert_eq!(light.as_raw(), unsafe {
        &*sys::lv_theme_shadcn_palette_zinc_light()
    });
    assert_eq!(dark.background(), Color::hex(0x09090b));
    assert_eq!(dark.primary(), Color::hex(0xe4e4e7));
    assert_eq!(dark.destructive(), Color::hex(0xff6467));
    assert_eq!(dark.ring(), Color::hex(0x71717b));
    assert_eq!(dark.chart(0), Color::hex(0x8ec5ff));
    assert_eq!(dark.chart(Palette::CHART_COUNT - 1), Color::hex(0x193cb8));
    assert_eq!(light.primary(), Color::hex(0x18181b));
    assert_eq!(light.background(), Color::hex(0xffffff));
}

#[test]
fn palette_setters_roundtrip() {
    let mut palette = Palette::zinc_dark();
    palette.set_card(Color::hex(0x123456));
    palette.set_chart(2, Color::hex(0x654321));
    palette.set_dark(false);
    assert_eq!(palette.card(), Color::hex(0x123456));
    assert_eq!(palette.chart(2), Color::hex(0x654321));
    assert!(!palette.is_dark());
    let raw: sys::lv_theme_shadcn_palette_t = palette.into();
    assert_eq!(Palette::from(raw), palette);
}

#[test]
fn from_primary_changes_primary_only() {
    let base = Palette::zinc_dark();
    let primary = Color::hex(0x2563eb);
    let primary_fg = Color::hex(0xffffff);
    let derived = base.from_primary(primary, primary_fg);
    assert_eq!(derived.primary(), primary);
    assert_eq!(derived.primary_fg(), primary_fg);
    assert_ne!(derived.ring(), base.ring());
    let mut expected = base;
    expected.set_primary(primary);
    expected.set_primary_fg(primary_fg);
    expected.set_ring(derived.ring());
    assert_eq!(derived, expected);
}

#[test]
fn flags_combine() {
    assert_eq!(Flags::NONE | Flags::COMPACT, Flags::COMPACT);
    assert!(Flags::COMPACT.contains(Flags::NONE));
    assert!(!Flags::NONE.contains(Flags::COMPACT));
    let mut flags = Flags::NONE;
    flags |= Flags::COMPACT;
    assert_eq!(flags.bits(), 1);
}

#[test]
fn theme_lifecycle_and_helpers() {
    unsafe { lv_init() };
    let display = unsafe { Display::from_ptr(lv_display_create(480, 320)) }.expect("display");
    assert!(unsafe { Display::from_ptr(core::ptr::null_mut::<c_void>()) }.is_none());
    assert!(!Theme::is_active(&display));

    let theme = unsafe {
        Theme::init(
            display,
            &Palette::zinc_dark(),
            &Fonts::default(),
            Flags::NONE,
        )
    }
    .expect("theme");
    assert!(Theme::is_active(&display));
    assert_eq!(theme.display(), &display);
    assert!(theme.palette().is_dark());
    assert_eq!(theme.palette(), Palette::zinc_dark());

    let screen = unsafe { lv_display_get_screen_active(display.as_ptr()) };
    let button = unsafe { Object::from_ptr(lv_button_create(screen)) }.expect("button");
    assert_eq!(bg_color(&button), Palette::zinc_dark().primary());

    unsafe { set_variant(&button, Variant::Destructive) };
    assert_eq!(bg_color(&button), Palette::zinc_dark().destructive());
    unsafe { set_variant(&button, Variant::Secondary) };
    assert_eq!(bg_color(&button), Palette::zinc_dark().secondary());
    unsafe {
        set_error(&button, true);
        set_error(&button, false);
        set_surface(&button, Surface::Card);
        set_variant(&button, Variant::Default);
    }

    theme.set_palette(&Palette::zinc_light());
    assert!(!theme.palette().is_dark());
    assert_eq!(theme.palette(), Palette::zinc_light());
    assert_eq!(bg_color(&button), Palette::zinc_light().primary());

    drop(theme);
    assert!(!Theme::is_active(&display));

    let theme = unsafe {
        Theme::init(
            display,
            &Palette::zinc_light(),
            &Fonts::default(),
            Flags::NONE | Flags::COMPACT,
        )
    }
    .expect("theme");
    assert!(Theme::is_active(&display));
    assert!(!theme.palette().is_dark());
    drop(theme);
    assert!(!Theme::is_active(&display));
}
