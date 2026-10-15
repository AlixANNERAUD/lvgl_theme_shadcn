#![cfg(lvgl_theme_shadcn_linked)]

use core::ffi::c_void;

use core::ffi::CStr;

use lvgl_theme_shadcn::{
    Accent, Base, Color, Display, Flags, Font, Fonts, Object, Palette, Radius, Role, Size, Surface,
    TextRole, Theme, Variant, set_error, set_role, set_size, set_surface, set_text_role,
    set_variant, sys,
};

const LV_PART_MAIN: u32 = 0;
/// Probed from LVGL 9.6 with the `rust/ci` `lv_conf.h`; the value depends on the LVGL version.
const LV_STYLE_BG_COLOR: u8 = 73;
/// Probed from LVGL 9.6 like [`LV_STYLE_BG_COLOR`].
const LV_STYLE_RADIUS: u8 = 120;
/// Probed from LVGL 9.6 like [`LV_STYLE_BG_COLOR`].
const LV_STYLE_TEXT_COLOR: u8 = 92;
/// Probed from LVGL 9.6 like [`LV_STYLE_BG_COLOR`].
const LV_STYLE_TEXT_FONT: u8 = 77;

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
    fn lv_obj_create(parent: *mut sys::lv_obj_t) -> *mut sys::lv_obj_t;
    fn lv_obj_update_layout(obj: *const sys::lv_obj_t);
    fn lv_obj_get_width(obj: *const sys::lv_obj_t) -> i32;
    fn lv_obj_get_height(obj: *const sys::lv_obj_t) -> i32;
    fn lv_label_create(parent: *mut sys::lv_obj_t) -> *mut sys::lv_obj_t;
    fn lv_obj_delete(obj: *mut sys::lv_obj_t);
    fn lv_anim_count_running() -> u16;
    static lv_font_montserrat_14: u8;
}

fn radius(object: &Object) -> i32 {
    unsafe { lv_obj_get_style_prop(object.as_ptr(), LV_PART_MAIN, LV_STYLE_RADIUS).num }
}

fn size(object: &Object) -> (i32, i32) {
    unsafe {
        lv_obj_update_layout(object.as_ptr());
        (
            lv_obj_get_width(object.as_ptr()),
            lv_obj_get_height(object.as_ptr()),
        )
    }
}

fn bg_color(object: &Object) -> Color {
    let value = unsafe { lv_obj_get_style_prop(object.as_ptr(), LV_PART_MAIN, LV_STYLE_BG_COLOR) };
    Color::from(unsafe { value.color })
}

fn text_color(object: &Object) -> Color {
    let value =
        unsafe { lv_obj_get_style_prop(object.as_ptr(), LV_PART_MAIN, LV_STYLE_TEXT_COLOR) };
    Color::from(unsafe { value.color })
}

fn text_font(object: &Object) -> *const c_void {
    unsafe { lv_obj_get_style_prop(object.as_ptr(), LV_PART_MAIN, LV_STYLE_TEXT_FONT).ptr }
}

