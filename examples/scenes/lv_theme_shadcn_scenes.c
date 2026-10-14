#include "lv_theme_shadcn_scenes.h"

#define SCENE_W 480
#define SCENE_H 320
#define SCENE_PAD 8
#define CONTENT_W (SCENE_W - 2 * SCENE_PAD)
#define CONTENT_H (SCENE_H - 2 * SCENE_PAD)
#define HALF_W ((CONTENT_W - SCENE_PAD) / 2)
#define THIRD_W ((CONTENT_W - 2 * SCENE_PAD) / 3)
#define QUARTER_W ((CONTENT_W - 3 * SCENE_PAD) / 4)

static void prepare(lv_obj_t * parent)
{
    lv_obj_set_size(parent, SCENE_W, SCENE_H);
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_style_pad_all(parent, SCENE_PAD, 0);
    lv_obj_set_style_pad_gap(parent, SCENE_PAD, 0);
    lv_obj_set_scrollable(parent, false);
}

static lv_obj_t * card(lv_obj_t * parent, int32_t w, int32_t h, lv_flex_flow_t flow, const char * title)
{
    lv_obj_t * c = lv_obj_create(parent);
    lv_obj_set_size(c, w, h);
    lv_obj_set_flex_flow(c, flow);
    lv_obj_set_style_pad_all(c, 12, 0);
    lv_obj_set_style_pad_gap(c, 12, 0);
    lv_obj_set_scrollable(c, false);
    if(title) {
        lv_obj_t * label = lv_label_create(c);
        lv_label_set_text(label, title);
        if(flow == LV_FLEX_FLOW_ROW_WRAP) lv_obj_set_width(label, lv_pct(100));
    }
    return c;
}

#if LV_USE_BUTTON && LV_USE_LABEL
static lv_obj_t * add_button(lv_obj_t * parent, const char * text, lv_theme_shadcn_variant_t variant, lv_state_t state)
{
    lv_obj_t * btn = lv_button_create(parent);
    lv_obj_set_size(btn, QUARTER_W, 48);
    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_center(label);
    lv_theme_shadcn_set_variant(btn, variant);
    if(state != LV_STATE_DEFAULT) lv_obj_add_state(btn, state);
    return btn;
}
#endif

static void build_buttons(lv_obj_t * parent)
{
    prepare(parent);

#if LV_USE_LABEL
    lv_obj_t * heading = lv_label_create(parent);
    lv_label_set_text(heading, "Buttons");
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(heading, &lv_font_montserrat_20, 0);
#endif
    lv_obj_set_width(heading, lv_pct(100));
#endif

#if LV_USE_BUTTON && LV_USE_LABEL
    add_button(parent, "Default", LV_THEME_SHADCN_VARIANT_DEFAULT, LV_STATE_DEFAULT);
    add_button(parent, "Secondary", LV_THEME_SHADCN_VARIANT_SECONDARY, LV_STATE_DEFAULT);
    add_button(parent, "Outline", LV_THEME_SHADCN_VARIANT_OUTLINE, LV_STATE_DEFAULT);
    add_button(parent, "Ghost", LV_THEME_SHADCN_VARIANT_GHOST, LV_STATE_DEFAULT);
    add_button(parent, "Destructive", LV_THEME_SHADCN_VARIANT_DESTRUCTIVE, LV_STATE_DEFAULT);
    add_button(parent, "Pressed", LV_THEME_SHADCN_VARIANT_DEFAULT, LV_STATE_PRESSED);
    add_button(parent, "Focused", LV_THEME_SHADCN_VARIANT_DEFAULT, LV_STATE_FOCUS_KEY);
    add_button(parent, "Disabled", LV_THEME_SHADCN_VARIANT_DEFAULT, LV_STATE_DISABLED);
#endif

#if LV_USE_BUTTONMATRIX
    static const char * map[] = { "1", "2", "3", "" };
    lv_obj_t * matrix = lv_buttonmatrix_create(parent);
    lv_buttonmatrix_set_map(matrix, map);
    lv_obj_set_size(matrix, CONTENT_W, 96);
#endif
}

