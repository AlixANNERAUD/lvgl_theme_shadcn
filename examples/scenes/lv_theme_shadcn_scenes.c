#include "lv_theme_shadcn_scenes.h"

#define SCENE_W 480
#define SCENE_H 320
#define SCENE_PAD 8
#define CONTENT_W (SCENE_W - 2 * SCENE_PAD)
#define CONTENT_H (SCENE_H - 2 * SCENE_PAD)
#define HALF_W ((CONTENT_W - SCENE_PAD) / 2)
#define QUARTER_W ((CONTENT_W - 3 * SCENE_PAD) / 4)
#define CARD_PAD 12

static const lv_theme_shadcn_palette_t * palette_of(lv_obj_t * obj)
{
    return lv_theme_shadcn_get_palette(lv_obj_get_display(obj));
}

static void prepare(lv_obj_t * parent, lv_flex_flow_t flow)
{
    lv_obj_set_size(parent, SCENE_W, SCENE_H);
    lv_obj_set_flex_flow(parent, flow);
    lv_obj_set_style_pad_all(parent, SCENE_PAD, 0);
    lv_obj_set_style_pad_gap(parent, SCENE_PAD, 0);
    lv_obj_set_scrollable(parent, false);
}

static lv_obj_t * box(lv_obj_t * parent, lv_flex_flow_t flow, int32_t gap)
{
    lv_obj_t * b = lv_obj_create(parent);
    lv_obj_remove_style_all(b);
    lv_obj_set_size(b, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(b, flow);
    lv_obj_set_style_pad_gap(b, gap, 0);
    lv_obj_set_scrollable(b, false);
    return b;
}

static lv_obj_t * inline_group(lv_obj_t * parent, int32_t gap, lv_flex_align_t align)
{
    lv_obj_t * group = box(parent, LV_FLEX_FLOW_ROW, gap);
    lv_obj_set_width(group, LV_SIZE_CONTENT);
    lv_obj_set_flex_align(group, LV_FLEX_ALIGN_START, align, align);
    return group;
}

static lv_obj_t * spread_row(lv_obj_t * parent)
{
    lv_obj_t * row = box(parent, LV_FLEX_FLOW_ROW, SCENE_PAD);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    return row;
}

static lv_obj_t * text(lv_obj_t * parent, const char * str)
{
    lv_obj_t * label = lv_label_create(parent);
    lv_label_set_text(label, str);
    return label;
}

static lv_obj_t * small_text(lv_obj_t * parent, const char * str)
{
    lv_obj_t * label = text(parent, str);
#if LV_FONT_MONTSERRAT_12
    lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
#endif
    return label;
}

static lv_obj_t * muted_text(lv_obj_t * parent, const char * str)
{
    lv_obj_t * label = small_text(parent, str);
    const lv_theme_shadcn_palette_t * palette = palette_of(parent);
    if(palette) lv_obj_set_style_text_color(label, palette->muted_fg, 0);
    return label;
}

static lv_obj_t * heading(lv_obj_t * parent, const char * str)
{
    lv_obj_t * label = text(parent, str);
#if LV_FONT_MONTSERRAT_20
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
#endif
    return label;
}

static void page_header(lv_obj_t * parent, const char * title, const char * description)
{
    lv_obj_t * row = box(parent, LV_FLEX_FLOW_ROW, SCENE_PAD);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END);
    heading(row, title);
    lv_obj_t * desc = muted_text(row, description);
    lv_obj_set_style_pad_bottom(desc, 2, 0);
}