#[test]
fn role_enums_list_every_value() {
    assert_eq!(TextRole::ALL.len(), 11);
    assert_eq!(TextRole::default(), TextRole::Default);
    assert_eq!(Role::ALL.len(), 15);
    assert_eq!(Role::default(), Role::None);
    for (index, role) in TextRole::ALL.into_iter().enumerate() {
        assert_eq!(sys::lv_theme_shadcn_text_t::from(role).0 as usize, index);
    }
    for (index, role) in Role::ALL.into_iter().enumerate() {
        assert_eq!(sys::lv_theme_shadcn_role_t::from(role).0 as usize, index);
    }
    assert!(Flags::REDUCED_MOTION.contains(Flags::NONE));
    assert_eq!((Flags::COMPACT | Flags::REDUCED_MOTION).bits(), 3);
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
    assert_eq!(dark.background(), Color::hex(0x09090b));
    assert_eq!(dark.primary(), Color::hex(0xe4e4e7));
    assert_eq!(dark.destructive(), Color::hex(0xff6467));
    assert_eq!(dark.ring(), Color::hex(0x71717b));
    assert_eq!(dark.chart(0), Color::hex(0xd4d4d8));
    assert_eq!(dark.chart(Palette::CHART_COUNT - 1), Color::hex(0x27272a));
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
fn with_primary_changes_primary_only() {
    let base = Palette::zinc_dark();
    let primary = Color::hex(0x2563eb);
    let primary_fg = Color::hex(0xffffff);
    let derived = base.with_primary(primary, primary_fg);
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
fn every_base_and_accent_constructs() {
    for base in Base::ALL {
        for dark in [false, true] {
            let plain = Palette::new(base, Accent::None, dark);
            assert_eq!(plain.is_dark(), dark);
            assert_ne!(plain.background(), plain.foreground());
            for accent in Accent::ALL {
                let palette = Palette::new(base, accent, dark);
                assert_eq!(palette.is_dark(), dark);
                assert_eq!(palette.background(), plain.background());
                assert_eq!(palette.card(), plain.card());
                if accent == Accent::None {
                    assert_eq!(palette, plain);
                } else {
                    assert_ne!(palette.primary(), plain.primary());
                }
            }
        }
    }
    assert_eq!(
        Palette::new(Base::Zinc, Accent::None, true),
        Palette::zinc_dark()
    );
    assert_eq!(
        Palette::new(Base::Zinc, Accent::None, false),
        Palette::zinc_light()
    );
    assert_eq!(
        Palette::new(Base::Neutral, Accent::Blue, false).primary(),
        Color::hex(0x1447e6)
    );
}

#[test]
fn names_match_c() {
    assert_eq!(Base::default(), Base::Zinc);
    assert_eq!(Accent::default(), Accent::None);
    assert_eq!(Base::Zinc.name(), "zinc");
    assert_eq!(Accent::Blue.name(), "blue");
    assert_eq!(Accent::None.name(), "none");
    for base in Base::ALL {
        let c = unsafe { CStr::from_ptr(sys::lv_theme_shadcn_base_name(base.into())) };
        assert_eq!(c.to_str(), Ok(base.name()));
    }
    for accent in Accent::ALL {
        let c = unsafe { CStr::from_ptr(sys::lv_theme_shadcn_accent_name(accent.into())) };
        assert_eq!(c.to_str(), Ok(accent.name()));
    }
}

#[test]
fn radius_and_size_convert() {
    assert_eq!(Radius::default(), Radius::Default);
    assert_eq!(Size::default(), Size::Default);
    for (index, radius) in Radius::ALL.into_iter().enumerate() {
        let raw = sys::lv_theme_shadcn_radius_t::from(radius);
        assert_eq!(raw.0 as usize, index);
        assert_eq!(Radius::from_raw(raw), Some(radius));
    }
    assert_eq!(Radius::from_raw(sys::lv_theme_shadcn_radius_t(42)), None);
    for (index, size) in Size::ALL.into_iter().enumerate() {
        assert_eq!(sys::lv_theme_shadcn_size_t::from(size).0 as usize, index);
    }
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
    assert!(theme.palette().expect("palette").is_dark());
    assert_eq!(theme.palette(), Some(Palette::zinc_dark()));

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
    assert!(!theme.palette().expect("palette").is_dark());
    assert_eq!(theme.palette(), Some(Palette::zinc_light()));
    assert_eq!(bg_color(&button), Palette::zinc_light().primary());

    let card = unsafe { Object::from_ptr(lv_obj_create(screen)) }.expect("card");
    assert_eq!(theme.radius(), Radius::Default);
    assert_eq!((radius(&button), radius(&card)), (8, 14));
    theme.set_radius(Radius::None);
    assert_eq!(theme.radius(), Radius::None);
    assert_eq!((radius(&button), radius(&card)), (0, 0));
    theme.set_radius(Radius::Xl);
    assert_eq!(theme.radius(), Radius::Xl);
    assert_eq!((radius(&button), radius(&card)), (13, 22));
    theme.set_radius(Radius::Default);

    unsafe { set_size(&button, Size::Sm) };
    assert_eq!(size(&button).1, 32);
    unsafe { set_size(&button, Size::IconLg) };
    assert_eq!(size(&button), (48, 48));
    unsafe { set_size(&button, Size::Default) };
    assert_eq!(size(&button).1, 40);

    let screen_object = unsafe { Object::from_ptr(screen) }.expect("screen");
    for base in Base::ALL {
        for accent in Accent::ALL {
            for dark in [true, false] {
                let palette = Palette::new(base, accent, dark);
                theme.set_palette(&palette);
                assert_eq!(theme.palette(), Some(palette));
                assert_eq!(bg_color(&button), palette.primary());
                assert_eq!(bg_color(&screen_object), palette.background());
            }
        }
    }

    theme.set_palette(&Palette::zinc_dark());
    let label = unsafe { Object::from_ptr(lv_label_create(screen)) }.expect("label");
    unsafe { set_text_role(&label, TextRole::Muted) };
    assert_eq!(text_color(&label), Palette::zinc_dark().muted_fg());
    unsafe { set_text_role(&label, TextRole::H1) };
    assert_eq!(text_color(&label), Palette::zinc_dark().foreground());
    for role in TextRole::ALL {
        unsafe { set_text_role(&label, role) };
    }

    let badge = unsafe { Object::from_ptr(lv_obj_create(screen)) }.expect("badge");
    unsafe { set_role(&badge, Role::BadgeDestructive) };
    assert_eq!(bg_color(&badge), Palette::zinc_dark().destructive());
    assert_eq!(radius(&badge), 0x7fff);
    unsafe { set_role(&badge, Role::Separator) };
    assert_eq!(bg_color(&badge), Palette::zinc_dark().border());
    assert_eq!(size(&badge).1, 1);
    theme.set_palette(&Palette::zinc_light());
    assert_eq!(bg_color(&badge), Palette::zinc_light().border());
    unsafe { set_role(&badge, Role::None) };
    assert_eq!(bg_color(&badge), Palette::zinc_light().card());

    let running = unsafe { lv_anim_count_running() };
    let skeleton = unsafe { Object::from_ptr(lv_obj_create(screen)) }.expect("skeleton");
    unsafe { set_role(&skeleton, Role::Skeleton) };
    assert_eq!(unsafe { lv_anim_count_running() }, running + 1);
    unsafe { lv_obj_delete(skeleton.as_ptr()) };
    assert_eq!(unsafe { lv_anim_count_running() }, running);
    for role in Role::ALL {
        unsafe { set_role(&badge, role) };
    }
    unsafe { set_role(&badge, Role::None) };

    drop(theme);
    assert!(!Theme::is_active(&display));

    let normal = unsafe { Font::from_const_ptr(&raw const lv_font_montserrat_14) };
    let heading_fonts = Fonts {
        normal,
        h3: normal,
        ..Fonts::default()
    };
    let theme = unsafe { Theme::init(display, &Palette::zinc_dark(), &heading_fonts, Flags::NONE) }
        .expect("theme");
    unsafe { set_text_role(&label, TextRole::H1) };
    assert_eq!(text_font(&label), normal.expect("font").as_ptr().cast());
    drop(theme);

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
    assert!(!theme.palette().expect("palette").is_dark());
    drop(theme);
    assert!(!Theme::is_active(&display));
}
