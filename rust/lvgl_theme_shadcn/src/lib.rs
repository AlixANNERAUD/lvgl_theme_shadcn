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
//! # Radius and sizes
//!
//! [`Theme::set_radius`] picks one of shadcn/ui's `--radius` options and restyles existing widgets in place.
//! [`set_size`] picks a shadcn/ui button size; small sizes keep a 48 dp touch target through an extended click area:
//!
//! ```no_run
//! use lvgl_theme_shadcn::{Object, Radius, Size, Theme, set_size};
//!
//! fn tweak(theme: &Theme, button: &Object) {
//!     theme.set_radius(Radius::Lg);
//!     unsafe { set_size(button, Size::IconSm) };
//! }
//! ```
//!
//! # Text and component roles
//!
//! [`set_text_role`] gives a label shadcn/ui's typography: headings, lead, muted text or inline code.
//! [`set_role`] turns a plain object or label into a badge, alert, separator, skeleton or card part. Both replace
//! the previous role of the same kind and follow palette and radius changes:
//!
//! ```no_run
//! use lvgl_theme_shadcn::{Object, Role, TextRole, set_role, set_text_role};
//!
//! fn decorate(title: &Object, status: &Object, divider: &Object) {
//!     unsafe {
//!         set_text_role(title, TextRole::H2);
//!         set_role(status, Role::BadgeSecondary);
//!         set_role(divider, Role::Separator);
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
///
/// Heading fonts fall back down the chain `h1` → `h2` → `h3` → `large` → `normal`, so setting only `h3` gives
/// every heading that font.
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
    /// Font for [`TextRole::H1`].
    pub h1: Option<Font>,
    /// Font for [`TextRole::H2`].
    pub h2: Option<Font>,
    /// Font for [`TextRole::H3`].
    pub h3: Option<Font>,
}

impl Fonts {
    fn to_raw(self) -> sys::lv_theme_shadcn_fonts_t {
        let raw = |font: Option<Font>| font.map_or(ptr::null(), |font| font.as_ptr());
        sys::lv_theme_shadcn_fonts_t {
            small: raw(self.small),
            normal: raw(self.normal),
            large: raw(self.large),
            mono: raw(self.mono),
            h1: raw(self.h1),
            h2: raw(self.h2),
            h3: raw(self.h3),
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
    /// Disables decorative animations such as the [`Role::Skeleton`] pulse.
    pub const REDUCED_MOTION: Self = Self(2);

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

/// Corner radius scale, mirroring shadcn/ui's `--radius` options.
///
/// With `r` the selected radius, widgets use shadcn/ui's derived radii: small elements such as checkboxes `0.6 r`,
/// controls such as buttons and inputs `0.8 r`, dialogs `r` and cards `1.4 r`. One rem is 16 dp.
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq, Hash)]
pub enum Radius {
    /// Square corners (`0`).
    None,
    /// `0.3rem`.
    Sm,
    /// `0.5rem`.
    Md,
    /// `0.625rem`, shadcn/ui's default.
    #[default]
    Default,
    /// `0.75rem`.
    Lg,
    /// `1rem`.
    Xl,
}

impl Radius {
    /// Every radius, from square to roundest.
    pub const ALL: [Radius; 6] = [
        Radius::None,
        Radius::Sm,
        Radius::Md,
        Radius::Default,
        Radius::Lg,
        Radius::Xl,
    ];

    /// Converts a raw radius, returning `None` for unknown values.
    pub const fn from_raw(raw: sys::lv_theme_shadcn_radius_t) -> Option<Self> {
        match raw {
            sys::lv_theme_shadcn_radius_t::LV_THEME_SHADCN_RADIUS_NONE => Some(Radius::None),
            sys::lv_theme_shadcn_radius_t::LV_THEME_SHADCN_RADIUS_SM => Some(Radius::Sm),
            sys::lv_theme_shadcn_radius_t::LV_THEME_SHADCN_RADIUS_MD => Some(Radius::Md),
            sys::lv_theme_shadcn_radius_t::LV_THEME_SHADCN_RADIUS_DEFAULT => Some(Radius::Default),
            sys::lv_theme_shadcn_radius_t::LV_THEME_SHADCN_RADIUS_LG => Some(Radius::Lg),
            sys::lv_theme_shadcn_radius_t::LV_THEME_SHADCN_RADIUS_XL => Some(Radius::Xl),
            _ => None,
        }
    }
}

impl From<Radius> for sys::lv_theme_shadcn_radius_t {
    fn from(radius: Radius) -> Self {
        match radius {
            Radius::None => Self::LV_THEME_SHADCN_RADIUS_NONE,
            Radius::Sm => Self::LV_THEME_SHADCN_RADIUS_SM,
            Radius::Md => Self::LV_THEME_SHADCN_RADIUS_MD,
            Radius::Default => Self::LV_THEME_SHADCN_RADIUS_DEFAULT,
            Radius::Lg => Self::LV_THEME_SHADCN_RADIUS_LG,
            Radius::Xl => Self::LV_THEME_SHADCN_RADIUS_XL,
        }
    }
}

/// Size of a control such as a button, mirroring shadcn/ui's button sizes.
///
/// Heights are 32, 40 and 48 dp for small, default and large (8 dp less with [`Flags::COMPACT`]). Small sizes use
/// the small font, and icon sizes are square. Controls smaller than the touch target get an extended click area.
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq, Hash)]
pub enum Size {
    /// The theme's default control height.
    #[default]
    Default,
    /// Shorter, with tighter padding and the small font.
    Sm,
    /// Taller, with wider padding.
    Lg,
    /// Square, at the default height.
    Icon,
    /// Square, at the small height.
    IconSm,
    /// Square, at the large height.
    IconLg,
}

