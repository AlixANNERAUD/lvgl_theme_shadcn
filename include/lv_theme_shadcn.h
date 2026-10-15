/**
 * @file lv_theme_shadcn.h
 * @brief shadcn/ui-inspired theme for LVGL 9.6 or newer.
 *
 * Call lv_theme_shadcn_init() before creating widgets to style every core LVGL widget with shadcn/ui's colour roles,
 * radii and border-based surfaces. Palettes and the corner radius can be changed at runtime and restyle existing
 * widgets in place. The per-object helpers (variants, sizes, surfaces, error state, text and component roles) only
 * take effect on displays where the shadcn theme is active.
 *
 * Functions taking an @c lv_display_t use the default display when passed NULL.
 */

#ifndef LV_THEME_SHADCN_H
#define LV_THEME_SHADCN_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(LV_THEME_SHADCN_LVGL_H_INCLUDE)
#include LV_THEME_SHADCN_LVGL_H_INCLUDE
#elif defined(__has_include)
#if __has_include(<lvgl/lvgl.h>)
#include <lvgl/lvgl.h>
#else
#include "lvgl.h"
#endif
#else
#include "lvgl.h"
#endif

#if LVGL_VERSION_MAJOR != 9 || LVGL_VERSION_MINOR < 6
#error "lv_theme_shadcn requires LVGL 9.6 or newer"
#endif

/**
 * @brief The colour roles of a theme, mirroring shadcn/ui CSS variables.
 *
 * Build one with lv_theme_shadcn_palette_make(), start from a built-in palette such as
 * lv_theme_shadcn_palette_zinc_dark(), or fill every field yourself.
 */
typedef struct {
    /** Page background. */
    lv_color_t background;
    /** Text on @ref background. */
    lv_color_t foreground;
    /** Card surface. */
    lv_color_t card;
    /** Text on @ref card. */
    lv_color_t card_fg;
    /** Popover surface: dropdown lists, message boxes and other floating elements. */
    lv_color_t popover;
    /** Text on @ref popover. */
    lv_color_t popover_fg;
    /** Main action colour: default buttons, checked controls and indicators. */
    lv_color_t primary;
    /** Text and icons on @ref primary. */
    lv_color_t primary_fg;
    /** Secondary action colour. */
    lv_color_t secondary;
    /** Text and icons on @ref secondary. */
    lv_color_t secondary_fg;
    /** Muted surface and tracks. */
    lv_color_t muted;
    /** Secondary text such as descriptions and placeholders. */
    lv_color_t muted_fg;
    /** Highlight surface, for example a ghost button when pressed. */
    lv_color_t accent;
    /** Text on @ref accent. */
    lv_color_t accent_fg;
    /** Destructive actions and errors. */
    lv_color_t destructive;
    /** Text and icons on @ref destructive. */
    lv_color_t destructive_fg;
    /** Borders of surfaces and separators. */
    lv_color_t border;
    /** Borders of input controls. */
    lv_color_t input;
    /** Focus ring. */
    lv_color_t ring;
    /** Chart series colours. */
    lv_color_t chart[5];
    /** Whether the palette is dark. */
    bool dark;
} lv_theme_shadcn_palette_t;

/**
 * @brief Base colour of a shadcn/ui palette: the neutral tones used for backgrounds, surfaces, borders and text.
 *
 * Gray and slate come from shadcn/ui's legacy base colours, the others from its current registry.
 */
typedef enum {
    /** shadcn/ui @c neutral. */
    LV_THEME_SHADCN_BASE_NEUTRAL,
    /** shadcn/ui @c stone. */
    LV_THEME_SHADCN_BASE_STONE,
    /** shadcn/ui @c zinc, the default. */
    LV_THEME_SHADCN_BASE_ZINC,
    /** shadcn/ui @c gray. */
    LV_THEME_SHADCN_BASE_GRAY,
    /** shadcn/ui @c slate. */
    LV_THEME_SHADCN_BASE_SLATE,
    /** shadcn/ui @c mauve. */
    LV_THEME_SHADCN_BASE_MAUVE,
    /** shadcn/ui @c olive. */
    LV_THEME_SHADCN_BASE_OLIVE,
    /** shadcn/ui @c mist. */
    LV_THEME_SHADCN_BASE_MIST,
    /** shadcn/ui @c taupe. */
    LV_THEME_SHADCN_BASE_TAUPE,
    /** Number of base colours. */
    LV_THEME_SHADCN_BASE_COUNT,
} lv_theme_shadcn_base_t;

