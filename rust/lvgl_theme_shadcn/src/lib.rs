//! Safe `no_std` bindings to `lv_theme_shadcn`, a shadcn/ui-inspired theme for LVGL 9.6+.
//!
//! This crate does not depend on any LVGL Rust binding. LVGL objects are passed in as raw pointers through
//! [`Display::from_ptr`], [`Object::from_ptr`] and [`Font::from_ptr`], which accept a pointer to any type, so
//! handles from any LVGL binding can be used. Linking LVGL itself is the application's job.
//!
//! # Locating LVGL
//!
//! The C theme is compiled by `lvgl_theme_shadcn_sys`, which needs LVGL headers and `lv_conf.h`:
//!
//! - `LVGL_INCLUDE_DIR`: the LVGL root directory (the one containing `lvgl.h`);
//! - `LV_CONF_PATH`: the directory containing `lv_conf.h` (the one used by the LVGL you link against).
//!
//! They are usually set in `.cargo/config.toml`:
//!
//! ```toml
//! [env]
//! LVGL_INCLUDE_DIR = { value = "third_party/lvgl", relative = true }
//! LV_CONF_PATH = { value = "config", relative = true }
//! ```
//!
//! `DEP_LVGL_INCLUDE` and `DEP_LVGL_CONF_PATH` take precedence, but Cargo only sets them when the sys crate is built
//! as a direct dependent of a crate declaring `links = "lvgl"`, which is rare.
//!
//! # Example
//!
//! ```no_run
//! use lvgl_theme_shadcn::{Display, Flags, Fonts, Palette, Theme};
//!
//! unsafe extern "C" {
//!     fn lv_init();
//!     fn lv_display_create(hor_res: i32, ver_res: i32) -> *mut core::ffi::c_void;
//! }
//!
//! let theme = unsafe {
//!     lv_init();
//!     let display = Display::from_ptr(lv_display_create(480, 320)).expect("display");
//!     Theme::init(display, &Palette::zinc_dark(), &Fonts::default(), Flags::NONE).expect("theme")
//! };
//! theme.set_palette(&Palette::zinc_light());
//! assert!(!theme.palette().expect("palette").is_dark());
//! ```
//!
//! # Palettes
//!
//! Every shadcn/ui base colour ([`Base`]) can be combined with every accent ([`Accent`]) in light or dark mode with
//! [`Palette::new`]. [`Theme::set_palette`] restyles existing widgets in place, so palettes can be switched at
//! runtime:
//!
//! ```no_run
//! use lvgl_theme_shadcn::{Accent, Base, Palette, Theme};
//!
//! fn apply(theme: &Theme, base: Base, accent: Accent, dark: bool) {
//!     theme.set_palette(&Palette::new(base, accent, dark));
//! }
//!
//! for base in Base::ALL {
//!     for accent in Accent::ALL {
//!         let palette = Palette::new(base, accent, true);
//!         assert!(palette.is_dark());
//!         let _label = (base.name(), accent.name());
//!     }
//! }
//! ```
//!
//! # Interoperability with another LVGL binding
//!
//! With a binding such as `lvgl_rust_sys`, pointers are passed directly and converted back with `.cast()`:
//!
//! ```ignore
//! use lvgl_rust_sys as lv;
//! use lvgl_theme_shadcn::{Display, Flags, Font, Fonts, Object, Palette, Theme, Variant};
//!
//! unsafe {
//!     let display = Display::from_ptr(lv::lv_display_create(480, 320)).unwrap();
//!     let fonts = Fonts {
//!         normal: Font::from_const_ptr(&raw const lv::lv_font_montserrat_14),
//!         ..Fonts::default()
//!     };
//!     let theme = Theme::init(display, &Palette::zinc_dark(), &fonts, Flags::COMPACT).unwrap();
//!     let screen = lv::lv_display_get_screen_active(theme.display().as_ptr().cast());
//!     let button = Object::from_ptr(lv::lv_button_create(screen)).unwrap();
//!     lvgl_theme_shadcn::set_variant(&button, Variant::Destructive);
//! }
//! ```
#![no_std]
#![deny(missing_docs)]

use core::ffi::c_uint;
use core::marker::PhantomData;
use core::ops::{BitOr, BitOrAssign};
use core::ptr::{self, NonNull};

pub use lvgl_theme_shadcn_sys as sys;

/// Handle to an LVGL display (`lv_display_t`).
#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash)]
pub struct Display(NonNull<sys::lv_display_t>);

