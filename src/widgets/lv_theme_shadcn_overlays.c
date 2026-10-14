#include "../lv_theme_shadcn_private.h"

bool lv_theme_shadcn_apply_overlays(lv_theme_shadcn_state_t * st, lv_obj_t * obj, lv_obj_t * parent)
{
    LV_UNUSED(st);
    LV_UNUSED(obj);
    LV_UNUSED(parent);
#if LV_USE_MSGBOX
    if(lv_obj_check_type(obj, &lv_msgbox_backdrop_class)) {
        lv_obj_add_style(obj, &S(st)->backdrop, 0);
        return true;
    }
    if(lv_obj_check_type(obj, &lv_msgbox_class)) {
        lv_obj_add_style(obj, &S(st)->surface[LV_THEME_SHADCN_SURFACE_POPOVER], 0);
        lv_obj_add_style(obj, &S(st)->msgbox, 0);
        return true;
    }
    if(lv_obj_check_type(obj, &lv_msgbox_header_class)) {
        lv_obj_add_style(obj, &S(st)->transparent, 0);
        lv_obj_add_style(obj, &S(st)->font_large, 0);
        return true;
    }
    if(lv_obj_check_type(obj, &lv_msgbox_content_class)) {
        lv_obj_add_style(obj, &S(st)->transparent, 0);
        lv_obj_add_style(obj, &S(st)->text_muted, 0);
        lv_obj_add_style(obj, &S(st)->scrollbar, LV_PART_SCROLLBAR);
        return true;
    }
    if(lv_obj_check_type(obj, &lv_msgbox_footer_class)) {
        lv_obj_add_style(obj, &S(st)->msgbox_footer, 0);
        return true;
    }
    if(lv_obj_check_type(obj, &lv_msgbox_footer_button_class)) {
        lv_theme_shadcn_add_button(st, obj, LV_THEME_SHADCN_VARIANT_DEFAULT);
        return true;
    }
    if(lv_obj_check_type(obj, &lv_msgbox_header_button_class)) {
        lv_theme_shadcn_add_button(st, obj, LV_THEME_SHADCN_VARIANT_GHOST);
        return true;
    }
#endif
    return false;
}
