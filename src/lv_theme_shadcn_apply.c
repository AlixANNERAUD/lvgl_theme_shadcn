#include "lv_theme_shadcn_private.h"

void lv_theme_shadcn_add_interaction(lv_theme_shadcn_state_t * st, lv_obj_t * obj, lv_style_selector_t part)
{
    lv_obj_add_style(obj, &S(st)->transition, part);
    lv_obj_add_style(obj, &S(st)->transition, part | LV_STATE_PRESSED);
    lv_obj_add_style(obj, &S(st)->focus_ring, part | LV_STATE_FOCUS_KEY);
    lv_obj_add_style(obj, &S(st)->disabled, part | LV_STATE_DISABLED);
}

void lv_theme_shadcn_set_touch_target(lv_theme_shadcn_state_t * st, lv_obj_t * obj)
{
    int32_t extra = (st->metrics.touch_target - st->metrics.control_height) / 2;
    if(extra > 0) lv_obj_set_ext_click_area(obj, extra);
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
    if(lv_theme_shadcn_apply_collections(st, obj, parent)) return;
    if(lv_theme_shadcn_apply_inputs(st, obj, parent)) return;
    if(lv_theme_shadcn_apply_ranges(st, obj, parent)) return;
    if(lv_theme_shadcn_apply_toggles(st, obj, parent)) return;
    if(lv_theme_shadcn_apply_actions(st, obj, parent)) return;
    if(lv_theme_shadcn_apply_surfaces(st, obj, parent)) return;
}