/**
 * @brief Accent colour of a shadcn/ui palette.
 *
 * An accent overrides the @c primary, @c primary_fg, @c secondary, @c secondary_fg and chart colours of the base.
 */
typedef enum {
    /** No accent: the base colour's own monochrome primary and charts. */
    LV_THEME_SHADCN_ACCENT_NONE,
    /** shadcn/ui @c amber. */
    LV_THEME_SHADCN_ACCENT_AMBER,
    /** shadcn/ui @c blue. */
    LV_THEME_SHADCN_ACCENT_BLUE,
    /** shadcn/ui @c cyan. */
    LV_THEME_SHADCN_ACCENT_CYAN,
    /** shadcn/ui @c emerald. */
    LV_THEME_SHADCN_ACCENT_EMERALD,
    /** shadcn/ui @c fuchsia. */
    LV_THEME_SHADCN_ACCENT_FUCHSIA,
    /** shadcn/ui @c green. */
    LV_THEME_SHADCN_ACCENT_GREEN,
    /** shadcn/ui @c indigo. */
    LV_THEME_SHADCN_ACCENT_INDIGO,
    /** shadcn/ui @c lime. */
    LV_THEME_SHADCN_ACCENT_LIME,
    /** shadcn/ui @c orange. */
    LV_THEME_SHADCN_ACCENT_ORANGE,
    /** shadcn/ui @c pink. */
    LV_THEME_SHADCN_ACCENT_PINK,
    /** shadcn/ui @c purple. */
    LV_THEME_SHADCN_ACCENT_PURPLE,
    /** shadcn/ui @c red. */
    LV_THEME_SHADCN_ACCENT_RED,
    /** shadcn/ui @c rose. */
    LV_THEME_SHADCN_ACCENT_ROSE,
    /** shadcn/ui @c sky. */
    LV_THEME_SHADCN_ACCENT_SKY,
    /** shadcn/ui @c teal. */
    LV_THEME_SHADCN_ACCENT_TEAL,
    /** shadcn/ui @c violet. */
    LV_THEME_SHADCN_ACCENT_VIOLET,
    /** shadcn/ui @c yellow. */
    LV_THEME_SHADCN_ACCENT_YELLOW,
    /** Number of accents, including ::LV_THEME_SHADCN_ACCENT_NONE. */
    LV_THEME_SHADCN_ACCENT_COUNT,
} lv_theme_shadcn_accent_t;

/**
 * @brief Fonts used by the theme.
 *
 * A NULL font falls back to @ref normal, and a NULL @ref normal falls back to @c LV_FONT_DEFAULT. Heading fonts fall
 * back down the chain @ref h1, @ref h2, @ref h3, @ref large, then @ref normal, so setting only @ref h3 gives every
 * heading that font. Fonts must outlive the theme.
 */
typedef struct {
    /** Captions and secondary text. */
    const lv_font_t * small;
    /** Body text. */
    const lv_font_t * normal;
    /** Titles. */
    const lv_font_t * large;
    /** Monospace text, used by ::LV_THEME_SHADCN_TEXT_CODE. */
    const lv_font_t * mono;
    /** ::LV_THEME_SHADCN_TEXT_H1 headings. */
    const lv_font_t * h1;
    /** ::LV_THEME_SHADCN_TEXT_H2 headings. */
    const lv_font_t * h2;
    /** ::LV_THEME_SHADCN_TEXT_H3 headings. */
    const lv_font_t * h3;
} lv_theme_shadcn_fonts_t;

/**
 * @brief Theme initialisation flags, combined with a bitwise OR.
 */