impl Display {
    /// Wraps a display pointer of any type, returning `None` if it is null.
    ///
    /// # Safety
    ///
    /// `ptr` must point to a live `lv_display_t` for as long as the handle or a copy is used, and the handle must
    /// only be used on the thread running LVGL.
    pub unsafe fn from_ptr<T>(ptr: *mut T) -> Option<Self> {
        NonNull::new(ptr.cast()).map(Self)
    }

    /// Returns the underlying display pointer.
    pub fn as_ptr(&self) -> *mut sys::lv_display_t {
        self.0.as_ptr()
    }
}

/// Handle to an LVGL object (`lv_obj_t`).
#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash)]
pub struct Object(NonNull<sys::lv_obj_t>);

impl Object {
    /// Wraps an object pointer of any type, returning `None` if it is null.
    ///
    /// # Safety
    ///
    /// `ptr` must point to an `lv_obj_t`; it must still be alive whenever the handle is passed to this crate.
    pub unsafe fn from_ptr<T>(ptr: *mut T) -> Option<Self> {
        NonNull::new(ptr.cast()).map(Self)
    }

    /// Returns the underlying object pointer.
    pub fn as_ptr(&self) -> *mut sys::lv_obj_t {
        self.0.as_ptr()
    }
}

/// Handle to an LVGL font (`lv_font_t`).
#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash)]
pub struct Font(NonNull<sys::lv_font_t>);

impl Font {
    /// Wraps a font pointer of any type, returning `None` if it is null.
    ///
    /// # Safety
    ///
    /// `ptr` must point to an `lv_font_t` that outlives every theme using it.
    pub unsafe fn from_ptr<T>(ptr: *mut T) -> Option<Self> {
        NonNull::new(ptr.cast()).map(Self)
    }

    /// Wraps a const font pointer of any type, such as `&raw const lv_font_montserrat_14`.
    ///
    /// # Safety
    ///
    /// Same as [`Font::from_ptr`]; the font is never written through.
    pub unsafe fn from_const_ptr<T>(ptr: *const T) -> Option<Self> {
        NonNull::new(ptr.cast_mut().cast()).map(Self)
    }

    /// Returns the underlying font pointer.
    pub fn as_ptr(&self) -> *const sys::lv_font_t {
        self.0.as_ptr()
    }
}

/// An RGB colour with 8 bits per channel.
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq, Hash)]
pub struct Color {
    /// Red channel.
    pub red: u8,
    /// Green channel.
    pub green: u8,
    /// Blue channel.
    pub blue: u8,
}

impl Color {
    /// Creates a colour from its channels.
    pub const fn rgb(red: u8, green: u8, blue: u8) -> Self {
        Self { red, green, blue }
    }

    /// Creates a colour from a `0xRRGGBB` value; the top byte is ignored.
    pub const fn hex(value: u32) -> Self {
        Self::rgb((value >> 16) as u8, (value >> 8) as u8, value as u8)
    }

    /// Returns the colour as a `0xRRGGBB` value.
    pub const fn to_hex(self) -> u32 {
        (self.red as u32) << 16 | (self.green as u32) << 8 | self.blue as u32
    }

    /// Converts an LVGL colour.
    pub const fn from_raw(raw: sys::lv_color_t) -> Self {
        Self::rgb(raw.red, raw.green, raw.blue)
    }

    /// Converts to an LVGL colour.
    pub const fn to_raw(self) -> sys::lv_color_t {
        sys::lv_color_t {
            red: self.red,
            green: self.green,
            blue: self.blue,
        }
    }
}

impl From<sys::lv_color_t> for Color {
    fn from(raw: sys::lv_color_t) -> Self {
        Self::from_raw(raw)
    }
}

impl From<Color> for sys::lv_color_t {
    fn from(color: Color) -> Self {
        color.to_raw()
    }
}

/// Base colour of a shadcn/ui palette: the neutral tones used for backgrounds, surfaces, borders and text.
///
/// `Gray` and `Slate` come from shadcn/ui's legacy base colours; the others from its current registry.
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq, Hash)]
pub enum Base {
    /// shadcn/ui `neutral`.
    Neutral,
    /// shadcn/ui `stone`.
    Stone,
    /// shadcn/ui `zinc`, the default.
    #[default]
    Zinc,
    /// shadcn/ui `gray`.
    Gray,
    /// shadcn/ui `slate`.
    Slate,
    /// shadcn/ui `mauve`.
    Mauve,
    /// shadcn/ui `olive`.
    Olive,
    /// shadcn/ui `mist`.
    Mist,
    /// shadcn/ui `taupe`.
    Taupe,
}

