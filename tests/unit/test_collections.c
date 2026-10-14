#include "harness.h"

static const lv_theme_shadcn_palette_t * dark(void)
{
    return lv_theme_shadcn_palette_zinc_dark();
}

static void list_rows(void)
{
    lv_obj_t * list = lv_list_create(lv_screen_active());
    lv_obj_t * text = lv_list_add_text(list, "Section");
    lv_obj_t * btn = lv_list_add_button(list, NULL, "Row");
    CHECK_COLOR(lv_obj_get_style_bg_color(list, LV_PART_MAIN), dark()->card);
    CHECK_INT(lv_obj_get_style_pad_top(list, LV_PART_MAIN), 0);
    CHECK_COLOR(lv_obj_get_style_text_color(text, LV_PART_MAIN), dark()->muted_fg);
    CHECK(lv_obj_get_style_text_font(text, LV_PART_MAIN) == &lv_font_montserrat_12);
    CHECK_INT(lv_obj_get_style_bg_opa(btn, LV_PART_MAIN), LV_OPA_TRANSP);
    CHECK_INT(lv_obj_get_style_border_side(btn, LV_PART_MAIN), LV_BORDER_SIDE_BOTTOM);
    CHECK_INT(lv_obj_get_style_min_height(btn, LV_PART_MAIN), lv_display_dpx(harness_display(), 48));
    lv_obj_add_state(btn, LV_STATE_PRESSED);
    harness_settle();
    CHECK_COLOR(lv_obj_get_style_bg_color(btn, LV_PART_MAIN), dark()->accent);
    CHECK_INT(lv_obj_get_style_bg_opa(btn, LV_PART_MAIN), LV_OPA_COVER);
}

static void tabview_segmented(void)
{
    lv_obj_t * tv = lv_tabview_create(lv_screen_active());
    lv_obj_t * page = lv_tabview_add_tab(tv, "One");
    lv_tabview_add_tab(tv, "Two");
    lv_obj_t * bar = lv_tabview_get_tab_bar(tv);
    lv_obj_t * tab = lv_obj_get_child(bar, 0);
    CHECK_COLOR(lv_obj_get_style_bg_color(bar, LV_PART_MAIN), dark()->muted);
    CHECK_INT(lv_obj_get_style_bg_opa(bar, LV_PART_MAIN), LV_OPA_COVER);
    CHECK_INT(lv_obj_get_style_radius(bar, LV_PART_MAIN), lv_display_dpx(harness_display(), 8));
    CHECK_COLOR(lv_obj_get_style_text_color(lv_obj_get_child(bar, 1), LV_PART_MAIN), dark()->muted_fg);
    CHECK_INT(lv_obj_get_style_bg_opa(lv_obj_get_child(bar, 1), LV_PART_MAIN), LV_OPA_TRANSP);
    CHECK(lv_obj_has_state(tab, LV_STATE_CHECKED));
    CHECK_COLOR(lv_obj_get_style_bg_color(tab, LV_PART_MAIN), dark()->background);
    CHECK_INT(lv_obj_get_style_bg_opa(tab, LV_PART_MAIN), LV_OPA_COVER);
    CHECK_COLOR(lv_obj_get_style_text_color(tab, LV_PART_MAIN), dark()->foreground);
    CHECK_INT(lv_obj_get_style_bg_opa(page, LV_PART_MAIN), LV_OPA_TRANSP);
    CHECK_INT(lv_obj_get_style_pad_left(page, LV_PART_MAIN), lv_display_dpx(harness_display(), 16));
}

static void table_cells(void)
{
    lv_obj_t * t = lv_table_create(lv_screen_active());
    CHECK_COLOR(lv_obj_get_style_border_color(t, LV_PART_ITEMS), dark()->border);
    CHECK_INT(lv_obj_get_style_border_side(t, LV_PART_ITEMS), LV_BORDER_SIDE_BOTTOM);
    CHECK_INT(lv_obj_get_style_bg_opa(t, LV_PART_ITEMS), LV_OPA_TRANSP);
    CHECK_COLOR(lv_obj_get_style_text_color(t, LV_PART_ITEMS), dark()->foreground);
    CHECK_COLOR(lv_obj_get_style_border_color(t, LV_PART_MAIN), dark()->border);
    CHECK_INT(lv_obj_get_style_radius(t, LV_PART_MAIN), lv_display_dpx(harness_display(), 8));
    CHECK(lv_obj_get_style_clip_corner(t, LV_PART_MAIN));
}