typedef enum {
    /** No flags. */
    LV_THEME_SHADCN_FLAG_NONE = 0,
    /** Smaller paddings and sizes for small displays. */
    LV_THEME_SHADCN_FLAG_COMPACT = 1 << 0,
    /** Disables decorative animations such as the ::LV_THEME_SHADCN_ROLE_SKELETON pulse. */
    LV_THEME_SHADCN_FLAG_REDUCED_MOTION = 1 << 1,
} lv_theme_shadcn_flags_t;

/**
 * @brief Visual variant of an action widget such as a button.
 */
typedef enum {
    /** Filled with the primary colour. */
    LV_THEME_SHADCN_VARIANT_DEFAULT,
    /** Filled with the secondary colour. */
    LV_THEME_SHADCN_VARIANT_SECONDARY,
    /** Bordered with a transparent background. */
    LV_THEME_SHADCN_VARIANT_OUTLINE,
    /** Transparent background, accent colour when pressed. */
    LV_THEME_SHADCN_VARIANT_GHOST,
    /** Filled with the destructive colour. */
    LV_THEME_SHADCN_VARIANT_DESTRUCTIVE,
} lv_theme_shadcn_variant_t;

/**
 * @brief Background surface of a container.
 */
typedef enum {
    /** Page background. */
    LV_THEME_SHADCN_SURFACE_BACKGROUND,
    /** Card surface. */
    LV_THEME_SHADCN_SURFACE_CARD,
    /** Popover surface. */
    LV_THEME_SHADCN_SURFACE_POPOVER,
    /** Muted surface. */
    LV_THEME_SHADCN_SURFACE_MUTED,
} lv_theme_shadcn_surface_t;

/**
 * @brief Corner radius scale, mirroring shadcn/ui's @c --radius options.
 *
 * With @c r the selected radius, widgets use shadcn/ui's derived radii: small elements such as checkboxes @c 0.6r,
 * controls such as buttons and inputs @c 0.8r, dialogs @c r and cards @c 1.4r. One rem is 16 dp.
 */
typedef enum {
    /** Square corners. */
    LV_THEME_SHADCN_RADIUS_NONE,
    /** 0.3 rem. */
    LV_THEME_SHADCN_RADIUS_SM,
    /** 0.5 rem. */
    LV_THEME_SHADCN_RADIUS_MD,
    /** 0.625 rem, shadcn/ui's default. */
    LV_THEME_SHADCN_RADIUS_DEFAULT,
    /** 0.75 rem. */
    LV_THEME_SHADCN_RADIUS_LG,
    /** 1 rem. */
    LV_THEME_SHADCN_RADIUS_XL,
} lv_theme_shadcn_radius_t;

/**
 * @brief Size of a control such as a button, mirroring shadcn/ui's button sizes.
 *
 * Heights are 32, 40 and 48 dp for small, default and large, 8 dp less with ::LV_THEME_SHADCN_FLAG_COMPACT. Small
 * sizes use the small font, and icon sizes are square. Controls smaller than the touch target get an extended click
 * area.
 */
typedef enum {
    /** The theme's default control height. */
    LV_THEME_SHADCN_SIZE_DEFAULT,
    /** Shorter, with tighter padding and the small font. */
    LV_THEME_SHADCN_SIZE_SM,
    /** Taller, with wider padding. */
    LV_THEME_SHADCN_SIZE_LG,
    /** Square, at the default height. */
    LV_THEME_SHADCN_SIZE_ICON,
    /** Square, at the small height. */
    LV_THEME_SHADCN_SIZE_ICON_SM,
    /** Square, at the large height. */
    LV_THEME_SHADCN_SIZE_ICON_LG,
} lv_theme_shadcn_size_t;

/**
 * @brief Typography role of a label, mirroring shadcn/ui's typography.
 *
 * Roles set the font and, for some, the colour and spacing. Text properties are inherited, so a role set on a
 * container applies to the labels inside it.
 */
