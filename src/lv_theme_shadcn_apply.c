#include "lv_theme_shadcn_private.h"

void lv_theme_shadcn_add_interaction(lv_theme_shadcn_state_t * st, lv_obj_t * obj, lv_style_selector_t part)
{
    lv_obj_add_style(obj, &S(st)->transition, part);
    lv_obj_add_style(obj, &S(st)->transition, part | LV_STATE_PRESSED);
    lv_obj_add_style(obj, &S(st)->focus_ring, part | LV_STATE_FOCUS_KEY);
    lv_obj_add_style(obj, &S(st)->disabled, part | LV_STATE_DISABLED);
}

static void hover_filter_cb(lv_event_t * e)
{
    lv_indev_t * indev = lv_event_get_param(e);
    if(indev == NULL || lv_indev_get_state(indev) != LV_INDEV_STATE_PRESSED) return;
    lv_obj_t * obj = lv_event_get_current_target_obj(e);
    if(lv_theme_shadcn_state_from_obj(obj) != NULL) lv_obj_remove_state(obj, LV_STATE_HOVERED);
}

void lv_theme_shadcn_add_hover_filter(lv_obj_t * obj)
{
    lv_obj_remove_event_cb_with_user_data(obj, hover_filter_cb, NULL);
    lv_obj_add_event_cb(obj, hover_filter_cb, LV_EVENT_HOVER_OVER, NULL);
}

void lv_theme_shadcn_set_touch_target(lv_theme_shadcn_state_t * st, lv_obj_t * obj)
{
    int32_t extra = (st->metrics.touch_target - st->metrics.control_height) / 2;
    if(extra > 0) lv_obj_set_ext_click_area(obj, extra);
}

static void toggle_size_cb(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target_obj(e);
    lv_theme_shadcn_state_t * st = lv_theme_shadcn_state_from_obj(obj);
    if(st == NULL) return;
    int32_t w = lv_obj_get_width(obj);
    int32_t h = lv_obj_get_height(obj);
    int32_t smaller = w < h ? w : h;
    int32_t extra = (st->metrics.touch_target - smaller) / 2;
    lv_obj_set_ext_click_area(obj, extra > 0 ? extra : 0);
}

void lv_theme_shadcn_set_toggle_touch_target(lv_obj_t * obj)
{
    lv_obj_remove_event_cb_with_user_data(obj, toggle_size_cb, NULL);
    lv_obj_add_event_cb(obj, toggle_size_cb, LV_EVENT_SIZE_CHANGED, NULL);
}

static void apply_screen(lv_theme_shadcn_state_t * st, lv_obj_t * obj)
{
    lv_obj_add_style(obj, &S(st)->screen, 0);
    lv_obj_add_style(obj, &S(st)->scrollbar, LV_PART_SCROLLBAR);
    lv_obj_add_style(obj, &S(st)->scrollbar_scrolled, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
}

void lv_theme_shadcn_apply_cb(lv_theme_t * th, lv_obj_t * obj)
{
    lv_theme_shadcn_state_t * st = lv_theme_get_user_data(th);
    if(st == NULL) return;
    lv_obj_t * parent = lv_obj_get_parent(obj);
    if(parent == NULL) {
        apply_screen(st, obj);
        return;
    }
    if(lv_theme_shadcn_apply_overlays(st, obj, parent)) return;
    if(lv_theme_shadcn_apply_collections(st, obj, parent)) return;
    if(lv_theme_shadcn_apply_inputs(st, obj, parent)) return;
    if(lv_theme_shadcn_apply_ranges(st, obj, parent)) return;
    if(lv_theme_shadcn_apply_toggles(st, obj, parent)) return;
    if(lv_theme_shadcn_apply_actions(st, obj, parent)) return;
    if(lv_theme_shadcn_apply_surfaces(st, obj, parent)) return;
}