impl Base {
    /// Every base colour, in declaration order.
    pub const ALL: [Base; 9] = [
        Base::Neutral,
        Base::Stone,
        Base::Zinc,
        Base::Gray,
        Base::Slate,
        Base::Mauve,
        Base::Olive,
        Base::Mist,
        Base::Taupe,
    ];

    /// Returns the lowercase shadcn/ui name, such as `"zinc"`.
    pub const fn name(self) -> &'static str {
        match self {
            Base::Neutral => "neutral",
            Base::Stone => "stone",
            Base::Zinc => "zinc",
            Base::Gray => "gray",
            Base::Slate => "slate",
            Base::Mauve => "mauve",
            Base::Olive => "olive",
            Base::Mist => "mist",
            Base::Taupe => "taupe",
        }
    }
}

impl From<Base> for sys::lv_theme_shadcn_base_t {
    fn from(base: Base) -> Self {
        match base {
            Base::Neutral => Self::LV_THEME_SHADCN_BASE_NEUTRAL,
            Base::Stone => Self::LV_THEME_SHADCN_BASE_STONE,
            Base::Zinc => Self::LV_THEME_SHADCN_BASE_ZINC,
            Base::Gray => Self::LV_THEME_SHADCN_BASE_GRAY,
            Base::Slate => Self::LV_THEME_SHADCN_BASE_SLATE,
            Base::Mauve => Self::LV_THEME_SHADCN_BASE_MAUVE,
            Base::Olive => Self::LV_THEME_SHADCN_BASE_OLIVE,
            Base::Mist => Self::LV_THEME_SHADCN_BASE_MIST,
            Base::Taupe => Self::LV_THEME_SHADCN_BASE_TAUPE,
        }
    }
}

/// Accent colour of a shadcn/ui palette, overriding `primary`, `primary_fg`, `secondary`, `secondary_fg` and the
/// chart colours of the base.
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq, Hash)]
pub enum Accent {
    /// No accent: the base colour's own monochrome `primary` and charts.
    #[default]
    None,
    /// shadcn/ui `amber`.
    Amber,
    /// shadcn/ui `blue`.
    Blue,
    /// shadcn/ui `cyan`.
    Cyan,
    /// shadcn/ui `emerald`.
    Emerald,
    /// shadcn/ui `fuchsia`.
    Fuchsia,
    /// shadcn/ui `green`.
    Green,
    /// shadcn/ui `indigo`.
    Indigo,
    /// shadcn/ui `lime`.
    Lime,
    /// shadcn/ui `orange`.
    Orange,
    /// shadcn/ui `pink`.
    Pink,
    /// shadcn/ui `purple`.
    Purple,
    /// shadcn/ui `red`.
    Red,
    /// shadcn/ui `rose`.
    Rose,
    /// shadcn/ui `sky`.
    Sky,
    /// shadcn/ui `teal`.
    Teal,
    /// shadcn/ui `violet`.
    Violet,
    /// shadcn/ui `yellow`.
    Yellow,
}

impl Accent {
    /// Every accent, starting with [`Accent::None`] and then in alphabetical order.
    pub const ALL: [Accent; 18] = [
        Accent::None,
        Accent::Amber,
        Accent::Blue,
        Accent::Cyan,
        Accent::Emerald,
        Accent::Fuchsia,
        Accent::Green,
        Accent::Indigo,
        Accent::Lime,
        Accent::Orange,
        Accent::Pink,
        Accent::Purple,
        Accent::Red,
        Accent::Rose,
        Accent::Sky,
        Accent::Teal,
        Accent::Violet,
        Accent::Yellow,
    ];

    /// Returns the lowercase shadcn/ui name, such as `"blue"`, or `"none"`.
    pub const fn name(self) -> &'static str {
        match self {
            Accent::None => "none",
            Accent::Amber => "amber",
            Accent::Blue => "blue",
            Accent::Cyan => "cyan",
            Accent::Emerald => "emerald",
            Accent::Fuchsia => "fuchsia",
            Accent::Green => "green",
            Accent::Indigo => "indigo",
            Accent::Lime => "lime",
            Accent::Orange => "orange",
            Accent::Pink => "pink",
            Accent::Purple => "purple",
            Accent::Red => "red",
            Accent::Rose => "rose",
            Accent::Sky => "sky",
            Accent::Teal => "teal",
            Accent::Violet => "violet",
            Accent::Yellow => "yellow",
        }
    }
}