typedef enum {
    /** The inherited body text. */
    LV_THEME_SHADCN_TEXT_DEFAULT,
    /** Page title, with lv_theme_shadcn_fonts_t::h1. */
    LV_THEME_SHADCN_TEXT_H1,
    /** Section title, with lv_theme_shadcn_fonts_t::h2. */
    LV_THEME_SHADCN_TEXT_H2,
    /** Subsection title, with lv_theme_shadcn_fonts_t::h3. */
    LV_THEME_SHADCN_TEXT_H3,
    /** Minor title, with lv_theme_shadcn_fonts_t::large. */
    LV_THEME_SHADCN_TEXT_H4,
    /** Paragraph: the normal font with relaxed line spacing. */
    LV_THEME_SHADCN_TEXT_P,
    /** Introductory text: the large font in the muted foreground colour. */
    LV_THEME_SHADCN_TEXT_LEAD,
    /** Emphasised text, with the large font. */
    LV_THEME_SHADCN_TEXT_LARGE,
    /** Fine print, with the small font. */
    LV_THEME_SHADCN_TEXT_SMALL,
    /** Secondary text: the small font in the muted foreground colour. */
    LV_THEME_SHADCN_TEXT_MUTED,
    /** Inline code: the monospace font on a muted, rounded background. */
    LV_THEME_SHADCN_TEXT_CODE,
} lv_theme_shadcn_text_t;

/**
 * @brief Component role of a plain object or label, mirroring shadcn/ui components that have no LVGL widget.
 */
typedef enum {
    /** No role: the widget's own style. */
    LV_THEME_SHADCN_ROLE_NONE,
    /** Pill-shaped badge filled with the primary colour, sized to its content with the small font. */
    LV_THEME_SHADCN_ROLE_BADGE,
    /** Badge filled with the secondary colour. */
    LV_THEME_SHADCN_ROLE_BADGE_SECONDARY,
    /** Badge with a border and a transparent background. */
    LV_THEME_SHADCN_ROLE_BADGE_OUTLINE,
    /** Badge filled with the destructive colour. */
    LV_THEME_SHADCN_ROLE_BADGE_DESTRUCTIVE,
    /** Full-width bordered box for a title and a description. */
    LV_THEME_SHADCN_ROLE_ALERT,
    /** Alert with destructive text. */
    LV_THEME_SHADCN_ROLE_ALERT_DESTRUCTIVE,
    /** Full-width 1 dp line in the border colour. */
    LV_THEME_SHADCN_ROLE_SEPARATOR,
    /** Full-height 1 dp line in the border colour. */
    LV_THEME_SHADCN_ROLE_SEPARATOR_VERTICAL,
    /** Muted placeholder block that pulses unless ::LV_THEME_SHADCN_FLAG_REDUCED_MOTION is set; size it yourself. */
    LV_THEME_SHADCN_ROLE_SKELETON,
    /** Transparent column holding a card title and description. */
    LV_THEME_SHADCN_ROLE_CARD_HEADER,
    /** Card title, with the large font. */
    LV_THEME_SHADCN_ROLE_CARD_TITLE,
    /** Card description: the small font in the muted foreground colour. */
    LV_THEME_SHADCN_ROLE_CARD_DESCRIPTION,
    /** Transparent column holding the body of a card. */
    LV_THEME_SHADCN_ROLE_CARD_CONTENT,
    /** Transparent row of vertically centred card actions. */
    LV_THEME_SHADCN_ROLE_CARD_FOOTER,
} lv_theme_shadcn_role_t;

/**
 * @brief Applies the shadcn theme to a display.
 *
 * Call it after @c lv_init() and before creating widgets. Initialising a display that already has a shadcn theme
 * updates it in place and keeps its radius. The palette and fonts are copied, but the font objects themselves must
 * outlive the theme.
 *
 * @param disp Display to theme, or NULL for the default display.
 * @param palette Colours to use, for example lv_theme_shadcn_palette_zinc_dark().
 * @param fonts Fonts to use, or NULL to use @c LV_FONT_DEFAULT everywhere.
 * @param flags Combination of ::lv_theme_shadcn_flags_t values.
 * @return The LVGL theme, or NULL when there is no display, @p palette is NULL or allocation fails.
 */
