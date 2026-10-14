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
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct lv_theme_shadcn_fonts_t {
    pub small: *const lv_font_t,
    pub normal: *const lv_font_t,
    pub large: *const lv_font_t,
    pub mono: *const lv_font_t,
}
impl lv_theme_shadcn_flags_t {
    pub const LV_THEME_SHADCN_FLAG_NONE: lv_theme_shadcn_flags_t = lv_theme_shadcn_flags_t(0);
    pub const LV_THEME_SHADCN_FLAG_COMPACT: lv_theme_shadcn_flags_t = lv_theme_shadcn_flags_t(1);
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
    pub fn lv_theme_shadcn_set_variant(obj: *mut lv_obj_t, variant: lv_theme_shadcn_variant_t);
}
unsafe extern "C" {
    pub fn lv_theme_shadcn_set_surface(obj: *mut lv_obj_t, surface: lv_theme_shadcn_surface_t);
}
unsafe extern "C" {
    pub fn lv_theme_shadcn_set_error(obj: *mut lv_obj_t, error: bool);
}