impl Size {
    /// Every size, in declaration order.
    pub const ALL: [Size; 6] = [
        Size::Default,
        Size::Sm,
        Size::Lg,
        Size::Icon,
        Size::IconSm,
        Size::IconLg,
    ];
}

impl From<Size> for sys::lv_theme_shadcn_size_t {
    fn from(size: Size) -> Self {
        match size {
            Size::Default => Self::LV_THEME_SHADCN_SIZE_DEFAULT,
            Size::Sm => Self::LV_THEME_SHADCN_SIZE_SM,
            Size::Lg => Self::LV_THEME_SHADCN_SIZE_LG,
            Size::Icon => Self::LV_THEME_SHADCN_SIZE_ICON,
            Size::IconSm => Self::LV_THEME_SHADCN_SIZE_ICON_SM,
            Size::IconLg => Self::LV_THEME_SHADCN_SIZE_ICON_LG,
        }
    }
}

/// Typography role of a label, mirroring shadcn/ui's typography.
///
/// Roles set the font and, for some, the colour and spacing; text properties are inherited, so a role set on a
/// container applies to the labels inside it.
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq, Hash)]
pub enum TextRole {
    /// The inherited body text.
    #[default]
    Default,
    /// Page title, with [`Fonts::h1`].
    H1,
    /// Section title, with [`Fonts::h2`].
    H2,
    /// Subsection title, with [`Fonts::h3`].
    H3,
    /// Minor title, with [`Fonts::large`].
    H4,
    /// Paragraph: the normal font with relaxed line spacing.
    P,
    /// Introductory text: the large font in the muted foreground colour.
    Lead,
    /// Emphasised text, with the large font.
    Large,
    /// Fine print, with the small font.
    Small,
    /// Secondary text: the small font in the muted foreground colour.
    Muted,
    /// Inline code: the monospace font on a muted, rounded background.
    Code,
}

impl TextRole {
    /// Every text role, in declaration order.
    pub const ALL: [TextRole; 11] = [
        TextRole::Default,
        TextRole::H1,
        TextRole::H2,
        TextRole::H3,
        TextRole::H4,
        TextRole::P,
        TextRole::Lead,
        TextRole::Large,
        TextRole::Small,
        TextRole::Muted,
        TextRole::Code,
    ];
}

impl From<TextRole> for sys::lv_theme_shadcn_text_t {
    fn from(role: TextRole) -> Self {
        match role {
            TextRole::Default => Self::LV_THEME_SHADCN_TEXT_DEFAULT,
            TextRole::H1 => Self::LV_THEME_SHADCN_TEXT_H1,
            TextRole::H2 => Self::LV_THEME_SHADCN_TEXT_H2,
            TextRole::H3 => Self::LV_THEME_SHADCN_TEXT_H3,
            TextRole::H4 => Self::LV_THEME_SHADCN_TEXT_H4,
            TextRole::P => Self::LV_THEME_SHADCN_TEXT_P,
            TextRole::Lead => Self::LV_THEME_SHADCN_TEXT_LEAD,
            TextRole::Large => Self::LV_THEME_SHADCN_TEXT_LARGE,
            TextRole::Small => Self::LV_THEME_SHADCN_TEXT_SMALL,
            TextRole::Muted => Self::LV_THEME_SHADCN_TEXT_MUTED,
            TextRole::Code => Self::LV_THEME_SHADCN_TEXT_CODE,
        }
    }
}

