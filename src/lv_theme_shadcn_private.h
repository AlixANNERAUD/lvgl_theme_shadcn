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
    lv_style_t surface[LV_THEME_SHADCN_SURFACE_COUNT];
    lv_style_t container;
    lv_style_t transparent;
    lv_style_t pad_none;
    lv_style_t pad_xs;
    lv_style_t pad_sm;
    lv_style_t pad_md;
    lv_style_t pad_lg;
    lv_style_t text_muted;
    lv_style_t font_small;
    lv_style_t font_large;
    lv_style_t accent;
    lv_style_t focused_input;
    lv_style_t error;
    lv_style_t control;
    lv_style_t fill_primary;
    lv_style_t switch_track;
    lv_style_t switch_knob;
    lv_style_t switch_knob_checked;
    lv_style_t checkbox_main;
    lv_style_t checkbox_marker;
    lv_style_t checkbox_marker_checked;
    lv_style_t led;
    lv_style_t track;
    lv_style_t knob;
    lv_style_t arc_track;
    lv_style_t arc_indicator;
    lv_style_t scale_main;
    lv_style_t scale_minor;
    lv_style_t scale_ticks;
    lv_style_t input;
    lv_style_t placeholder;
    lv_style_t cursor;
    lv_style_t popover_frame;
    lv_style_t key;
    lv_style_t key_pressed;
    lv_style_t key_control;
    lv_style_t roller;
    lv_style_t list_text;
    lv_style_t list_button;
    lv_style_t table_cell;
    lv_style_t tab_bar;
    lv_style_t tab_button;
    lv_style_t tab_button_checked;
    lv_style_t calendar_day;
    lv_style_t chart;
    lv_style_t chart_series;
    lv_style_t backdrop;
    lv_style_t msgbox;
    lv_style_t msgbox_footer;
    lv_style_t variant[LV_THEME_SHADCN_VARIANT_COUNT];
    lv_style_t variant_pressed[LV_THEME_SHADCN_VARIANT_COUNT];
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
bool lv_theme_shadcn_apply_actions(lv_theme_shadcn_state_t * st, lv_obj_t * obj, lv_obj_t * parent);
void lv_theme_shadcn_add_button(lv_theme_shadcn_state_t * st, lv_obj_t * obj, lv_theme_shadcn_variant_t variant);
bool lv_theme_shadcn_apply_toggles(lv_theme_shadcn_state_t * st, lv_obj_t * obj, lv_obj_t * parent);
bool lv_theme_shadcn_apply_surfaces(lv_theme_shadcn_state_t * st, lv_obj_t * obj, lv_obj_t * parent);
bool lv_theme_shadcn_apply_ranges(lv_theme_shadcn_state_t * st, lv_obj_t * obj, lv_obj_t * parent);
bool lv_theme_shadcn_apply_collections(lv_theme_shadcn_state_t * st, lv_obj_t * obj, lv_obj_t * parent);
bool lv_theme_shadcn_apply_overlays(lv_theme_shadcn_state_t * st, lv_obj_t * obj, lv_obj_t * parent);
bool lv_theme_shadcn_apply_inputs(lv_theme_shadcn_state_t * st, lv_obj_t * obj, lv_obj_t * parent);
void lv_theme_shadcn_set_touch_target(lv_theme_shadcn_state_t * st, lv_obj_t * obj);

#endif
