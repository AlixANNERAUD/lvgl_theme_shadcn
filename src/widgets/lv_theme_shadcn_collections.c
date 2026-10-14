#include "../lv_theme_shadcn_private.h"

#if LV_USE_LIST || LV_USE_TABLE || LV_USE_CHART
static void add_scrollbar(lv_theme_shadcn_state_t * st, lv_obj_t * obj)
{
    lv_obj_add_style(obj, &S(st)->scrollbar, LV_PART_SCROLLBAR);
    lv_obj_add_style(obj, &S(st)->scrollbar_scrolled, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
}
#endif

#if LV_USE_CALENDAR
static void calendar_draw_cb(lv_event_t * e)
{
    lv_obj_t * days = lv_event_get_current_target_obj(e);
    lv_theme_shadcn_state_t * st = lv_theme_shadcn_state_from_obj(days);
    if(st == NULL) return;
    lv_draw_task_t * task = lv_event_get_draw_task(e);
    lv_draw_dsc_base_t * base = lv_draw_task_get_draw_dsc(task);
    if(base == NULL || base->part != LV_PART_ITEMS) return;
    uint32_t id = base->id1;
    lv_draw_fill_dsc_t * fill = lv_draw_task_get_fill_dsc(task);
    lv_draw_border_dsc_t * border = lv_draw_task_get_border_dsc(task);
    lv_draw_label_dsc_t * label = lv_draw_task_get_label_dsc(task);
    bool today = lv_buttonmatrix_has_button_ctrl(days, id, LV_BUTTONMATRIX_CTRL_CUSTOM_1);
    bool highlighted = lv_buttonmatrix_has_button_ctrl(days, id, LV_BUTTONMATRIX_CTRL_CUSTOM_2);
    bool active = lv_buttonmatrix_get_selected_button(days) == id
                  && (lv_obj_has_state(days, LV_STATE_PRESSED) || lv_obj_has_state(days, LV_STATE_FOCUS_KEY));
    if(!today && !highlighted && !active) {
        if(fill) fill->opa = LV_OPA_TRANSP;
        if(border) border->opa = LV_OPA_TRANSP;
        return;
    }
    if(highlighted) {
        if(fill) {
            fill->color = st->palette.primary;
            fill->opa = LV_OPA_COVER;
        }
        if(border) border->color = st->palette.primary;
        if(label) label->color = st->palette.primary_fg;
    }
    if(today && border) {
        border->color = st->palette.ring;
    }
}

static void calendar_attach_cb(lv_event_t * e)
{
    lv_obj_t * cal = lv_event_get_current_target_obj(e);
    lv_obj_t * days = lv_calendar_get_btnmatrix(cal);
    if(days == NULL) return;
    lv_obj_remove_event_cb(cal, calendar_attach_cb);
    lv_obj_remove_event_cb(days, calendar_draw_cb);
    lv_obj_add_event_cb(days, calendar_draw_cb, LV_EVENT_DRAW_TASK_ADDED, NULL);
    lv_obj_set_send_draw_task_events(days, true);
}
#endif

static bool apply_tab_button(lv_theme_shadcn_state_t * st, lv_obj_t * obj, lv_obj_t * parent)
{
#if LV_USE_TABVIEW && LV_USE_BUTTON
    if(!lv_obj_check_type(obj, &lv_button_class)) return false;
    lv_obj_t * tabview = lv_obj_get_parent(parent);
    if(tabview == NULL || !lv_obj_check_type(tabview, &lv_tabview_class)) return false;
    if(lv_obj_get_child(tabview, 0) != parent) return false;
    lv_obj_add_style(obj, &S(st)->control, 0);
    lv_obj_add_style(obj, &S(st)->tab_button, 0);
    lv_obj_add_style(obj, &S(st)->tab_button_checked, LV_STATE_CHECKED);
    lv_theme_shadcn_add_interaction(st, obj, LV_PART_MAIN);
    return true;
#else
    LV_UNUSED(st);
    LV_UNUSED(obj);
    LV_UNUSED(parent);
    return false;
#endif
}

bool lv_theme_shadcn_apply_collections(lv_theme_shadcn_state_t * st, lv_obj_t * obj, lv_obj_t * parent)
{
    LV_UNUSED(parent);
    if(apply_tab_button(st, obj, parent)) return true;
#if LV_USE_TABVIEW
    if(lv_obj_check_type(obj, &lv_obj_class) && lv_obj_check_type(parent, &lv_tabview_class)
       && lv_obj_get_index(obj) == 0) {
        lv_obj_add_style(obj, &S(st)->tab_bar, 0);
        lv_obj_add_style(obj, &S(st)->focus_ring, LV_STATE_FOCUS_KEY);
        return true;
    }
    if(lv_obj_check_type(obj, &lv_tabview_class)) {
        lv_obj_add_style(obj, &S(st)->transparent, 0);
        lv_obj_add_style(obj, &S(st)->pad_sm, 0);
        return true;
    }
#endif
#if LV_USE_LIST
    if(lv_obj_check_type(obj, &lv_list_class)) {
        lv_obj_add_style(obj, &S(st)->surface[LV_THEME_SHADCN_SURFACE_CARD], 0);
        lv_obj_add_style(obj, &S(st)->container, 0);
        lv_obj_add_style(obj, &S(st)->pad_none, 0);
        add_scrollbar(st, obj);
        return true;
    }
    if(lv_obj_check_type(obj, &lv_list_text_class)) {
        lv_obj_add_style(obj, &S(st)->list_text, 0);
        return true;
    }
    if(lv_obj_check_type(obj, &lv_list_button_class)) {
        lv_obj_add_style(obj, &S(st)->list_button, 0);
        lv_obj_add_style(obj, &S(st)->accent, LV_STATE_PRESSED);
        lv_obj_add_style(obj, &S(st)->accent, LV_STATE_CHECKED);
        lv_theme_shadcn_add_interaction(st, obj, LV_PART_MAIN);
        return true;
    }
#endif
#if LV_USE_TABLE
    if(lv_obj_check_type(obj, &lv_table_class)) {
        lv_obj_add_style(obj, &S(st)->surface[LV_THEME_SHADCN_SURFACE_BACKGROUND], 0);
        lv_obj_add_style(obj, &S(st)->container, 0);
        lv_obj_add_style(obj, &S(st)->pad_none, 0);
        lv_obj_add_style(obj, &S(st)->table_cell, LV_PART_ITEMS);
        lv_obj_add_style(obj, &S(st)->accent, LV_PART_ITEMS | LV_STATE_PRESSED);
        lv_obj_add_style(obj, &S(st)->focus_ring, LV_STATE_FOCUS_KEY);
        add_scrollbar(st, obj);
        return true;
    }
#endif
#if LV_USE_CALENDAR
    if(lv_obj_check_type(obj, &lv_calendar_class)) {
        lv_obj_add_style(obj, &S(st)->surface[LV_THEME_SHADCN_SURFACE_CARD], 0);
        lv_obj_add_style(obj, &S(st)->container, 0);
        lv_obj_add_style(obj, &S(st)->pad_sm, 0);
        lv_obj_remove_event_cb(obj, calendar_attach_cb);
        lv_obj_add_event_cb(obj, calendar_attach_cb, LV_EVENT_STYLE_CHANGED, NULL);
        return true;
    }
    if(lv_obj_check_type(obj, &lv_buttonmatrix_class) && lv_obj_check_type(parent, &lv_calendar_class)) {
        lv_obj_add_style(obj, &S(st)->transparent, 0);
        lv_obj_add_style(obj, &S(st)->pad_xs, 0);
        lv_obj_add_style(obj, &S(st)->calendar_day, LV_PART_ITEMS);
        lv_obj_add_style(obj, &S(st)->accent, LV_PART_ITEMS | LV_STATE_PRESSED);
        lv_obj_add_style(obj, &S(st)->text_muted, LV_PART_ITEMS | LV_STATE_DISABLED);
        lv_obj_add_style(obj, &S(st)->focus_ring, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
        return true;
    }
    if(lv_obj_check_type(obj, &lv_calendar_header_arrow_class)
       || lv_obj_check_type(obj, &lv_calendar_header_dropdown_class)) {
        lv_obj_add_style(obj, &S(st)->transparent, 0);
        lv_obj_add_style(obj, &S(st)->pad_xs, 0);
        return true;
    }
#endif
#if LV_USE_CHART
    if(lv_obj_check_type(obj, &lv_chart_class)) {
        lv_obj_add_style(obj, &S(st)->surface[LV_THEME_SHADCN_SURFACE_BACKGROUND], 0);
        lv_obj_add_style(obj, &S(st)->container, 0);
        lv_obj_add_style(obj, &S(st)->pad_md, 0);
        lv_obj_add_style(obj, &S(st)->chart, 0);
        lv_obj_add_style(obj, &S(st)->chart_series, LV_PART_ITEMS);
        lv_obj_add_style(obj, &S(st)->chart_series, LV_PART_INDICATOR);
        lv_obj_add_style(obj, &S(st)->chart_series, LV_PART_CURSOR);
        add_scrollbar(st, obj);
        return true;
    }
#endif
    return false;
}