impl From<Accent> for sys::lv_theme_shadcn_accent_t {
    fn from(accent: Accent) -> Self {
        match accent {
            Accent::None => Self::LV_THEME_SHADCN_ACCENT_NONE,
            Accent::Amber => Self::LV_THEME_SHADCN_ACCENT_AMBER,
            Accent::Blue => Self::LV_THEME_SHADCN_ACCENT_BLUE,
            Accent::Cyan => Self::LV_THEME_SHADCN_ACCENT_CYAN,
            Accent::Emerald => Self::LV_THEME_SHADCN_ACCENT_EMERALD,
            Accent::Fuchsia => Self::LV_THEME_SHADCN_ACCENT_FUCHSIA,
            Accent::Green => Self::LV_THEME_SHADCN_ACCENT_GREEN,
            Accent::Indigo => Self::LV_THEME_SHADCN_ACCENT_INDIGO,
            Accent::Lime => Self::LV_THEME_SHADCN_ACCENT_LIME,
            Accent::Orange => Self::LV_THEME_SHADCN_ACCENT_ORANGE,
            Accent::Pink => Self::LV_THEME_SHADCN_ACCENT_PINK,
            Accent::Purple => Self::LV_THEME_SHADCN_ACCENT_PURPLE,
            Accent::Red => Self::LV_THEME_SHADCN_ACCENT_RED,
            Accent::Rose => Self::LV_THEME_SHADCN_ACCENT_ROSE,
            Accent::Sky => Self::LV_THEME_SHADCN_ACCENT_SKY,
            Accent::Teal => Self::LV_THEME_SHADCN_ACCENT_TEAL,
            Accent::Violet => Self::LV_THEME_SHADCN_ACCENT_VIOLET,
            Accent::Yellow => Self::LV_THEME_SHADCN_ACCENT_YELLOW,
        }
    }
}

/// The colour roles of a theme, mirroring shadcn/ui CSS variables.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct Palette(sys::lv_theme_shadcn_palette_t);

macro_rules! palette_roles {
    ($($get:ident $set:ident),* $(,)?) => {
        impl Palette {
            $(
                #[doc = concat!("Returns the `", stringify!($get), "` colour.")]
                pub const fn $get(&self) -> Color {
                    Color::from_raw(self.0.$get)
                }

                #[doc = concat!("Sets the `", stringify!($get), "` colour.")]
                pub const fn $set(&mut self, color: Color) {
                    self.0.$get = color.to_raw();
                }
            )*
        }
    };
}

palette_roles!(
    background set_background,
    foreground set_foreground,
    card set_card,
    card_fg set_card_fg,
    popover set_popover,
    popover_fg set_popover_fg,
    primary set_primary,
    primary_fg set_primary_fg,
    secondary set_secondary,
    secondary_fg set_secondary_fg,
    muted set_muted,
    muted_fg set_muted_fg,
    accent set_accent,
    accent_fg set_accent_fg,
    destructive set_destructive,
    destructive_fg set_destructive_fg,
    border set_border,
    input set_input,
    ring set_ring,
);

impl Palette {
    /// Number of chart colours.
    pub const CHART_COUNT: usize = 5;

    /// Builds the shadcn/ui palette for `base` with `accent`, in dark or light mode.
    ///
    /// The tables are generated from the shadcn/ui registry and stored in flash; nothing is allocated. Switch
    /// palettes at runtime with [`Theme::set_palette`]:
    ///
    /// ```no_run
    /// # use lvgl_theme_shadcn::{Accent, Base, Palette, Theme};
    /// # fn switch(theme: &Theme) {
    /// theme.set_palette(&Palette::new(Base::Slate, Accent::Blue, true));
    /// # }
    /// ```
    pub fn new(base: Base, accent: Accent, dark: bool) -> Self {
        let mut out = sys::lv_theme_shadcn_palette_t::default();
        unsafe { sys::lv_theme_shadcn_palette_make(&mut out, base.into(), accent.into(), dark) };
        Self(out)
    }

    /// The built-in dark zinc palette.
    pub fn zinc_dark() -> Self {
        unsafe { Self(*sys::lv_theme_shadcn_palette_zinc_dark()) }
    }