lv_theme_t * lv_theme_shadcn_init(lv_display_t * disp, const lv_theme_shadcn_palette_t * palette,
                                  const lv_theme_shadcn_fonts_t * fonts, lv_theme_shadcn_flags_t flags);

/**
 * @brief Removes the shadcn theme from a display and restores the theme it had before lv_theme_shadcn_init().
 *
 * Does nothing when the display has no shadcn theme.
 *
 * @param disp Themed display, or NULL for the default display.
 */
void lv_theme_shadcn_deinit(lv_display_t * disp);

/**
 * @brief Whether the shadcn theme is the active theme of a display.
 *
 * @param disp Display to check, or NULL for the default display.
 * @return true when the display uses the shadcn theme.
 */
bool lv_theme_shadcn_is_active(lv_display_t * disp);

/**
 * @brief Replaces the palette and restyles every object on the display.
 *
 * Does nothing when the display has no shadcn theme or @p palette is NULL. LED colours are object properties set at
 * creation and are not updated.
 *
 * @param disp Themed display, or NULL for the default display.
 * @param palette New colours, copied by the theme.
 */
void lv_theme_shadcn_set_palette(lv_display_t * disp, const lv_theme_shadcn_palette_t * palette);

/**
 * @brief Returns the current palette of a display.
 *
 * @param disp Themed display, or NULL for the default display.
 * @return The palette owned by the theme, valid until the theme is removed, or NULL when the display has no shadcn
 *         theme.
 */
const lv_theme_shadcn_palette_t * lv_theme_shadcn_get_palette(lv_display_t * disp);

/**
 * @brief Replaces the corner radius scale and restyles every object on the display.
 *
 * Does nothing when the display has no shadcn theme or @p radius is out of range.
 *
 * @param disp Themed display, or NULL for the default display.
 * @param radius New radius scale.
 */
void lv_theme_shadcn_set_radius(lv_display_t * disp, lv_theme_shadcn_radius_t radius);

/**
 * @brief Returns the current corner radius scale of a display.
 *
 * @param disp Themed display, or NULL for the default display.
 * @return The radius scale, or ::LV_THEME_SHADCN_RADIUS_DEFAULT when the display has no shadcn theme.
 */
lv_theme_shadcn_radius_t lv_theme_shadcn_get_radius(lv_display_t * disp);

/**
 * @brief The built-in dark zinc palette.
 *
 * @return A palette stored in flash, valid for the lifetime of the program.
 */
const lv_theme_shadcn_palette_t * lv_theme_shadcn_palette_zinc_dark(void);

/**
 * @brief The built-in light zinc palette.
 *
 * @return A palette stored in flash, valid for the lifetime of the program.
 */
const lv_theme_shadcn_palette_t * lv_theme_shadcn_palette_zinc_light(void);

/**
 * @brief Derives a branded palette from a base palette and a primary colour.
 *
 * Copies @p base, replaces @c primary and @c primary_fg, and sets @c ring to @p primary mixed half and half with the
 * background.
 *
 * @param out Palette to fill. It may be the same as @p base. Must not be NULL.
 * @param base Palette to start from. Must not be NULL.
 * @param primary New primary colour.
 * @param primary_fg Text and icon colour on @p primary.
 */
void lv_theme_shadcn_palette_from_primary(lv_theme_shadcn_palette_t * out, const lv_theme_shadcn_palette_t * base,
                                          lv_color_t primary, lv_color_t primary_fg);

/**
 * @brief Builds the shadcn/ui palette for a base colour and an accent, in dark or light mode.
 *
 * The tables are generated from the shadcn/ui registry and stored in flash; nothing is allocated. An out of range
 * @p base falls back to ::LV_THEME_SHADCN_BASE_ZINC, and an out of range @p accent to ::LV_THEME_SHADCN_ACCENT_NONE.
 * Does nothing when @p out is NULL.
 *
 * @code
 * lv_theme_shadcn_palette_t palette;
 * lv_theme_shadcn_palette_make(&palette, LV_THEME_SHADCN_BASE_SLATE, LV_THEME_SHADCN_ACCENT_BLUE, true);
 * lv_theme_shadcn_set_palette(NULL, &palette);
 * @endcode
 *
 * @param out Palette to fill.
 * @param base Base colour.
 * @param accent Accent colour, or ::LV_THEME_SHADCN_ACCENT_NONE.
 * @param dark true for the dark palette, false for the light one.
 */
