#include "../lv_theme_shadcn_private.h"

static void add_scrollbar(lv_theme_shadcn_state_t * st, lv_obj_t * obj)
{
    lv_obj_add_style(obj, &S(st)->scrollbar, LV_PART_SCROLLBAR);
    lv_obj_add_style(obj, &S(st)->scrollbar_scrolled, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
}

static void add_card(lv_theme_shadcn_state_t * st, lv_obj_t * obj)
{
    lv_obj_add_style(obj, &S(st)->surface[LV_THEME_SHADCN_SURFACE_CARD], 0);
    lv_obj_add_style(obj, &S(st)->container, 0);
    lv_theme_shadcn_add_interaction(st, obj, LV_PART_MAIN);
    add_scrollbar(st, obj);
}

static bool apply_plain_obj(lv_theme_shadcn_state_t * st, lv_obj_t * obj, lv_obj_t * parent)
{
#if LV_USE_TABVIEW
    if(lv_obj_check_type(parent, &lv_tabview_class)) {
        lv_obj_add_style(obj, &S(st)->transparent, 0);
        return true;
    }
    lv_obj_t * grandparent = lv_obj_get_parent(parent);
    if(grandparent && lv_obj_check_type(grandparent, &lv_tabview_class) && lv_obj_get_child(grandparent, 0) != parent) {
        lv_obj_add_style(obj, &S(st)->transparent, 0);
        lv_obj_add_style(obj, &S(st)->pad_lg, 0);
        add_scrollbar(st, obj);
        return true;
    }
#endif
#if LV_USE_WIN
    if(lv_obj_check_type(parent, &lv_win_class)) {
        if(lv_obj_get_index(obj) == 0) {
            lv_obj_add_style(obj, &S(st)->surface[LV_THEME_SHADCN_SURFACE_MUTED], 0);
            lv_obj_add_style(obj, &S(st)->pad_sm, 0);
        }
        else {
            lv_obj_add_style(obj, &S(st)->surface[LV_THEME_SHADCN_SURFACE_BACKGROUND], 0);
            lv_obj_add_style(obj, &S(st)->pad_lg, 0);
            add_scrollbar(st, obj);
        }
        return true;
    }
#endif
    add_card(st, obj);
    return true;
}

bool lv_theme_shadcn_apply_surfaces(lv_theme_shadcn_state_t * st, lv_obj_t * obj, lv_obj_t * parent)
{
    if(lv_obj_check_type(obj, &lv_obj_class)) return apply_plain_obj(st, obj, parent);

#if LV_USE_WIN
    if(lv_obj_check_type(obj, &lv_win_class)) {
        lv_obj_add_style(obj, &S(st)->surface[LV_THEME_SHADCN_SURFACE_BACKGROUND], 0);
        return true;
    }
#endif
#if LV_USE_TILEVIEW
    if(lv_obj_check_type(obj, &lv_tileview_class)) {
        lv_obj_add_style(obj, &S(st)->transparent, 0);
        add_scrollbar(st, obj);
        return true;
    }
    if(lv_obj_check_type(obj, &lv_tileview_tile_class)) {
        lv_obj_add_style(obj, &S(st)->transparent, 0);
        add_scrollbar(st, obj);
        return true;
    }
#endif
#if LV_USE_MENU
    if(lv_obj_check_type(obj, &lv_menu_class)) {
        lv_obj_add_style(obj, &S(st)->surface[LV_THEME_SHADCN_SURFACE_BACKGROUND], 0);
        return true;
    }
    if(lv_obj_check_type(obj, &lv_menu_page_class)) {
        lv_obj_add_style(obj, &S(st)->transparent, 0);
        lv_obj_add_style(obj, &S(st)->pad_md, 0);
        add_scrollbar(st, obj);
        return true;
    }
    if(lv_obj_check_type(obj, &lv_menu_section_class)) {
        lv_obj_add_style(obj, &S(st)->surface[LV_THEME_SHADCN_SURFACE_CARD], 0);
        lv_obj_add_style(obj, &S(st)->container, 0);
        lv_obj_add_style(obj, &S(st)->pad_none, 0);
        return true;
    }
    if(lv_obj_check_type(obj, &lv_menu_cont_class)) {
        lv_obj_add_style(obj, &S(st)->pad_md, 0);
        lv_obj_add_style(obj, &S(st)->accent, LV_STATE_PRESSED);
        lv_obj_add_style(obj, &S(st)->accent, LV_STATE_CHECKED);
        lv_theme_shadcn_add_interaction(st, obj, LV_PART_MAIN);
        return true;
    }
    if(lv_obj_check_type(obj, &lv_menu_separator_class)
       || lv_obj_check_type(obj, &lv_menu_main_cont_class)
       || lv_obj_check_type(obj, &lv_menu_sidebar_cont_class)) {
        lv_obj_add_style(obj, &S(st)->transparent, 0);
        return true;
    }
    if(lv_obj_check_type(obj, &lv_menu_main_header_cont_class)
       || lv_obj_check_type(obj, &lv_menu_sidebar_header_cont_class)) {
        lv_obj_add_style(obj, &S(st)->pad_md, 0);
        return true;
    }
#endif
    if(lv_obj_check_type(obj, &lv_label_class)) return true;
#if LV_USE_SPAN
    if(lv_obj_check_type(obj, &lv_spangroup_class)) return true;
#endif
#if LV_USE_ARCLABEL
    if(lv_obj_check_type(obj, &lv_arclabel_class)) return true;
#endif
    return false;
}
