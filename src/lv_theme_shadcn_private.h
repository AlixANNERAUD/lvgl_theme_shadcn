#ifndef LV_THEME_SHADCN_PRIVATE_H
#define LV_THEME_SHADCN_PRIVATE_H

#include "lv_theme_shadcn.h"

#define LV_THEME_SHADCN_VARIANT_COUNT 5
#define LV_THEME_SHADCN_SURFACE_COUNT 4
#define LV_THEME_SHADCN_PRESS_MIX 26
#define LV_THEME_SHADCN_DISABLED_OPA 97
#define LV_THEME_SHADCN_TRANSITION_MS 150

typedef struct {
    int32_t space_xs;
    int32_t space_sm;
    int32_t space_md;
    int32_t space_lg;
    int32_t space_xl;
    int32_t radius_sm;
    int32_t radius_md;
    int32_t radius_lg;
    int32_t border_width;
    int32_t ring_width;
    int32_t ring_gap;
    int32_t control_height;
    int32_t touch_target;
    int32_t scrollbar_width;
} lv_theme_shadcn_metrics_t;

typedef struct {
    lv_style_t screen;
    lv_style_t scrollbar;
    lv_style_t scrollbar_scrolled;
    lv_style_t transition;
    lv_style_t focus_ring;
    lv_style_t disabled;
} lv_theme_shadcn_styles_t;

typedef struct lv_theme_shadcn_state_t {
    struct lv_theme_shadcn_state_t * next;
    lv_display_t * disp;
    lv_theme_t * theme;
    lv_theme_t * previous_theme;
    lv_theme_shadcn_palette_t palette;
    lv_theme_shadcn_fonts_t fonts;
    lv_theme_shadcn_flags_t flags;
    lv_theme_shadcn_metrics_t metrics;
    lv_style_transition_dsc_t transition_dsc;
    bool styles_ready;
    lv_theme_shadcn_styles_t styles;
} lv_theme_shadcn_state_t;

#define S(st) (&(st)->styles)

lv_theme_shadcn_state_t * lv_theme_shadcn_state_find(const lv_display_t * disp);
lv_theme_shadcn_state_t * lv_theme_shadcn_state_from_obj(const lv_obj_t * obj);
void lv_theme_shadcn_metrics_resolve(lv_theme_shadcn_state_t * st);
void lv_theme_shadcn_styles_build(lv_theme_shadcn_state_t * st);
void lv_theme_shadcn_styles_reset(lv_theme_shadcn_state_t * st);
void lv_theme_shadcn_apply_cb(lv_theme_t * th, lv_obj_t * obj);
void lv_theme_shadcn_add_interaction(lv_theme_shadcn_state_t * st, lv_obj_t * obj, lv_style_selector_t part);
void lv_theme_shadcn_set_touch_target(lv_theme_shadcn_state_t * st, lv_obj_t * obj);

#endif