void lv_theme_shadcn_palette_make(lv_theme_shadcn_palette_t * out, lv_theme_shadcn_base_t base,
                                  lv_theme_shadcn_accent_t accent, bool dark);

/**
 * @brief Returns the lowercase shadcn/ui name of a base colour, such as @c "zinc".
 *
 * @param base Base colour. Out of range values return the name of ::LV_THEME_SHADCN_BASE_ZINC.
 * @return A static string.
 */
const char * lv_theme_shadcn_base_name(lv_theme_shadcn_base_t base);

/**
 * @brief Returns the lowercase shadcn/ui name of an accent, such as @c "blue", or @c "none".
 *
 * @param accent Accent colour. Out of range values return the name of ::LV_THEME_SHADCN_ACCENT_NONE.
 * @return A static string.
 */
const char * lv_theme_shadcn_accent_name(lv_theme_shadcn_accent_t accent);

/**
 * @brief Sets the visual variant of an action widget such as a button.
 *
 * Replaces any previous variant. Ignored when @p obj is NULL, its display has no active shadcn theme or @p variant is
 * out of range.
 *
 * @param obj Widget to style.
 * @param variant New variant.
 */
void lv_theme_shadcn_set_variant(lv_obj_t * obj, lv_theme_shadcn_variant_t variant);

/**
 * @brief Sets the size of a control such as a button or dropdown.
 *
 * Replaces any previous size and extends the click area of controls smaller than the touch target. Ignored when
 * @p obj is NULL, its display has no active shadcn theme or @p size is out of range.
 *
 * @param obj Control to resize.
 * @param size New size.
 */
void lv_theme_shadcn_set_size(lv_obj_t * obj, lv_theme_shadcn_size_t size);

/**
 * @brief Sets the background surface of a container.
 *
 * Replaces any previous surface. Ignored when @p obj is NULL, its display has no active shadcn theme or @p surface is
 * out of range.
 *
 * @param obj Container to style.
 * @param surface New surface.
 */
void lv_theme_shadcn_set_surface(lv_obj_t * obj, lv_theme_shadcn_surface_t surface);

/**
 * @brief Marks an input as invalid or valid.
 *
 * An invalid input gets a destructive border and text colour, also when focused or checked. Ignored when @p obj is
 * NULL or its display has no active shadcn theme.
 *
 * @param obj Input widget such as a text area or dropdown.
 * @param error true to mark the input invalid, false to clear the error.
 */
void lv_theme_shadcn_set_error(lv_obj_t * obj, bool error);

/**
 * @brief Sets the typography role of a label or container.
 *
 * Replaces any previous text role. Ignored when @p obj is NULL, its display has no active shadcn theme or @p role is
 * out of range.
 *
 * @param obj Label, or container whose labels inherit the role.
 * @param role New text role.
 */
void lv_theme_shadcn_set_text_role(lv_obj_t * obj, lv_theme_shadcn_text_t role);

/**
 * @brief Sets the component role of a plain object or label.
 *
 * Replaces any previous role. A ::LV_THEME_SHADCN_ROLE_SKELETON pulse animation is bound to the object and stops
 * when the role is replaced, the object is deleted or the theme is removed. Ignored when @p obj is NULL, its display
 * has no active shadcn theme or @p role is out of range.
 *
 * @param obj Object or label to style.
 * @param role New role, or ::LV_THEME_SHADCN_ROLE_NONE to remove it.
 */
void lv_theme_shadcn_set_role(lv_obj_t * obj, lv_theme_shadcn_role_t role);

#ifdef __cplusplus
}
#endif

#endif
