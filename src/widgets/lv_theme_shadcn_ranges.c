#include "../lv_theme_shadcn_private.h"

bool lv_theme_shadcn_apply_ranges(lv_theme_shadcn_state_t * st, lv_obj_t * obj, lv_obj_t * parent)
{
    LV_UNUSED(st);
    LV_UNUSED(obj);
    LV_UNUSED(parent);
#if LV_USE_SLIDER
    if(lv_obj_check_type(obj, &lv_slider_class)) {
        lv_obj_add_style(obj, &S(st)->track, 0);
        lv_obj_add_style(obj, &S(st)->fill_primary, LV_PART_INDICATOR);
        lv_obj_add_style(obj, &S(st)->knob, LV_PART_KNOB);
        lv_theme_shadcn_add_interaction(st, obj, LV_PART_MAIN);
        return true;
    }
#endif
#if LV_USE_BAR
    if(lv_obj_check_type(obj, &lv_bar_class)) {
        lv_obj_add_style(obj, &S(st)->track, 0);
        lv_obj_add_style(obj, &S(st)->fill_primary, LV_PART_INDICATOR);
        lv_obj_add_style(obj, &S(st)->disabled, LV_STATE_DISABLED);
        return true;
    }
#endif
#if LV_USE_ARC
    if(lv_obj_check_type(obj, &lv_arc_class)) {
        lv_obj_add_style(obj, &S(st)->arc_track, 0);
        lv_obj_add_style(obj, &S(st)->arc_indicator, LV_PART_INDICATOR);
        lv_obj_add_style(obj, &S(st)->knob, LV_PART_KNOB);
        lv_theme_shadcn_add_interaction(st, obj, LV_PART_MAIN);
        return true;
    }
#endif
#if LV_USE_SPINNER
    if(lv_obj_check_type(obj, &lv_spinner_class)) {
        lv_obj_add_style(obj, &S(st)->arc_track, 0);
        lv_obj_add_style(obj, &S(st)->arc_indicator, LV_PART_INDICATOR);
        return true;
    }
#endif
#if LV_USE_SCALE
    if(lv_obj_check_type(obj, &lv_scale_class)) {
        lv_obj_add_style(obj, &S(st)->scale_main, 0);
        lv_obj_add_style(obj, &S(st)->scale_minor, LV_PART_ITEMS);
        lv_obj_add_style(obj, &S(st)->scale_ticks, LV_PART_INDICATOR);
        return true;
    }
#endif
#if LV_USE_LINE
    if(lv_obj_check_type(obj, &lv_line_class)) {
        lv_obj_add_style(obj, &S(st)->line, 0);
        return true;
    }
#endif
    return false;
}
