#include "../lv_theme_shadcn_private.h"

#if LV_USE_TEXTAREA || LV_USE_SPINBOX || LV_USE_DROPDOWN || LV_USE_ROLLER
static void add_input(lv_theme_shadcn_state_t * st, lv_obj_t * obj)
{
    lv_obj_add_style(obj, &S(st)->input, 0);
    lv_obj_add_style(obj, &S(st)->focused_input, LV_STATE_FOCUSED);
    lv_theme_shadcn_add_interaction(st, obj, LV_PART_MAIN);
    lv_obj_add_style(obj, &S(st)->scrollbar, LV_PART_SCROLLBAR);
    lv_obj_add_style(obj, &S(st)->scrollbar_scrolled, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
}
#endif

#if LV_USE_TEXTAREA || LV_USE_SPINBOX
static void add_text_input(lv_theme_shadcn_state_t * st, lv_obj_t * obj)
{
    add_input(st, obj);
    lv_obj_add_style(obj, &S(st)->placeholder, LV_PART_TEXTAREA_PLACEHOLDER);
    lv_obj_add_style(obj, &S(st)->cursor, LV_PART_CURSOR | LV_STATE_FOCUSED);
    lv_obj_add_style(obj, &S(st)->accent, LV_PART_SELECTED);
}
#endif

bool lv_theme_shadcn_apply_inputs(lv_theme_shadcn_state_t * st, lv_obj_t * obj, lv_obj_t * parent)
{
    LV_UNUSED(st);
    LV_UNUSED(obj);
    LV_UNUSED(parent);
#if LV_USE_TEXTAREA
    if(lv_obj_check_type(obj, &lv_textarea_class)) {
        add_text_input(st, obj);
        return true;
    }
#endif
#if LV_USE_SPINBOX
    if(lv_obj_check_type(obj, &lv_spinbox_class)) {
        add_text_input(st, obj);
        return true;
    }
#endif
#if LV_USE_DROPDOWN
    if(lv_obj_check_type(obj, &lv_dropdown_class)) {
        lv_obj_add_style(obj, &S(st)->control, 0);
        add_input(st, obj);
        lv_obj_add_style(obj, &S(st)->focused_input, LV_STATE_CHECKED);
        lv_theme_shadcn_set_touch_target(st, obj);
        return true;
    }
    if(lv_obj_check_type(obj, &lv_dropdownlist_class)) {
        lv_obj_add_style(obj, &S(st)->surface[LV_THEME_SHADCN_SURFACE_POPOVER], 0);
        lv_obj_add_style(obj, &S(st)->popover_frame, 0);
        lv_obj_add_style(obj, &S(st)->accent, LV_PART_SELECTED);
        lv_obj_add_style(obj, &S(st)->accent, LV_PART_SELECTED | LV_STATE_CHECKED);
        lv_obj_add_style(obj, &S(st)->accent, LV_PART_SELECTED | LV_STATE_PRESSED);
        lv_obj_add_style(obj, &S(st)->scrollbar, LV_PART_SCROLLBAR);
        lv_obj_add_style(obj, &S(st)->scrollbar_scrolled, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
        return true;
    }
#endif
#if LV_USE_ROLLER
    if(lv_obj_check_type(obj, &lv_roller_class)) {
        add_input(st, obj);
        lv_obj_add_style(obj, &S(st)->roller, 0);
        lv_obj_add_style(obj, &S(st)->accent, LV_PART_SELECTED);
        return true;
    }
#endif
#if LV_USE_KEYBOARD
    if(lv_obj_check_type(obj, &lv_keyboard_class)) {
        lv_obj_add_style(obj, &S(st)->surface[LV_THEME_SHADCN_SURFACE_POPOVER], 0);
        lv_obj_add_style(obj, &S(st)->pad_xs, 0);
        lv_obj_add_style(obj, &S(st)->key, LV_PART_ITEMS);
        lv_obj_add_style(obj, &S(st)->key_pressed, LV_PART_ITEMS | LV_STATE_PRESSED);
        lv_obj_add_style(obj, &S(st)->key_control, LV_PART_ITEMS | LV_STATE_CHECKED);
        lv_obj_add_style(obj, &S(st)->focus_ring, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
        lv_obj_add_style(obj, &S(st)->disabled, LV_PART_ITEMS | LV_STATE_DISABLED);
        return true;
    }
#endif
    return false;
}