static lv_obj_t * card(lv_obj_t * parent, int32_t w, const char * title, const char * description)
{
    lv_obj_t * c = lv_obj_create(parent);
    lv_obj_set_size(c, w, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(c, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(c, CARD_PAD, 0);
    lv_obj_set_style_pad_gap(c, CARD_PAD, 0);
    lv_obj_set_scrollable(c, false);
    if(title) {
        lv_obj_t * head = box(c, LV_FLEX_FLOW_COLUMN, 2);
        text(head, title);
        if(description) muted_text(head, description);
    }
    return c;
}

static void equalize(lv_obj_t * a, lv_obj_t * b)
{
    lv_obj_update_layout(lv_obj_get_parent(a));
    int32_t ha = lv_obj_get_height(a);
    int32_t hb = lv_obj_get_height(b);
    int32_t h = ha > hb ? ha : hb;
    lv_obj_set_height(a, h);
    lv_obj_set_height(b, h);
}

static lv_obj_t * labelled(lv_obj_t * parent, const char * title, const char * value)
{
    lv_obj_t * group = box(parent, LV_FLEX_FLOW_COLUMN, 10);
    lv_obj_t * row = spread_row(group);
    text(row, title);
    muted_text(row, value);
    return group;
}

#if LV_USE_CHART
static lv_color_t chart_color(lv_obj_t * obj, int index, lv_palette_t fallback)
{
    const lv_theme_shadcn_palette_t * palette = palette_of(obj);
    return palette ? palette->chart[index] : lv_palette_main(fallback);
}
#endif

#if LV_USE_SPINNER
static lv_obj_t * small_spinner(lv_obj_t * parent, int32_t size, int32_t arc_width)
{
    lv_obj_t * spinner = lv_spinner_create(parent);
    lv_obj_set_size(spinner, size, size);
    lv_obj_set_style_arc_width(spinner, arc_width, 0);
    lv_obj_set_style_arc_width(spinner, arc_width, LV_PART_INDICATOR);
    return spinner;
}
#endif

static lv_obj_t * field(lv_obj_t * parent, const char * label)
{
    lv_obj_t * f = box(parent, LV_FLEX_FLOW_COLUMN, 6);
    small_text(f, label);
    return f;
}

#if LV_USE_BUTTON
static lv_obj_t * button(lv_obj_t * parent, const char * str, lv_theme_shadcn_variant_t variant, int32_t w)
{
    lv_obj_t * btn = lv_button_create(parent);
    lv_obj_set_width(btn, w);
    if(str) text(btn, str);
    lv_theme_shadcn_set_variant(btn, variant);
    return btn;
}
#endif

#if LV_USE_SWITCH
static lv_obj_t * switch_row(lv_obj_t * parent, const char * icon, const char * str, lv_state_t state)
{
    lv_obj_t * row = spread_row(parent);
    lv_obj_t * left = inline_group(row, 10, LV_FLEX_ALIGN_CENTER);
    if(icon) {
        lv_obj_t * glyph = text(left, icon);
        lv_obj_set_width(glyph, 16);
    }
    text(left, str);
    lv_obj_t * sw = lv_switch_create(row);
    lv_obj_set_size(sw, 40, 22);
    if(state != LV_STATE_DEFAULT) lv_obj_add_state(sw, state);
    return sw;
}
#endif

static void build_showcase(lv_obj_t * parent)
{
    prepare(parent, LV_FLEX_FLOW_ROW_WRAP);

    lv_obj_t * header = spread_row(parent);
    lv_obj_t * titles = box(header, LV_FLEX_FLOW_COLUMN, 0);
    lv_obj_set_width(titles, LV_SIZE_CONTENT);
    heading(titles, "Living room");
    muted_text(titles, "4 devices online");
#if LV_USE_DROPDOWN
    lv_obj_t * scene = lv_dropdown_create(header);
    lv_obj_set_width(scene, 128);
    lv_dropdown_set_options_static(scene, "Evening\nMorning\nAway");
#endif

    lv_obj_t * devices = card(parent, HALF_W, "Devices", "Toggle connected devices.");
#if LV_USE_SWITCH
    switch_row(devices, LV_SYMBOL_WIFI, "Wi-Fi", LV_STATE_CHECKED);
    switch_row(devices, LV_SYMBOL_BLUETOOTH, "Bluetooth", LV_STATE_DEFAULT);
    switch_row(devices, LV_SYMBOL_BELL, "Alerts", LV_STATE_CHECKED);
#endif
#if LV_USE_SLIDER
    lv_obj_t * slider = lv_slider_create(labelled(devices, "Brightness", "72%"));
    lv_obj_set_size(slider, lv_pct(100), 8);
    lv_slider_set_value(slider, 72, LV_ANIM_OFF);
#endif

    lv_obj_t * energy = card(parent, HALF_W, "Energy", "Usage over the last 7 days.");
    lv_obj_t * figure = inline_group(energy, 8, LV_FLEX_ALIGN_END);
    lv_obj_set_width(figure, lv_pct(100));
    heading(figure, "12.4 kWh");
    lv_obj_t * trend = muted_text(figure, "+8% vs last week");
    lv_obj_set_style_pad_bottom(trend, 2, 0);
#if LV_USE_CHART
    lv_obj_t * chart = lv_chart_create(energy);
    lv_obj_set_size(chart, lv_pct(100), 76);
    lv_obj_set_style_bg_opa(chart, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(chart, 0, 0);
    lv_obj_set_style_pad_all(chart, 0, 0);
    lv_obj_set_style_pad_column(chart, 8, 0);
    lv_obj_set_style_radius(chart, 4, LV_PART_ITEMS);
    lv_chart_set_type(chart, LV_CHART_TYPE_BAR);
    lv_chart_set_div_line_count(chart, 0, 0);
    lv_chart_set_point_count(chart, 7);
    lv_chart_set_axis_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    lv_chart_series_t * series = lv_chart_add_series(chart, chart_color(parent, 1, LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);
    static const int32_t usage[7] = { 45, 62, 38, 74, 56, 88, 67 };
    for(int i = 0; i < 7; i++) lv_chart_set_next_value(chart, series, usage[i]);
#endif
    equalize(devices, energy);
#if LV_USE_CHART
    lv_obj_set_flex_grow(chart, 1);
#endif

    lv_obj_t * footer = spread_row(parent);
    muted_text(footer, "Last synced 2 minutes ago");
#if LV_USE_BUTTON
    lv_obj_t * actions = box(footer, LV_FLEX_FLOW_ROW, SCENE_PAD);
    lv_obj_set_width(actions, LV_SIZE_CONTENT);
    button(actions, "Reset", LV_THEME_SHADCN_VARIANT_OUTLINE, LV_SIZE_CONTENT);
    button(actions, "Apply scene", LV_THEME_SHADCN_VARIANT_DEFAULT, LV_SIZE_CONTENT);
#endif
}

static void build_buttons(lv_obj_t * parent)
{
    prepare(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(parent, 14, 0);
    page_header(parent, "Buttons", "Variants, icons and states");

#if LV_USE_BUTTON
    lv_obj_t * variants = box(parent, LV_FLEX_FLOW_ROW_WRAP, SCENE_PAD);
    lv_obj_t * variants_label = muted_text(variants, "Variants");
    lv_obj_set_width(variants_label, lv_pct(100));
    button(variants, "Default", LV_THEME_SHADCN_VARIANT_DEFAULT, QUARTER_W);
    button(variants, "Secondary", LV_THEME_SHADCN_VARIANT_SECONDARY, QUARTER_W);
    button(variants, "Outline", LV_THEME_SHADCN_VARIANT_OUTLINE, QUARTER_W);
    button(variants, "Ghost", LV_THEME_SHADCN_VARIANT_GHOST, QUARTER_W);
    button(variants, "Destructive", LV_THEME_SHADCN_VARIANT_DESTRUCTIVE, QUARTER_W);
    button(variants, LV_SYMBOL_DOWNLOAD "  Download", LV_THEME_SHADCN_VARIANT_DEFAULT, QUARTER_W);
    button(variants, LV_SYMBOL_UPLOAD "  Upload", LV_THEME_SHADCN_VARIANT_OUTLINE, QUARTER_W);
    button(variants, LV_SYMBOL_SETTINGS "  Settings", LV_THEME_SHADCN_VARIANT_SECONDARY, QUARTER_W);

    lv_obj_t * states = box(parent, LV_FLEX_FLOW_ROW_WRAP, SCENE_PAD);
    lv_obj_t * states_label = muted_text(states, "States");
    lv_obj_set_width(states_label, lv_pct(100));
    lv_obj_add_state(button(states, "Pressed", LV_THEME_SHADCN_VARIANT_DEFAULT, QUARTER_W), LV_STATE_PRESSED);
    lv_obj_add_state(button(states, "Focused", LV_THEME_SHADCN_VARIANT_OUTLINE, QUARTER_W), LV_STATE_FOCUS_KEY);
    lv_obj_add_state(button(states, "Disabled", LV_THEME_SHADCN_VARIANT_DEFAULT, QUARTER_W), LV_STATE_DISABLED);
    lv_obj_t * loading = button(states, NULL, LV_THEME_SHADCN_VARIANT_SECONDARY, QUARTER_W);
#if LV_USE_SPINNER
    lv_obj_t * spinner = small_spinner(loading, 14, 2);
    lv_obj_set_style_arc_opa(spinner, LV_OPA_TRANSP, 0);
    const lv_theme_shadcn_palette_t * palette = palette_of(parent);
    if(palette) lv_obj_set_style_arc_color(spinner, palette->secondary_fg, LV_PART_INDICATOR);
#endif
    text(loading, "Loading");
#endif

#if LV_USE_BUTTONMATRIX
    lv_obj_t * segmented = box(parent, LV_FLEX_FLOW_COLUMN, SCENE_PAD);
    muted_text(segmented, "Button matrix");
    static const char * map[] = { "Day", "Week", "Month", "Year", "" };
    lv_obj_t * matrix = lv_buttonmatrix_create(segmented);
    lv_buttonmatrix_set_map(matrix, map);
    lv_obj_set_size(matrix, CONTENT_W, 40);
    lv_obj_set_style_pad_all(matrix, 0, 0);
    lv_obj_set_style_pad_gap(matrix, SCENE_PAD, 0);
    lv_buttonmatrix_set_button_ctrl_all(matrix, LV_BUTTONMATRIX_CTRL_CHECKABLE);
    lv_buttonmatrix_set_one_checked(matrix, true);
    lv_buttonmatrix_set_button_ctrl(matrix, 1, LV_BUTTONMATRIX_CTRL_CHECKED);
#endif
}

#if LV_USE_LED
static void status_led(lv_obj_t * parent, const char * str, lv_color_t color, uint8_t brightness)
{
    lv_obj_t * item = inline_group(parent, 8, LV_FLEX_ALIGN_CENTER);
    lv_obj_t * led = lv_led_create(item);
    lv_obj_set_size(led, 10, 10);
    lv_led_set_color(led, color);
    lv_led_set_brightness(led, brightness);
    text(item, str);
}
#endif

static void build_toggles(lv_obj_t * parent)
{
    prepare(parent, LV_FLEX_FLOW_ROW_WRAP);
    page_header(parent, "Toggles", "Switches, checkboxes and LEDs");

    lv_obj_t * notifications = card(parent, HALF_W, "Notifications", "How should we reach you?");
    lv_obj_set_style_pad_row(notifications, 16, 0);
#if LV_USE_SWITCH
    switch_row(notifications, NULL, "Push", LV_STATE_CHECKED);
    switch_row(notifications, NULL, "Email digest", LV_STATE_DEFAULT);
    switch_row(notifications, NULL, "SMS alerts", LV_STATE_DISABLED);
#endif

    lv_obj_t * privacy = card(parent, HALF_W, "Privacy", "Choose what you share.");
    lv_obj_set_style_pad_row(privacy, 16, 0);
#if LV_USE_CHECKBOX
    lv_obj_t * analytics = lv_checkbox_create(privacy);
    lv_checkbox_set_text_static(analytics, "Usage analytics");
    lv_obj_add_state(analytics, LV_STATE_CHECKED);
    lv_obj_t * reports = lv_checkbox_create(privacy);
    lv_checkbox_set_text_static(reports, "Crash reports");
    lv_obj_t * beta = lv_checkbox_create(privacy);
    lv_checkbox_set_text_static(beta, "Beta features");
    lv_obj_add_state(beta, LV_STATE_DISABLED);
#endif
    equalize(notifications, privacy);

#if LV_USE_LED
    lv_obj_t * status = card(parent, CONTENT_W, "Status", "Live state of connected services.");
    lv_obj_t * leds = box(status, LV_FLEX_FLOW_ROW, 8);
    lv_obj_set_flex_align(leds, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    status_led(leds, "Online", lv_color_hex(0x22c55e), LV_LED_BRIGHT_MAX);
    status_led(leds, "Syncing", lv_color_hex(0xf59e0b), 220);
    status_led(leds, "Degraded", lv_color_hex(0xef4444), LV_LED_BRIGHT_MAX);
    status_led(leds, "Offline", lv_color_hex(0xa1a1aa), 150);
#endif
}

static void build_ranges(lv_obj_t * parent)
{
    prepare(parent, LV_FLEX_FLOW_ROW_WRAP);
    page_header(parent, "Ranges", "Sliders, bars, scales and arcs");

    lv_obj_t * linear = card(parent, HALF_W, "Audio", "Output level and storage.");
    lv_obj_set_style_pad_row(linear, 16, 0);
#if LV_USE_SLIDER
    lv_obj_t * volume = lv_slider_create(labelled(linear, "Volume", "60%"));
    lv_obj_set_size(volume, lv_pct(100), 8);
    lv_slider_set_value(volume, 60, LV_ANIM_OFF);

    lv_obj_t * band = lv_slider_create(labelled(linear, "Band", "20 - 80 Hz"));
    lv_obj_set_size(band, lv_pct(100), 8);
    lv_slider_set_mode(band, LV_SLIDER_MODE_RANGE);
    lv_slider_set_value(band, 80, LV_ANIM_OFF);
    lv_slider_set_start_value(band, 20, LV_ANIM_OFF);
#endif
#if LV_USE_BAR
    lv_obj_t * bar = lv_bar_create(labelled(linear, "Storage", "35 of 100 GB"));
    lv_obj_set_size(bar, lv_pct(100), 8);
    lv_bar_set_value(bar, 35, LV_ANIM_OFF);
#endif
#if LV_USE_SCALE
    lv_obj_t * scale = lv_scale_create(linear);
    lv_obj_set_size(scale, lv_pct(100), 32);
    lv_obj_set_style_pad_hor(scale, 10, 0);
    lv_scale_set_total_tick_count(scale, 21);
    lv_scale_set_major_tick_every(scale, 5);
#if LV_FONT_MONTSERRAT_12
    lv_obj_set_style_text_font(scale, &lv_font_montserrat_12, 0);
#endif
#endif

    lv_obj_t * round = card(parent, HALF_W, "Upload", "Syncing your library.");
#if LV_USE_ARC
    lv_obj_t * arc = lv_arc_create(round);
    lv_obj_set_size(arc, 140, 140);
    lv_arc_set_value(arc, 65);
    lv_obj_set_clickable(arc, false);
    lv_obj_t * arc_value = heading(arc, "65%");
    lv_obj_center(arc_value);
#endif
#if LV_USE_SPINNER
    lv_obj_t * progress = inline_group(round, 8, LV_FLEX_ALIGN_CENTER);
    small_spinner(progress, 16, 3);
    muted_text(progress, "128 of 196 files");
#endif
    equalize(linear, round);
    lv_obj_set_flex_align(round, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
}

static void build_inputs(lv_obj_t * parent)
{
    prepare(parent, LV_FLEX_FLOW_ROW_WRAP);
    page_header(parent, "Inputs", "Text, selects and pickers");

    lv_obj_t * account = card(parent, HALF_W, "Account", NULL);
#if LV_USE_TEXTAREA
    lv_obj_t * email = lv_textarea_create(field(account, "Email"));
    lv_obj_set_width(email, lv_pct(100));
    lv_textarea_set_one_line(email, true);
    lv_textarea_set_placeholder_text(email, "name@example.com");

    lv_obj_t * username_field = field(account, "Username");
    lv_obj_t * username = lv_textarea_create(username_field);
    lv_obj_set_width(username, lv_pct(100));
    lv_textarea_set_one_line(username, true);
    lv_textarea_set_text(username, "ab");
    lv_theme_shadcn_set_error(username, true);
    lv_obj_t * hint = muted_text(username_field, "Must be at least 3 characters.");
    const lv_theme_shadcn_palette_t * palette = palette_of(parent);
    if(palette) lv_obj_set_style_text_color(hint, palette->destructive, 0);
#endif
#if LV_USE_DROPDOWN
    lv_obj_t * role = lv_dropdown_create(field(account, "Role"));
    lv_obj_set_width(role, lv_pct(100));
    lv_dropdown_set_options_static(role, "Developer\nDesigner\nManager");
#endif

    lv_obj_t * schedule = card(parent, HALF_W, "Schedule", NULL);
#if LV_USE_ROLLER
    lv_obj_t * roller = lv_roller_create(field(schedule, "Day"));
    lv_obj_set_width(roller, lv_pct(100));
    lv_roller_set_options(roller, "Mon\nTue\nWed\nThu\nFri", LV_ROLLER_MODE_NORMAL);
    lv_obj_set_style_text_line_space(roller, 6, 0);
    lv_roller_set_visible_row_count(roller, 5);
    lv_roller_set_selected(roller, 2, LV_ANIM_OFF);
#endif
#if LV_USE_SPINBOX
    lv_obj_t * quantity = box(field(schedule, "Guests"), LV_FLEX_FLOW_ROW, SCENE_PAD);
#if LV_USE_BUTTON
    button(quantity, LV_SYMBOL_MINUS, LV_THEME_SHADCN_VARIANT_OUTLINE, 40);
#endif
    lv_obj_t * spinbox = lv_spinbox_create(quantity);
    lv_obj_set_flex_grow(spinbox, 1);
    lv_obj_set_style_text_align(spinbox, LV_TEXT_ALIGN_CENTER, 0);
    lv_spinbox_set_range(spinbox, 1, 9);
    lv_spinbox_set_digit_format(spinbox, 1, 0);
    lv_spinbox_set_value(spinbox, 4);
#if LV_USE_BUTTON
    button(quantity, LV_SYMBOL_PLUS, LV_THEME_SHADCN_VARIANT_OUTLINE, 40);
#endif
#endif
    equalize(account, schedule);
}

static void build_collections(lv_obj_t * parent)
{
    prepare(parent, LV_FLEX_FLOW_ROW);
#if LV_USE_TABVIEW || LV_USE_TABLE || LV_USE_CHART
    lv_obj_t * left = box(parent, LV_FLEX_FLOW_COLUMN, SCENE_PAD);
    lv_obj_set_size(left, HALF_W, CONTENT_H);
#endif
#if LV_USE_CALENDAR || LV_USE_LIST
    lv_obj_t * right = box(parent, LV_FLEX_FLOW_COLUMN, SCENE_PAD);
    lv_obj_set_size(right, HALF_W, CONTENT_H);
#endif

#if LV_USE_TABVIEW
    lv_obj_t * tabview = lv_tabview_create(left);
    lv_obj_set_size(tabview, HALF_W, 104);
    lv_obj_set_style_pad_all(tabview, 0, 0);
    lv_tabview_set_tab_bar_size(tabview, 36);
    lv_obj_t * tab_a = lv_tabview_add_tab(tabview, "Account");
    lv_tabview_add_tab(tabview, "Team");
    lv_obj_set_style_pad_hor(tab_a, 4, 0);
    lv_obj_set_style_pad_ver(tab_a, 8, 0);
    lv_obj_set_flex_flow(tab_a, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(tab_a, 2, 0);
    text(tab_a, "Profile");
    muted_text(tab_a, "Manage your account settings.");
#endif

#if LV_USE_TABLE
    lv_obj_t * table = lv_table_create(left);
    lv_obj_set_size(table, HALF_W, LV_SIZE_CONTENT);
    lv_table_set_column_count(table, 3);
    lv_table_set_row_count(table, 3);
    lv_table_set_column_width(table, 0, HALF_W - 142);
    lv_table_set_column_width(table, 1, 60);
    lv_table_set_column_width(table, 2, 80);
    lv_table_set_cell_value(table, 0, 0, "Item");
    lv_table_set_cell_value(table, 0, 1, "Qty");
    lv_table_set_cell_value(table, 0, 2, "Cost");
    lv_table_set_cell_value(table, 1, 0, "Pear");
    lv_table_set_cell_value(table, 1, 1, "5");
    lv_table_set_cell_value(table, 1, 2, "$12.00");
    lv_table_set_cell_value(table, 2, 0, "Plum");
    lv_table_set_cell_value(table, 2, 1, "3");
    lv_table_set_cell_value(table, 2, 2, "$7.50");
#endif

#if LV_USE_CHART
    lv_obj_t * chart = lv_chart_create(left);
    lv_obj_set_width(chart, HALF_W);
    lv_obj_set_flex_grow(chart, 1);
    lv_chart_set_point_count(chart, 10);
    lv_chart_set_div_line_count(chart, 3, 0);
    lv_chart_set_axis_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    lv_chart_series_t * revenue = lv_chart_add_series(chart, chart_color(parent, 0, LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_series_t * costs = lv_chart_add_series(chart, chart_color(parent, 1, LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);
    static const int32_t revenue_values[10] = { 20, 35, 30, 55, 45, 70, 60, 80, 65, 90 };
    static const int32_t cost_values[10] = { 10, 15, 25, 20, 30, 28, 40, 35, 45, 42 };
    for(int i = 0; i < 10; i++) {
        lv_chart_set_next_value(chart, revenue, revenue_values[i]);
        lv_chart_set_next_value(chart, costs, cost_values[i]);
    }
#endif

#if LV_USE_CALENDAR
    lv_obj_t * calendar = lv_calendar_create(right);
    lv_obj_set_width(calendar, HALF_W);
    lv_obj_set_flex_grow(calendar, 1);
    lv_calendar_set_today_date(calendar, 2026, 10, 14);
    lv_calendar_set_month_shown(calendar, 2026, 10);
    static lv_calendar_date_t trip[] = { { 2026, 10, 21 }, { 2026, 10, 22 }, { 2026, 10, 23 } };
    lv_calendar_set_highlighted_dates(calendar, trip, 3);
#endif

#if LV_USE_LIST
    lv_obj_t * list = lv_list_create(right);
    lv_obj_set_size(list, HALF_W, LV_SIZE_CONTENT);
    lv_list_add_button(list, LV_SYMBOL_WIFI, "Network");
    lv_list_add_button(list, LV_SYMBOL_BELL, "Alerts");
#endif
}

static void build_overlays(lv_obj_t * parent)
{
    prepare(parent, LV_FLEX_FLOW_ROW_WRAP);

#if LV_USE_KEYBOARD
    lv_obj_t * keyboard = lv_keyboard_create(parent);
    lv_obj_set_ignore_layout(keyboard, true);
    lv_obj_set_size(keyboard, CONTENT_W, 128);
    lv_obj_align(keyboard, LV_ALIGN_BOTTOM_MID, 0, 0);
#endif

#if LV_USE_MSGBOX
    lv_obj_t * msgbox = lv_msgbox_create(parent);
    lv_obj_set_ignore_layout(msgbox, true);
    lv_obj_set_size(msgbox, 360, LV_SIZE_CONTENT);
    lv_obj_set_style_pad_all(msgbox, 16, 0);
    lv_obj_set_style_pad_gap(msgbox, 8, 0);
    lv_msgbox_add_title(msgbox, "Delete file?");
    lv_msgbox_add_text(msgbox, "This action cannot be undone.");
    lv_msgbox_add_close_button(msgbox);
    lv_obj_t * cancel = lv_msgbox_add_footer_button(msgbox, "Cancel");
    lv_theme_shadcn_set_variant(cancel, LV_THEME_SHADCN_VARIANT_OUTLINE);
    lv_obj_t * confirm = lv_msgbox_add_footer_button(msgbox, "Delete");
    lv_theme_shadcn_set_variant(confirm, LV_THEME_SHADCN_VARIANT_DESTRUCTIVE);
    lv_obj_set_flex_align(lv_msgbox_get_footer(msgbox), LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_align(msgbox, LV_ALIGN_TOP_MID, 0, 0);
#endif
}

const lv_theme_shadcn_scene_t * lv_theme_shadcn_scenes(uint32_t * count)
{
    static const lv_theme_shadcn_scene_t scenes[] = {
        { "showcase", build_showcase },
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
