#include "../lv_theme_shadcn_private.h"

void lv_theme_shadcn_add_button(lv_theme_shadcn_state_t * st, lv_obj_t * obj, lv_theme_shadcn_variant_t variant)
{
    lv_obj_add_style(obj, &S(st)->control, 0);
    lv_obj_add_style(obj, &S(st)->variant[variant], 0);
    lv_obj_add_style(obj, &S(st)->variant_pressed[variant], LV_STATE_PRESSED);
    lv_theme_shadcn_add_interaction(st, obj, LV_PART_MAIN);
    lv_theme_shadcn_set_touch_target(st, obj);
}

bool lv_theme_shadcn_apply_actions(lv_theme_shadcn_state_t * st, lv_obj_t * obj, lv_obj_t * parent)
{
    LV_UNUSED(parent);
#if LV_USE_BUTTON
    if(lv_obj_check_type(obj, &lv_button_class)) {
        lv_theme_shadcn_add_button(st, obj, LV_THEME_SHADCN_VARIANT_DEFAULT);
        return true;
    }
#endif
#if LV_USE_IMAGEBUTTON
    if(lv_obj_check_type(obj, &lv_imagebutton_class)) {
        lv_theme_shadcn_add_interaction(st, obj, LV_PART_MAIN);
        return true;
    }
#endif
#if LV_USE_BUTTONMATRIX
    if(lv_obj_check_type(obj, &lv_buttonmatrix_class)) {
        lv_obj_add_style(obj, &S(st)->transparent, 0);
        lv_obj_add_style(obj, &S(st)->pad_xs, 0);
        lv_obj_add_style(obj, &S(st)->control, LV_PART_ITEMS);
        lv_obj_add_style(obj, &S(st)->variant[LV_THEME_SHADCN_VARIANT_OUTLINE], LV_PART_ITEMS);
        lv_obj_add_style(obj, &S(st)->variant_pressed[LV_THEME_SHADCN_VARIANT_OUTLINE], LV_PART_ITEMS | LV_STATE_PRESSED);
        lv_obj_add_style(obj, &S(st)->accent, LV_PART_ITEMS | LV_STATE_CHECKED);
        lv_obj_add_style(obj, &S(st)->focus_ring, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
        lv_obj_add_style(obj, &S(st)->disabled, LV_PART_ITEMS | LV_STATE_DISABLED);
        return true;
    }
#endif
    return false;
}