static void build_toggles(lv_obj_t * parent)
{
    prepare(parent);
    const int32_t h = CONTENT_H;

#if LV_USE_SWITCH
    lv_obj_t * switches = card(parent, THIRD_W, h, LV_FLEX_FLOW_COLUMN, "Switch");
    lv_switch_create(switches);
    lv_obj_t * sw_on = lv_switch_create(switches);
    lv_obj_add_state(sw_on, LV_STATE_CHECKED);
    lv_obj_t * sw_disabled = lv_switch_create(switches);
    lv_obj_add_state(sw_disabled, LV_STATE_DISABLED);
#endif

#if LV_USE_CHECKBOX
    lv_obj_t * boxes = card(parent, THIRD_W, h, LV_FLEX_FLOW_COLUMN, "Checkbox");
    lv_obj_t * cb_off = lv_checkbox_create(boxes);
    lv_checkbox_set_text_static(cb_off, "Unchecked");
    lv_obj_t * cb_on = lv_checkbox_create(boxes);
    lv_checkbox_set_text_static(cb_on, "Checked");
    lv_obj_add_state(cb_on, LV_STATE_CHECKED);
    lv_obj_t * cb_disabled = lv_checkbox_create(boxes);
    lv_checkbox_set_text_static(cb_disabled, "Disabled");
    lv_obj_add_state(cb_disabled, LV_STATE_DISABLED);
#endif

#if LV_USE_LED
    lv_obj_t * leds = card(parent, THIRD_W, h, LV_FLEX_FLOW_COLUMN, "LED");
    lv_obj_t * led_on = lv_led_create(leds);
    lv_led_on(led_on);
    lv_obj_t * led_off = lv_led_create(leds);
    lv_led_off(led_off);
#endif
}

static void build_ranges(lv_obj_t * parent)
{
    prepare(parent);
    const int32_t h = CONTENT_H;

    lv_obj_t * linear = card(parent, HALF_W, h, LV_FLEX_FLOW_COLUMN, "Slider, bar and scale");
#if LV_USE_SLIDER
    lv_obj_t * slider = lv_slider_create(linear);
    lv_obj_set_width(slider, lv_pct(100));
    lv_slider_set_value(slider, 60, LV_ANIM_OFF);
#endif
#if LV_USE_BAR
    lv_obj_t * bar = lv_bar_create(linear);
    lv_obj_set_width(bar, lv_pct(100));
    lv_bar_set_value(bar, 35, LV_ANIM_OFF);
#endif
#if LV_USE_SCALE
    lv_obj_t * scale = lv_scale_create(linear);
    lv_obj_set_size(scale, lv_pct(100), 50);
    lv_scale_set_total_tick_count(scale, 21);
    lv_scale_set_major_tick_every(scale, 5);
#endif

    lv_obj_t * round = card(parent, HALF_W, h, LV_FLEX_FLOW_ROW_WRAP, "Arc and spinner");
    lv_obj_set_flex_align(round, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
#if LV_USE_ARC
    lv_obj_t * arc = lv_arc_create(round);
    lv_obj_set_size(arc, 100, 100);
    lv_arc_set_value(arc, 65);
#endif
#if LV_USE_SPINNER
    lv_obj_t * spinner = lv_spinner_create(round);
    lv_obj_set_size(spinner, 48, 48);
#endif
}

static void build_inputs(lv_obj_t * parent)
{
    prepare(parent);
    const int32_t h = CONTENT_H;

    lv_obj_t * text = card(parent, HALF_W, h, LV_FLEX_FLOW_COLUMN, "Text input");
#if LV_USE_TEXTAREA
    lv_obj_t * empty = lv_textarea_create(text);
    lv_obj_set_width(empty, lv_pct(100));
    lv_textarea_set_one_line(empty, true);
    lv_textarea_set_placeholder_text(empty, "Email");

    lv_obj_t * invalid = lv_textarea_create(text);
    lv_obj_set_width(invalid, lv_pct(100));
    lv_textarea_set_one_line(invalid, true);
    lv_textarea_set_text(invalid, "invalid@");
    lv_theme_shadcn_set_error(invalid, true);
#endif
#if LV_USE_DROPDOWN
    lv_obj_t * dropdown = lv_dropdown_create(text);
    lv_obj_set_width(dropdown, lv_pct(100));
    lv_dropdown_set_options_static(dropdown, "Apple\nBanana\nCherry");
#endif

    lv_obj_t * pickers = card(parent, HALF_W, h, LV_FLEX_FLOW_COLUMN, "Pickers");
#if LV_USE_ROLLER
    lv_obj_t * roller = lv_roller_create(pickers);
    lv_obj_set_width(roller, lv_pct(100));
    lv_roller_set_options(roller, "Mon\nTue\nWed\nThu\nFri", LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller, 3);
    lv_roller_set_selected(roller, 1, LV_ANIM_OFF);
#endif
#if LV_USE_SPINBOX
    lv_obj_t * spinbox = lv_spinbox_create(pickers);
    lv_obj_set_width(spinbox, lv_pct(100));
    lv_spinbox_set_range(spinbox, 0, 999);
    lv_spinbox_set_digit_format(spinbox, 3, 0);
    lv_spinbox_set_value(spinbox, 42);
#endif
}

static void build_collections(lv_obj_t * parent)
{
    prepare(parent);
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN_WRAP);
    const int32_t third_h = (CONTENT_H - 2 * SCENE_PAD) / 3;
    const int32_t calendar_h = CONTENT_H - third_h - SCENE_PAD;

#if LV_USE_TABVIEW
    lv_obj_t * tabview = lv_tabview_create(parent);
    lv_obj_set_size(tabview, HALF_W, third_h);
    lv_obj_t * tab_a = lv_tabview_add_tab(tabview, "Account");
    lv_tabview_add_tab(tabview, "Team");
    lv_obj_t * tab_label = lv_label_create(tab_a);
    lv_label_set_text(tab_label, "Profile");
#endif

#if LV_USE_TABLE
    lv_obj_t * table = lv_table_create(parent);
    lv_obj_set_size(table, HALF_W, third_h);
    lv_table_set_column_count(table, 3);
    lv_table_set_row_count(table, 2);
    lv_table_set_column_width(table, 0, 80);
    lv_table_set_column_width(table, 1, 60);
    lv_table_set_column_width(table, 2, 80);
    lv_table_set_cell_value(table, 0, 0, "Item");
    lv_table_set_cell_value(table, 0, 1, "Qty");
    lv_table_set_cell_value(table, 0, 2, "Cost");
    lv_table_set_cell_value(table, 1, 0, "Pear");
    lv_table_set_cell_value(table, 1, 1, "5");
    lv_table_set_cell_value(table, 1, 2, "12");
#endif

#if LV_USE_CHART
    lv_obj_t * chart = lv_chart_create(parent);
    lv_obj_set_size(chart, HALF_W, third_h);
    lv_chart_set_point_count(chart, 10);
    lv_chart_series_t * series = lv_chart_add_series(chart, lv_theme_shadcn_get_palette(lv_obj_get_display(parent))->chart[0],
                                                     LV_CHART_AXIS_PRIMARY_Y);
    static const int32_t values[10] = { 20, 35, 30, 55, 45, 70, 60, 80, 65, 90 };
    for(int i = 0; i < 10; i++) lv_chart_set_next_value(chart, series, values[i]);
#endif

#if LV_USE_CALENDAR
    lv_obj_t * calendar = lv_calendar_create(parent);
    lv_obj_set_size(calendar, HALF_W, calendar_h);
    lv_calendar_set_today_date(calendar, 2026, 10, 14);
    lv_calendar_set_month_shown(calendar, 2026, 10);
#endif

#if LV_USE_LIST
    lv_obj_t * list = lv_list_create(parent);
    lv_obj_set_size(list, HALF_W, third_h);
    lv_list_add_button(list, LV_SYMBOL_WIFI, "Network");
    lv_list_add_button(list, LV_SYMBOL_BELL, "Alerts");
#endif
}

