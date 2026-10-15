#[repr(C)]
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq)]
pub struct lv_color_t {
    pub blue: u8,
    pub green: u8,
    pub red: u8,
}
#[repr(C)]
pub struct lv_obj_t {
    _unused: [u8; 0],
}
#[repr(C)]
pub struct lv_display_t {
    _unused: [u8; 0],
}
#[repr(C)]
pub struct lv_font_t {
    _unused: [u8; 0],
}
#[repr(C)]
pub struct lv_theme_t {
    _unused: [u8; 0],
}

#[repr(C)]
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq)]
pub struct lv_theme_shadcn_palette_t {
    pub background: lv_color_t,
    pub foreground: lv_color_t,
    pub card: lv_color_t,
    pub card_fg: lv_color_t,
    pub popover: lv_color_t,
    pub popover_fg: lv_color_t,
    pub primary: lv_color_t,
    pub primary_fg: lv_color_t,
    pub secondary: lv_color_t,
    pub secondary_fg: lv_color_t,
    pub muted: lv_color_t,
    pub muted_fg: lv_color_t,
    pub accent: lv_color_t,
    pub accent_fg: lv_color_t,
    pub destructive: lv_color_t,
    pub destructive_fg: lv_color_t,
    pub border: lv_color_t,
    pub input: lv_color_t,
    pub ring: lv_color_t,
    pub chart: [lv_color_t; 5usize],
    pub dark: bool,
}
impl lv_theme_shadcn_base_t {
    pub const LV_THEME_SHADCN_BASE_NEUTRAL: lv_theme_shadcn_base_t = lv_theme_shadcn_base_t(0);
    pub const LV_THEME_SHADCN_BASE_STONE: lv_theme_shadcn_base_t = lv_theme_shadcn_base_t(1);
    pub const LV_THEME_SHADCN_BASE_ZINC: lv_theme_shadcn_base_t = lv_theme_shadcn_base_t(2);
    pub const LV_THEME_SHADCN_BASE_GRAY: lv_theme_shadcn_base_t = lv_theme_shadcn_base_t(3);
    pub const LV_THEME_SHADCN_BASE_SLATE: lv_theme_shadcn_base_t = lv_theme_shadcn_base_t(4);
    pub const LV_THEME_SHADCN_BASE_MAUVE: lv_theme_shadcn_base_t = lv_theme_shadcn_base_t(5);
    pub const LV_THEME_SHADCN_BASE_OLIVE: lv_theme_shadcn_base_t = lv_theme_shadcn_base_t(6);
    pub const LV_THEME_SHADCN_BASE_MIST: lv_theme_shadcn_base_t = lv_theme_shadcn_base_t(7);
    pub const LV_THEME_SHADCN_BASE_TAUPE: lv_theme_shadcn_base_t = lv_theme_shadcn_base_t(8);
    pub const LV_THEME_SHADCN_BASE_COUNT: lv_theme_shadcn_base_t = lv_theme_shadcn_base_t(9);
}
#[repr(transparent)]
#[derive(Debug, Copy, Clone, Hash, PartialEq, Eq)]
pub struct lv_theme_shadcn_base_t(pub ::core::ffi::c_uint);
impl lv_theme_shadcn_accent_t {
    pub const LV_THEME_SHADCN_ACCENT_NONE: lv_theme_shadcn_accent_t = lv_theme_shadcn_accent_t(0);
    pub const LV_THEME_SHADCN_ACCENT_AMBER: lv_theme_shadcn_accent_t = lv_theme_shadcn_accent_t(1);
    pub const LV_THEME_SHADCN_ACCENT_BLUE: lv_theme_shadcn_accent_t = lv_theme_shadcn_accent_t(2);
    pub const LV_THEME_SHADCN_ACCENT_CYAN: lv_theme_shadcn_accent_t = lv_theme_shadcn_accent_t(3);
    pub const LV_THEME_SHADCN_ACCENT_EMERALD: lv_theme_shadcn_accent_t =
        lv_theme_shadcn_accent_t(4);
    pub const LV_THEME_SHADCN_ACCENT_FUCHSIA: lv_theme_shadcn_accent_t =
        lv_theme_shadcn_accent_t(5);
    pub const LV_THEME_SHADCN_ACCENT_GREEN: lv_theme_shadcn_accent_t = lv_theme_shadcn_accent_t(6);
    pub const LV_THEME_SHADCN_ACCENT_INDIGO: lv_theme_shadcn_accent_t = lv_theme_shadcn_accent_t(7);
    pub const LV_THEME_SHADCN_ACCENT_LIME: lv_theme_shadcn_accent_t = lv_theme_shadcn_accent_t(8);
    pub const LV_THEME_SHADCN_ACCENT_ORANGE: lv_theme_shadcn_accent_t = lv_theme_shadcn_accent_t(9);
    pub const LV_THEME_SHADCN_ACCENT_PINK: lv_theme_shadcn_accent_t = lv_theme_shadcn_accent_t(10);
    pub const LV_THEME_SHADCN_ACCENT_PURPLE: lv_theme_shadcn_accent_t =
        lv_theme_shadcn_accent_t(11);
    pub const LV_THEME_SHADCN_ACCENT_RED: lv_theme_shadcn_accent_t = lv_theme_shadcn_accent_t(12);
    pub const LV_THEME_SHADCN_ACCENT_ROSE: lv_theme_shadcn_accent_t = lv_theme_shadcn_accent_t(13);
    pub const LV_THEME_SHADCN_ACCENT_SKY: lv_theme_shadcn_accent_t = lv_theme_shadcn_accent_t(14);
    pub const LV_THEME_SHADCN_ACCENT_TEAL: lv_theme_shadcn_accent_t = lv_theme_shadcn_accent_t(15);
    pub const LV_THEME_SHADCN_ACCENT_VIOLET: lv_theme_shadcn_accent_t =
        lv_theme_shadcn_accent_t(16);
    pub const LV_THEME_SHADCN_ACCENT_YELLOW: lv_theme_shadcn_accent_t =
        lv_theme_shadcn_accent_t(17);
    pub const LV_THEME_SHADCN_ACCENT_COUNT: lv_theme_shadcn_accent_t = lv_theme_shadcn_accent_t(18);
}
#[repr(transparent)]
#[derive(Debug, Copy, Clone, Hash, PartialEq, Eq)]
pub struct lv_theme_shadcn_accent_t(pub ::core::ffi::c_uint);
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct lv_theme_shadcn_fonts_t {
    pub small: *const lv_font_t,
    pub normal: *const lv_font_t,
    pub large: *const lv_font_t,
    pub mono: *const lv_font_t,
    pub h1: *const lv_font_t,
    pub h2: *const lv_font_t,
    pub h3: *const lv_font_t,
}
impl lv_theme_shadcn_flags_t {
    pub const LV_THEME_SHADCN_FLAG_NONE: lv_theme_shadcn_flags_t = lv_theme_shadcn_flags_t(0);
    pub const LV_THEME_SHADCN_FLAG_COMPACT: lv_theme_shadcn_flags_t = lv_theme_shadcn_flags_t(1);
    pub const LV_THEME_SHADCN_FLAG_REDUCED_MOTION: lv_theme_shadcn_flags_t =
        lv_theme_shadcn_flags_t(2);
}
#[repr(transparent)]
#[derive(Debug, Copy, Clone, Hash, PartialEq, Eq)]
pub struct lv_theme_shadcn_flags_t(pub ::core::ffi::c_uint);
impl lv_theme_shadcn_variant_t {
    pub const LV_THEME_SHADCN_VARIANT_DEFAULT: lv_theme_shadcn_variant_t =
        lv_theme_shadcn_variant_t(0);
    pub const LV_THEME_SHADCN_VARIANT_SECONDARY: lv_theme_shadcn_variant_t =
        lv_theme_shadcn_variant_t(1);
    pub const LV_THEME_SHADCN_VARIANT_OUTLINE: lv_theme_shadcn_variant_t =
        lv_theme_shadcn_variant_t(2);
    pub const LV_THEME_SHADCN_VARIANT_GHOST: lv_theme_shadcn_variant_t =
        lv_theme_shadcn_variant_t(3);
    pub const LV_THEME_SHADCN_VARIANT_DESTRUCTIVE: lv_theme_shadcn_variant_t =
        lv_theme_shadcn_variant_t(4);
}
#[repr(transparent)]
#[derive(Debug, Copy, Clone, Hash, PartialEq, Eq)]
pub struct lv_theme_shadcn_variant_t(pub ::core::ffi::c_uint);
impl lv_theme_shadcn_surface_t {
    pub const LV_THEME_SHADCN_SURFACE_BACKGROUND: lv_theme_shadcn_surface_t =
        lv_theme_shadcn_surface_t(0);
    pub const LV_THEME_SHADCN_SURFACE_CARD: lv_theme_shadcn_surface_t =
        lv_theme_shadcn_surface_t(1);
    pub const LV_THEME_SHADCN_SURFACE_POPOVER: lv_theme_shadcn_surface_t =
        lv_theme_shadcn_surface_t(2);
    pub const LV_THEME_SHADCN_SURFACE_MUTED: lv_theme_shadcn_surface_t =
        lv_theme_shadcn_surface_t(3);
}
#[repr(transparent)]
#[derive(Debug, Copy, Clone, Hash, PartialEq, Eq)]
pub struct lv_theme_shadcn_surface_t(pub ::core::ffi::c_uint);
impl lv_theme_shadcn_radius_t {
    pub const LV_THEME_SHADCN_RADIUS_NONE: lv_theme_shadcn_radius_t = lv_theme_shadcn_radius_t(0);
    pub const LV_THEME_SHADCN_RADIUS_SM: lv_theme_shadcn_radius_t = lv_theme_shadcn_radius_t(1);
    pub const LV_THEME_SHADCN_RADIUS_MD: lv_theme_shadcn_radius_t = lv_theme_shadcn_radius_t(2);
    pub const LV_THEME_SHADCN_RADIUS_DEFAULT: lv_theme_shadcn_radius_t =
        lv_theme_shadcn_radius_t(3);
    pub const LV_THEME_SHADCN_RADIUS_LG: lv_theme_shadcn_radius_t = lv_theme_shadcn_radius_t(4);
    pub const LV_THEME_SHADCN_RADIUS_XL: lv_theme_shadcn_radius_t = lv_theme_shadcn_radius_t(5);
}
#[repr(transparent)]
#[derive(Debug, Copy, Clone, Hash, PartialEq, Eq)]
pub struct lv_theme_shadcn_radius_t(pub ::core::ffi::c_uint);
impl lv_theme_shadcn_size_t {
    pub const LV_THEME_SHADCN_SIZE_DEFAULT: lv_theme_shadcn_size_t = lv_theme_shadcn_size_t(0);
    pub const LV_THEME_SHADCN_SIZE_SM: lv_theme_shadcn_size_t = lv_theme_shadcn_size_t(1);
    pub const LV_THEME_SHADCN_SIZE_LG: lv_theme_shadcn_size_t = lv_theme_shadcn_size_t(2);
    pub const LV_THEME_SHADCN_SIZE_ICON: lv_theme_shadcn_size_t = lv_theme_shadcn_size_t(3);
    pub const LV_THEME_SHADCN_SIZE_ICON_SM: lv_theme_shadcn_size_t = lv_theme_shadcn_size_t(4);
    pub const LV_THEME_SHADCN_SIZE_ICON_LG: lv_theme_shadcn_size_t = lv_theme_shadcn_size_t(5);
}
#[repr(transparent)]
#[derive(Debug, Copy, Clone, Hash, PartialEq, Eq)]
pub struct lv_theme_shadcn_size_t(pub ::core::ffi::c_uint);
impl lv_theme_shadcn_text_t {
    pub const LV_THEME_SHADCN_TEXT_DEFAULT: lv_theme_shadcn_text_t = lv_theme_shadcn_text_t(0);
    pub const LV_THEME_SHADCN_TEXT_H1: lv_theme_shadcn_text_t = lv_theme_shadcn_text_t(1);
    pub const LV_THEME_SHADCN_TEXT_H2: lv_theme_shadcn_text_t = lv_theme_shadcn_text_t(2);
    pub const LV_THEME_SHADCN_TEXT_H3: lv_theme_shadcn_text_t = lv_theme_shadcn_text_t(3);
    pub const LV_THEME_SHADCN_TEXT_H4: lv_theme_shadcn_text_t = lv_theme_shadcn_text_t(4);
    pub const LV_THEME_SHADCN_TEXT_P: lv_theme_shadcn_text_t = lv_theme_shadcn_text_t(5);
    pub const LV_THEME_SHADCN_TEXT_LEAD: lv_theme_shadcn_text_t = lv_theme_shadcn_text_t(6);
    pub const LV_THEME_SHADCN_TEXT_LARGE: lv_theme_shadcn_text_t = lv_theme_shadcn_text_t(7);
    pub const LV_THEME_SHADCN_TEXT_SMALL: lv_theme_shadcn_text_t = lv_theme_shadcn_text_t(8);
    pub const LV_THEME_SHADCN_TEXT_MUTED: lv_theme_shadcn_text_t = lv_theme_shadcn_text_t(9);
    pub const LV_THEME_SHADCN_TEXT_CODE: lv_theme_shadcn_text_t = lv_theme_shadcn_text_t(10);
}
#[repr(transparent)]
#[derive(Debug, Copy, Clone, Hash, PartialEq, Eq)]
pub struct lv_theme_shadcn_text_t(pub ::core::ffi::c_uint);
impl lv_theme_shadcn_role_t {
    pub const LV_THEME_SHADCN_ROLE_NONE: lv_theme_shadcn_role_t = lv_theme_shadcn_role_t(0);
    pub const LV_THEME_SHADCN_ROLE_BADGE: lv_theme_shadcn_role_t = lv_theme_shadcn_role_t(1);
    pub const LV_THEME_SHADCN_ROLE_BADGE_SECONDARY: lv_theme_shadcn_role_t =
        lv_theme_shadcn_role_t(2);
    pub const LV_THEME_SHADCN_ROLE_BADGE_OUTLINE: lv_theme_shadcn_role_t =
        lv_theme_shadcn_role_t(3);
    pub const LV_THEME_SHADCN_ROLE_BADGE_DESTRUCTIVE: lv_theme_shadcn_role_t =
        lv_theme_shadcn_role_t(4);
    pub const LV_THEME_SHADCN_ROLE_ALERT: lv_theme_shadcn_role_t = lv_theme_shadcn_role_t(5);
    pub const LV_THEME_SHADCN_ROLE_ALERT_DESTRUCTIVE: lv_theme_shadcn_role_t =
        lv_theme_shadcn_role_t(6);
    pub const LV_THEME_SHADCN_ROLE_SEPARATOR: lv_theme_shadcn_role_t = lv_theme_shadcn_role_t(7);
    pub const LV_THEME_SHADCN_ROLE_SEPARATOR_VERTICAL: lv_theme_shadcn_role_t =
        lv_theme_shadcn_role_t(8);
    pub const LV_THEME_SHADCN_ROLE_SKELETON: lv_theme_shadcn_role_t = lv_theme_shadcn_role_t(9);
    pub const LV_THEME_SHADCN_ROLE_CARD_HEADER: lv_theme_shadcn_role_t = lv_theme_shadcn_role_t(10);
    pub const LV_THEME_SHADCN_ROLE_CARD_TITLE: lv_theme_shadcn_role_t = lv_theme_shadcn_role_t(11);
    pub const LV_THEME_SHADCN_ROLE_CARD_DESCRIPTION: lv_theme_shadcn_role_t =
        lv_theme_shadcn_role_t(12);
    pub const LV_THEME_SHADCN_ROLE_CARD_CONTENT: lv_theme_shadcn_role_t =
        lv_theme_shadcn_role_t(13);
    pub const LV_THEME_SHADCN_ROLE_CARD_FOOTER: lv_theme_shadcn_role_t = lv_theme_shadcn_role_t(14);
}
#[repr(transparent)]
#[derive(Debug, Copy, Clone, Hash, PartialEq, Eq)]
pub struct lv_theme_shadcn_role_t(pub ::core::ffi::c_uint);
unsafe extern "C" {
    pub fn lv_theme_shadcn_init(
        disp: *mut lv_display_t,
        palette: *const lv_theme_shadcn_palette_t,
        fonts: *const lv_theme_shadcn_fonts_t,
        flags: lv_theme_shadcn_flags_t,
    ) -> *mut lv_theme_t;
}
unsafe extern "C" {
    pub fn lv_theme_shadcn_deinit(disp: *mut lv_display_t);
}
unsafe extern "C" {
    pub fn lv_theme_shadcn_is_active(disp: *mut lv_display_t) -> bool;
}
unsafe extern "C" {
    pub fn lv_theme_shadcn_set_palette(
        disp: *mut lv_display_t,
        palette: *const lv_theme_shadcn_palette_t,
    );
}
unsafe extern "C" {
    pub fn lv_theme_shadcn_get_palette(disp: *mut lv_display_t)
        -> *const lv_theme_shadcn_palette_t;
}
unsafe extern "C" {
    pub fn lv_theme_shadcn_set_radius(disp: *mut lv_display_t, radius: lv_theme_shadcn_radius_t);
}
unsafe extern "C" {
    pub fn lv_theme_shadcn_get_radius(disp: *mut lv_display_t) -> lv_theme_shadcn_radius_t;
}
unsafe extern "C" {
    pub fn lv_theme_shadcn_palette_zinc_dark() -> *const lv_theme_shadcn_palette_t;
}
unsafe extern "C" {
    pub fn lv_theme_shadcn_palette_zinc_light() -> *const lv_theme_shadcn_palette_t;
}
unsafe extern "C" {
    pub fn lv_theme_shadcn_palette_from_primary(
        out: *mut lv_theme_shadcn_palette_t,
        base: *const lv_theme_shadcn_palette_t,
        primary: lv_color_t,
        primary_fg: lv_color_t,
    );
}
unsafe extern "C" {
    pub fn lv_theme_shadcn_palette_make(
        out: *mut lv_theme_shadcn_palette_t,
        base: lv_theme_shadcn_base_t,
        accent: lv_theme_shadcn_accent_t,
        dark: bool,
    );
}
unsafe extern "C" {
    pub fn lv_theme_shadcn_base_name(base: lv_theme_shadcn_base_t) -> *const ::core::ffi::c_char;
}
unsafe extern "C" {
    pub fn lv_theme_shadcn_accent_name(
        accent: lv_theme_shadcn_accent_t,
    ) -> *const ::core::ffi::c_char;
}
unsafe extern "C" {
    pub fn lv_theme_shadcn_set_variant(obj: *mut lv_obj_t, variant: lv_theme_shadcn_variant_t);
}
unsafe extern "C" {
    pub fn lv_theme_shadcn_set_size(obj: *mut lv_obj_t, size: lv_theme_shadcn_size_t);
}
unsafe extern "C" {
    pub fn lv_theme_shadcn_set_surface(obj: *mut lv_obj_t, surface: lv_theme_shadcn_surface_t);
}
unsafe extern "C" {
    pub fn lv_theme_shadcn_set_error(obj: *mut lv_obj_t, error: bool);
}
unsafe extern "C" {
    pub fn lv_theme_shadcn_set_text_role(obj: *mut lv_obj_t, role: lv_theme_shadcn_text_t);
}
unsafe extern "C" {
    pub fn lv_theme_shadcn_set_role(obj: *mut lv_obj_t, role: lv_theme_shadcn_role_t);
}
