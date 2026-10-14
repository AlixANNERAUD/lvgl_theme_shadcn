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

typedef struct {
    lv_color_t background;
    lv_color_t foreground;
    lv_color_t card;
    lv_color_t card_fg;
    lv_color_t popover;
    lv_color_t popover_fg;
    lv_color_t primary;
    lv_color_t primary_fg;
    lv_color_t secondary;
    lv_color_t secondary_fg;
    lv_color_t muted;
    lv_color_t muted_fg;
    lv_color_t accent;
    lv_color_t accent_fg;
    lv_color_t destructive;
    lv_color_t destructive_fg;
    lv_color_t border;
    lv_color_t input;
    lv_color_t ring;
    lv_color_t chart[5];
    bool dark;
} lv_theme_shadcn_palette_t;

typedef struct {
    const lv_font_t * small;
    const lv_font_t * normal;
    const lv_font_t * large;
    const lv_font_t * mono;
} lv_theme_shadcn_fonts_t;

typedef enum {
    LV_THEME_SHADCN_FLAG_NONE = 0,
    LV_THEME_SHADCN_FLAG_COMPACT = 1 << 0,
} lv_theme_shadcn_flags_t;

typedef enum {
    LV_THEME_SHADCN_VARIANT_DEFAULT,
    LV_THEME_SHADCN_VARIANT_SECONDARY,
    LV_THEME_SHADCN_VARIANT_OUTLINE,
    LV_THEME_SHADCN_VARIANT_GHOST,
    LV_THEME_SHADCN_VARIANT_DESTRUCTIVE,
} lv_theme_shadcn_variant_t;

typedef enum {
    LV_THEME_SHADCN_SURFACE_BACKGROUND,
    LV_THEME_SHADCN_SURFACE_CARD,
    LV_THEME_SHADCN_SURFACE_POPOVER,
    LV_THEME_SHADCN_SURFACE_MUTED,
} lv_theme_shadcn_surface_t;

lv_theme_t * lv_theme_shadcn_init(lv_display_t * disp, const lv_theme_shadcn_palette_t * palette,
                                  const lv_theme_shadcn_fonts_t * fonts, lv_theme_shadcn_flags_t flags);
void lv_theme_shadcn_deinit(lv_display_t * disp);
bool lv_theme_shadcn_is_active(lv_display_t * disp);
void lv_theme_shadcn_set_palette(lv_display_t * disp, const lv_theme_shadcn_palette_t * palette);
const lv_theme_shadcn_palette_t * lv_theme_shadcn_get_palette(lv_display_t * disp);

const lv_theme_shadcn_palette_t * lv_theme_shadcn_palette_zinc_dark(void);
const lv_theme_shadcn_palette_t * lv_theme_shadcn_palette_zinc_light(void);
void lv_theme_shadcn_palette_from_primary(lv_theme_shadcn_palette_t * out, const lv_theme_shadcn_palette_t * base,
                                          lv_color_t primary, lv_color_t primary_fg);

void lv_theme_shadcn_set_variant(lv_obj_t * obj, lv_theme_shadcn_variant_t variant);
void lv_theme_shadcn_set_surface(lv_obj_t * obj, lv_theme_shadcn_surface_t surface);
void lv_theme_shadcn_set_error(lv_obj_t * obj, bool error);

#ifdef __cplusplus
}
#endif

#endif