    /// The built-in light zinc palette.
    pub fn zinc_light() -> Self {
        unsafe { Self(*sys::lv_theme_shadcn_palette_zinc_light()) }
    }

    /// Returns a copy of this palette with new `primary` and `primary_fg` colours and a matching `ring`.
    pub fn with_primary(&self, primary: Color, primary_fg: Color) -> Palette {
        let mut out = self.0;
        unsafe {
            sys::lv_theme_shadcn_palette_from_primary(
                &mut out,
                &self.0,
                primary.to_raw(),
                primary_fg.to_raw(),
            );
        }
        Self(out)
    }

    /// Wraps a raw palette.
    pub const fn from_raw(raw: sys::lv_theme_shadcn_palette_t) -> Self {
        Self(raw)
    }

    /// Returns the raw palette.
    pub const fn as_raw(&self) -> &sys::lv_theme_shadcn_palette_t {
        &self.0
    }

    /// Whether the palette is dark.
    pub const fn is_dark(&self) -> bool {
        self.0.dark
    }

    /// Sets whether the palette is dark.
    pub const fn set_dark(&mut self, dark: bool) {
        self.0.dark = dark;
    }

    /// Returns chart colour `index`.
    ///
    /// # Panics
    ///
    /// Panics if `index >= Palette::CHART_COUNT`.
    pub const fn chart(&self, index: usize) -> Color {
        Color::from_raw(self.0.chart[index])
    }

    /// Sets chart colour `index`.
    ///
    /// # Panics
    ///
    /// Panics if `index >= Palette::CHART_COUNT`.
    pub const fn set_chart(&mut self, index: usize, color: Color) {
        self.0.chart[index] = color.to_raw();
    }
}

impl From<sys::lv_theme_shadcn_palette_t> for Palette {
    fn from(raw: sys::lv_theme_shadcn_palette_t) -> Self {
        Self(raw)
    }
}

impl From<Palette> for sys::lv_theme_shadcn_palette_t {
    fn from(palette: Palette) -> Self {
        palette.0
    }
}

/// Fonts used by the theme; `None` falls back to `normal`, then to `LV_FONT_DEFAULT`.
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq)]
pub struct Fonts {
    /// Font for captions and secondary text.
    pub small: Option<Font>,
    /// Font for body text.
    pub normal: Option<Font>,
    /// Font for titles.
    pub large: Option<Font>,
    /// Monospace font.
    pub mono: Option<Font>,
}

impl Fonts {
    fn to_raw(self) -> sys::lv_theme_shadcn_fonts_t {
        let raw = |font: Option<Font>| font.map_or(ptr::null(), |font| font.as_ptr());
        sys::lv_theme_shadcn_fonts_t {
            small: raw(self.small),
            normal: raw(self.normal),
            large: raw(self.large),
            mono: raw(self.mono),
        }
    }
}

/// Theme initialisation flags.
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq, Hash)]
pub struct Flags(u32);

impl Flags {
    /// No flags.
    pub const NONE: Self = Self(0);
    /// Smaller paddings and sizes for small displays.
    pub const COMPACT: Self = Self(1);

    /// Returns the raw bits.
    pub const fn bits(self) -> u32 {
        self.0
    }

    /// Whether all flags in `other` are set.
    pub const fn contains(self, other: Self) -> bool {
        self.0 & other.0 == other.0
    }
}

impl BitOr for Flags {
    type Output = Self;

    fn bitor(self, rhs: Self) -> Self {
        Self(self.0 | rhs.0)
    }
}

impl BitOrAssign for Flags {
    fn bitor_assign(&mut self, rhs: Self) {
        self.0 |= rhs.0;
    }
}

/// Visual variant of an action widget such as a button.
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq, Hash)]
pub enum Variant {
    /// Filled with the primary colour.
    #[default]
    Default,
    /// Filled with the secondary colour.
    Secondary,
    /// Bordered with a transparent background.
    Outline,
    /// Transparent background, accent colour when pressed.
    Ghost,
    /// Filled with the destructive colour.
    Destructive,
}