static uint32_t table_rows_bordered;
static uint32_t table_last_row_bordered;

static void table_probe_cb(lv_event_t * e)
{
    lv_obj_t * t = lv_event_get_current_target_obj(e);
    lv_draw_task_t * task = lv_event_get_draw_task(e);
    lv_draw_dsc_base_t * base = lv_draw_task_get_draw_dsc(task);
    if(base->part != LV_PART_ITEMS) return;
    lv_draw_border_dsc_t * border = lv_draw_task_get_border_dsc(task);
    if(border == NULL || border->opa <= LV_OPA_MIN || border->width == 0) return;
    if(base->id1 == lv_table_get_row_count(t) - 1) table_last_row_bordered++;
    else table_rows_bordered++;
}

static void table_last_row_has_no_divider(void)
{
    lv_obj_t * t = lv_table_create(lv_screen_active());
    lv_table_set_column_count(t, 2);
    lv_table_set_row_count(t, 3);
    for(uint32_t r = 0; r < 3; r++) {
        lv_table_set_cell_value(t, r, 0, "a");
        lv_table_set_cell_value(t, r, 1, "b");
    }
    lv_obj_add_event_cb(t, table_probe_cb, LV_EVENT_DRAW_TASK_ADDED, NULL);
    lv_obj_set_send_draw_task_events(t, true);
    table_rows_bordered = 0;
    table_last_row_bordered = 0;
    lv_refr_now(harness_display());
    CHECK(table_rows_bordered > 0);
    CHECK_INT(table_last_row_bordered, 0);
}

static bool highlight_seen;
static lv_color_t highlight_fill;
static lv_opa_t highlight_opa;
static bool today_seen;
static lv_color_t today_border;

static void calendar_probe_cb(lv_event_t * e)
{
    lv_obj_t * days = lv_event_get_current_target_obj(e);
    lv_draw_task_t * task = lv_event_get_draw_task(e);
    lv_draw_dsc_base_t * base = lv_draw_task_get_draw_dsc(task);
    if(base->part != LV_PART_ITEMS) return;
    lv_draw_fill_dsc_t * fill = lv_draw_task_get_fill_dsc(task);
    lv_draw_border_dsc_t * border = lv_draw_task_get_border_dsc(task);
    if(fill && lv_buttonmatrix_has_button_ctrl(days, base->id1, LV_BUTTONMATRIX_CTRL_CUSTOM_2)) {
        highlight_seen = true;
        highlight_fill = fill->color;
        highlight_opa = fill->opa;
    }
    if(border && lv_buttonmatrix_has_button_ctrl(days, base->id1, LV_BUTTONMATRIX_CTRL_CUSTOM_1)) {
        today_seen = true;
        today_border = border->color;
    }
}

static int plain_fill_seen;
static int plain_fill_opaque;
static int plain_border_opaque;

static void calendar_plain_cb(lv_event_t * e)
{
    lv_obj_t * days = lv_event_get_current_target_obj(e);
    lv_draw_task_t * task = lv_event_get_draw_task(e);
    lv_draw_dsc_base_t * base = lv_draw_task_get_draw_dsc(task);
    if(base->part != LV_PART_ITEMS) return;
    if(lv_buttonmatrix_has_button_ctrl(days, base->id1, LV_BUTTONMATRIX_CTRL_CUSTOM_1)) return;
    if(lv_buttonmatrix_has_button_ctrl(days, base->id1, LV_BUTTONMATRIX_CTRL_CUSTOM_2)) return;
    lv_draw_fill_dsc_t * fill = lv_draw_task_get_fill_dsc(task);
    lv_draw_border_dsc_t * border = lv_draw_task_get_border_dsc(task);
    if(fill) {
        plain_fill_seen++;
        if(fill->opa != LV_OPA_TRANSP) plain_fill_opaque++;
    }
    if(border && border->opa != LV_OPA_TRANSP) plain_border_opaque++;
}

