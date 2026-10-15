#include "../lv_theme_shadcn_private.h"

#if LV_USE_LED
static void led_initial_color_cb(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target_obj(e);
    lv_obj_remove_event_cb(obj, led_initial_color_cb);
    lv_theme_shadcn_state_t * st = lv_theme_shadcn_state_from_obj(obj);
    if(st != NULL) lv_led_set_color(obj, st->palette.primary);
}
#endif

bool lv_theme_shadcn_apply_toggles(lv_theme_shadcn_state_t * st, lv_obj_t * obj, lv_obj_t * parent)
{
    LV_UNUSED(st);
    LV_UNUSED(obj);
    LV_UNUSED(parent);
#if LV_USE_SWITCH
    if(lv_obj_check_type(obj, &lv_switch_class)) {
        lv_obj_add_style(obj, &S(st)->switch_track, 0);
        lv_obj_add_style(obj, &S(st)->fill_primary, LV_PART_INDICATOR | LV_STATE_CHECKED);
        lv_obj_add_style(obj, &S(st)->switch_knob, LV_PART_KNOB);
        lv_obj_add_style(obj, &S(st)->switch_knob_checked, LV_PART_KNOB | LV_STATE_CHECKED);
        lv_obj_add_style(obj, &S(st)->switch_track_hovered, LV_STATE_HOVERED);
        lv_obj_add_style(obj, &S(st)->fill_primary_hovered, LV_PART_INDICATOR | LV_STATE_CHECKED | LV_STATE_HOVERED);
        lv_theme_shadcn_add_interaction(st, obj, LV_PART_MAIN);
        lv_theme_shadcn_add_hover_filter(obj);
        lv_theme_shadcn_set_toggle_touch_target(obj);
        return true;
    }
#endif
#if LV_USE_CHECKBOX
    if(lv_obj_check_type(obj, &lv_checkbox_class)) {
        lv_obj_add_style(obj, &S(st)->checkbox_main, 0);
        lv_obj_add_style(obj, &S(st)->checkbox_marker, LV_PART_INDICATOR);
        lv_obj_add_style(obj, &S(st)->checkbox_marker_checked, LV_PART_INDICATOR | LV_STATE_CHECKED);
        lv_obj_add_style(obj, &S(st)->checkbox_marker_hovered, LV_PART_INDICATOR | LV_STATE_HOVERED);
        lv_obj_add_style(obj, &S(st)->fill_primary_hovered, LV_PART_INDICATOR | LV_STATE_CHECKED | LV_STATE_HOVERED);
        lv_theme_shadcn_add_interaction(st, obj, LV_PART_MAIN);
        lv_theme_shadcn_add_hover_filter(obj);
        lv_theme_shadcn_set_toggle_touch_target(obj);
        return true;
    }
#endif
#if LV_USE_LED
    if(lv_obj_check_type(obj, &lv_led_class)) {
        lv_obj_add_style(obj, &S(st)->led, 0);
        lv_obj_add_event_cb(obj, led_initial_color_cb, LV_EVENT_STYLE_CHANGED, NULL);
        return true;
    }
#endif
    return false;
}