/// Component role of a plain object or label, mirroring shadcn/ui components that have no LVGL widget.
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq, Hash)]
pub enum Role {
    /// No role: the widget's own style.
    #[default]
    None,
    /// Pill-shaped badge filled with the primary colour, sized to its content with the small font.
    Badge,
    /// Badge filled with the secondary colour.
    BadgeSecondary,
    /// Badge with a border and a transparent background.
    BadgeOutline,
    /// Badge filled with the destructive colour.
    BadgeDestructive,
    /// Full-width bordered box for a title and a description.
    Alert,
    /// Alert with destructive text.
    AlertDestructive,
    /// Full-width 1 dp line in the border colour.
    Separator,
    /// Full-height 1 dp line in the border colour.
    SeparatorVertical,
    /// Muted placeholder block that pulses unless [`Flags::REDUCED_MOTION`] is set; size it yourself.
    Skeleton,
    /// Transparent column holding a card title and description.
    CardHeader,
    /// Card title, with the large font.
    CardTitle,
    /// Card description: the small font in the muted foreground colour.
    CardDescription,
    /// Transparent column holding the body of a card.
    CardContent,
    /// Transparent row of vertically centred card actions.
    CardFooter,
}

impl Role {
    /// Every role, in declaration order.
    pub const ALL: [Role; 15] = [
        Role::None,
        Role::Badge,
        Role::BadgeSecondary,
        Role::BadgeOutline,
        Role::BadgeDestructive,
        Role::Alert,
        Role::AlertDestructive,
        Role::Separator,
        Role::SeparatorVertical,
        Role::Skeleton,
        Role::CardHeader,
        Role::CardTitle,
        Role::CardDescription,
        Role::CardContent,
        Role::CardFooter,
    ];
}

impl From<Role> for sys::lv_theme_shadcn_role_t {
    fn from(role: Role) -> Self {
        match role {
            Role::None => Self::LV_THEME_SHADCN_ROLE_NONE,
            Role::Badge => Self::LV_THEME_SHADCN_ROLE_BADGE,
            Role::BadgeSecondary => Self::LV_THEME_SHADCN_ROLE_BADGE_SECONDARY,
            Role::BadgeOutline => Self::LV_THEME_SHADCN_ROLE_BADGE_OUTLINE,
            Role::BadgeDestructive => Self::LV_THEME_SHADCN_ROLE_BADGE_DESTRUCTIVE,
            Role::Alert => Self::LV_THEME_SHADCN_ROLE_ALERT,
            Role::AlertDestructive => Self::LV_THEME_SHADCN_ROLE_ALERT_DESTRUCTIVE,
            Role::Separator => Self::LV_THEME_SHADCN_ROLE_SEPARATOR,
            Role::SeparatorVertical => Self::LV_THEME_SHADCN_ROLE_SEPARATOR_VERTICAL,
            Role::Skeleton => Self::LV_THEME_SHADCN_ROLE_SKELETON,
            Role::CardHeader => Self::LV_THEME_SHADCN_ROLE_CARD_HEADER,
            Role::CardTitle => Self::LV_THEME_SHADCN_ROLE_CARD_TITLE,
            Role::CardDescription => Self::LV_THEME_SHADCN_ROLE_CARD_DESCRIPTION,
            Role::CardContent => Self::LV_THEME_SHADCN_ROLE_CARD_CONTENT,
            Role::CardFooter => Self::LV_THEME_SHADCN_ROLE_CARD_FOOTER,
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

    /// Replaces the corner radius scale and restyles every object on the display.
    pub fn set_radius(&self, radius: Radius) {
        unsafe { sys::lv_theme_shadcn_set_radius(self.display.as_ptr(), radius.into()) }
    }

    /// Returns the current corner radius scale.
    pub fn radius(&self) -> Radius {
        let raw = unsafe { sys::lv_theme_shadcn_get_radius(self.display.as_ptr()) };
        Radius::from_raw(raw).unwrap_or_default()
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

/// Sets the size of a control such as a button or dropdown, replacing any previous size; ignored if its display has
/// no shadcn theme.
///
/// # Safety
///
/// `object` must be a live LVGL object, used on the thread running LVGL.
pub unsafe fn set_size(object: &Object, size: Size) {
    unsafe { sys::lv_theme_shadcn_set_size(object.as_ptr(), size.into()) }
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

/// Sets the typography role of a label or container, replacing any previous text role; ignored if its display has
/// no shadcn theme.
///
/// # Safety
///
/// `object` must be a live LVGL object, used on the thread running LVGL.
pub unsafe fn set_text_role(object: &Object, role: TextRole) {
    unsafe { sys::lv_theme_shadcn_set_text_role(object.as_ptr(), role.into()) }
}

/// Sets the component role of a plain object or label, replacing any previous role; ignored if its display has no
/// shadcn theme.
///
/// A [`Role::Skeleton`] pulse animation is bound to the object and stops when the role is replaced, the object is
/// deleted or the theme is removed.
///
/// # Safety
///
/// `object` must be a live LVGL object, used on the thread running LVGL.
pub unsafe fn set_role(object: &Object, role: Role) {
    unsafe { sys::lv_theme_shadcn_set_role(object.as_ptr(), role.into()) }
}