impl From<Variant> for sys::lv_theme_shadcn_variant_t {
    fn from(variant: Variant) -> Self {
        match variant {
            Variant::Default => Self::LV_THEME_SHADCN_VARIANT_DEFAULT,
            Variant::Secondary => Self::LV_THEME_SHADCN_VARIANT_SECONDARY,
            Variant::Outline => Self::LV_THEME_SHADCN_VARIANT_OUTLINE,
            Variant::Ghost => Self::LV_THEME_SHADCN_VARIANT_GHOST,
            Variant::Destructive => Self::LV_THEME_SHADCN_VARIANT_DESTRUCTIVE,
        }
    }
}

/// Background surface of a container.
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq, Hash)]
pub enum Surface {
    /// Page background.
    #[default]
    Background,
    /// Card surface.
    Card,
    /// Popover surface.
    Popover,
    /// Muted surface.
    Muted,
}

impl From<Surface> for sys::lv_theme_shadcn_surface_t {
    fn from(surface: Surface) -> Self {
        match surface {
            Surface::Background => Self::LV_THEME_SHADCN_SURFACE_BACKGROUND,
            Surface::Card => Self::LV_THEME_SHADCN_SURFACE_CARD,
            Surface::Popover => Self::LV_THEME_SHADCN_SURFACE_POPOVER,
            Surface::Muted => Self::LV_THEME_SHADCN_SURFACE_MUTED,
        }
    }
}

/// The shadcn theme applied to a display; dropping it restores the previous theme.
#[derive(Debug)]
pub struct Theme {
    display: Display,
    _not_send_sync: PhantomData<*mut ()>,
}

impl Theme {
    /// Applies the theme to `display`, returning `None` if LVGL cannot allocate it.
    ///
    /// Initialising a display that already has a shadcn theme updates it in place. Only one `Theme` per display may
    /// exist: the C state is shared, so dropping either `Theme` tears it down.
    ///
    /// # Safety
    ///
    /// LVGL must be initialised and only used from the current thread, `display` must outlive the returned
    /// `Theme`, and the fonts must outlive the theme.
    pub unsafe fn init(
        display: Display,
        palette: &Palette,
        fonts: &Fonts,
        flags: Flags,
    ) -> Option<Theme> {
        let fonts = fonts.to_raw();
        let theme = unsafe {
            sys::lv_theme_shadcn_init(
                display.as_ptr(),
                &palette.0,
                &fonts,
                sys::lv_theme_shadcn_flags_t(flags.0 as c_uint),
            )
        };
        if theme.is_null() {
            return None;
        }
        Some(Theme {
            display,
            _not_send_sync: PhantomData,
        })
    }

    /// Returns the themed display.
    pub fn display(&self) -> &Display {
        &self.display
    }

    /// Replaces the palette and restyles every object on the display.
    pub fn set_palette(&self, palette: &Palette) {
        unsafe { sys::lv_theme_shadcn_set_palette(self.display.as_ptr(), &palette.0) }
    }

    /// Returns the current palette, or `None` if the C theme state no longer exists.
    pub fn palette(&self) -> Option<Palette> {
        let palette = unsafe { sys::lv_theme_shadcn_get_palette(self.display.as_ptr()) };
        if palette.is_null() {
            None
        } else {
            Some(Palette(unsafe { *palette }))
        }
    }

    /// Whether the shadcn theme is the active theme of `display`.
    pub fn is_active(display: &Display) -> bool {
        unsafe { sys::lv_theme_shadcn_is_active(display.as_ptr()) }
    }
}

impl Drop for Theme {
    fn drop(&mut self) {
        unsafe { sys::lv_theme_shadcn_deinit(self.display.as_ptr()) }
    }
}

/// Sets the visual variant of an action widget; ignored if its display has no shadcn theme.
///
/// # Safety
///
/// `object` must be a live LVGL object, used on the thread running LVGL.
pub unsafe fn set_variant(object: &Object, variant: Variant) {
    unsafe { sys::lv_theme_shadcn_set_variant(object.as_ptr(), variant.into()) }
}

/// Sets the background surface of a container; ignored if its display has no shadcn theme.
///
/// # Safety
///
/// `object` must be a live LVGL object, used on the thread running LVGL.
pub unsafe fn set_surface(object: &Object, surface: Surface) {
    unsafe { sys::lv_theme_shadcn_set_surface(object.as_ptr(), surface.into()) }
}

/// Marks an input as invalid or valid; ignored if its display has no shadcn theme.
///
/// # Safety
///
/// `object` must be a live LVGL object, used on the thread running LVGL.
pub unsafe fn set_error(object: &Object, error: bool) {
    unsafe { sys::lv_theme_shadcn_set_error(object.as_ptr(), error) }
}