static void build_overlays(lv_obj_t * parent)
{
    prepare(parent);

#if LV_USE_KEYBOARD
    lv_obj_t * keyboard = lv_keyboard_create(parent);
    lv_obj_set_ignore_layout(keyboard, true);
    lv_obj_set_size(keyboard, CONTENT_W, 136);
    lv_obj_align(keyboard, LV_ALIGN_BOTTOM_MID, 0, 0);
#endif

#if LV_USE_MSGBOX
    lv_obj_t * msgbox = lv_msgbox_create(parent);
    lv_obj_set_ignore_layout(msgbox, true);
    lv_obj_set_size(msgbox, 340, LV_SIZE_CONTENT);
    lv_obj_set_style_pad_all(msgbox, 12, 0);
    lv_obj_set_style_pad_gap(msgbox, 6, 0);
    lv_msgbox_add_title(msgbox, "Delete file?");
    lv_msgbox_add_text(msgbox, "This action cannot be undone.");
    lv_msgbox_add_close_button(msgbox);
    lv_msgbox_add_footer_button(msgbox, "Cancel");
    lv_obj_t * confirm = lv_msgbox_add_footer_button(msgbox, "Delete");
    lv_theme_shadcn_set_variant(confirm, LV_THEME_SHADCN_VARIANT_DESTRUCTIVE);
    lv_obj_align(msgbox, LV_ALIGN_TOP_MID, 0, 0);
#endif
}

const lv_theme_shadcn_scene_t * lv_theme_shadcn_scenes(uint32_t * count)
{
    static const lv_theme_shadcn_scene_t scenes[] = {
        { "buttons", build_buttons },
        { "toggles", build_toggles },
        { "ranges", build_ranges },
        { "inputs", build_inputs },
        { "collections", build_collections },
        { "overlays", build_overlays },
    };
    if(count) *count = sizeof(scenes) / sizeof(scenes[0]);
    return scenes;
}