static void calendar_plain_transparent(void)
{
    lv_obj_t * cal = lv_calendar_create(lv_screen_active());
    lv_obj_set_size(cal, 300, 300);
    lv_theme_shadcn_set_surface(cal, LV_THEME_SHADCN_SURFACE_BACKGROUND);
    lv_obj_t * days = lv_calendar_get_btnmatrix(cal);
    lv_calendar_set_today_date(cal, 2026, 10, 14);
    lv_calendar_set_month_shown(cal, 2026, 10);
    lv_obj_add_event_cb(days, calendar_plain_cb, LV_EVENT_DRAW_TASK_ADDED, NULL);
    plain_fill_seen = 0;
    plain_fill_opaque = 0;
    plain_border_opaque = 0;
    lv_refr_now(harness_display());
    CHECK(plain_fill_seen > 0);
    CHECK_INT(plain_fill_opaque, 0);
    CHECK_INT(plain_border_opaque, 0);
}

static void calendar_days(void)
{
    lv_obj_t * cal = lv_calendar_create(lv_screen_active());
    lv_obj_set_size(cal, 300, 300);
    lv_obj_t * days = lv_calendar_get_btnmatrix(cal);
    CHECK_COLOR(lv_obj_get_style_bg_color(cal, LV_PART_MAIN), dark()->card);
    CHECK_INT(lv_obj_get_style_bg_opa(days, LV_PART_MAIN), LV_OPA_TRANSP);
    CHECK_COLOR(lv_obj_get_style_bg_color(days, LV_PART_ITEMS), dark()->card);
    CHECK_INT(lv_obj_get_style_border_width(days, LV_PART_ITEMS), lv_display_dpx(harness_display(), 1));
    CHECK_COLOR(lv_obj_get_style_text_color(days, LV_PART_ITEMS), dark()->card_fg);
    CHECK_INT(lv_obj_get_style_radius(days, LV_PART_ITEMS), lv_display_dpx(harness_display(), 6));
    lv_calendar_set_today_date(cal, 2026, 10, 14);
    lv_calendar_set_month_shown(cal, 2026, 10);
    static lv_calendar_date_t highlighted[] = { { 2026, 10, 20 } };
    lv_calendar_set_highlighted_dates(cal, highlighted, 1);
    lv_obj_add_event_cb(days, calendar_probe_cb, LV_EVENT_DRAW_TASK_ADDED, NULL);
    highlight_seen = false;
    today_seen = false;
    lv_refr_now(harness_display());
    CHECK(highlight_seen);
    CHECK_COLOR(highlight_fill, dark()->primary);
    CHECK_INT(highlight_opa, LV_OPA_COVER);
    CHECK(today_seen);
    CHECK_COLOR(today_border, dark()->ring);
}

static void chart_frame(void)
{
    lv_obj_t * ch = lv_chart_create(lv_screen_active());
    CHECK_COLOR(lv_obj_get_style_bg_color(ch, LV_PART_MAIN), dark()->background);
    CHECK_COLOR(lv_obj_get_style_line_color(ch, LV_PART_MAIN), dark()->border);
    CHECK(lv_obj_get_style_line_width(ch, LV_PART_MAIN) > 0);
    CHECK_COLOR(lv_obj_get_style_border_color(ch, LV_PART_MAIN), dark()->border);
}

static void chart_series_visible(void)
{
    lv_obj_t * ch = lv_chart_create(lv_screen_active());
    CHECK(lv_obj_get_style_line_width(ch, LV_PART_ITEMS) > 0);
    CHECK(lv_obj_get_style_width(ch, LV_PART_INDICATOR) > 0);
    CHECK(lv_obj_get_style_height(ch, LV_PART_INDICATOR) > 0);
}

int main(void)
{
    RUN(list_rows);
    RUN(tabview_segmented);
    RUN(table_cells);
    RUN(table_last_row_has_no_divider);
    RUN(calendar_days);
    RUN(chart_frame);
    RUN(chart_series_visible);
    RUN(calendar_plain_transparent);
    return harness_result();
}
