#include "harness.h"

static const lv_theme_shadcn_palette_t * dark(void)
{
    return lv_theme_shadcn_palette_zinc_dark();
}

static void textarea_input_box(void)
{
    lv_obj_t * ta = lv_textarea_create(lv_screen_active());
    CHECK_COLOR(lv_obj_get_style_bg_color(ta, LV_PART_MAIN), dark()->background);
    CHECK_COLOR(lv_obj_get_style_border_color(ta, LV_PART_MAIN), dark()->input);
    CHECK_COLOR(lv_obj_get_style_text_color(ta, LV_PART_TEXTAREA_PLACEHOLDER), dark()->muted_fg);
    CHECK_COLOR(lv_obj_get_style_bg_color(ta, LV_PART_SELECTED), dark()->accent);
    lv_obj_add_state(ta, LV_STATE_FOCUSED);
    harness_settle();
    CHECK_COLOR(lv_obj_get_style_border_color(ta, LV_PART_MAIN), dark()->ring);
    CHECK_COLOR(lv_obj_get_style_border_color(ta, LV_PART_CURSOR), dark()->foreground);
}

static void textarea_error(void)
{
    lv_obj_t * ta = lv_textarea_create(lv_screen_active());
    lv_theme_shadcn_set_error(ta, true);
    lv_obj_add_state(ta, LV_STATE_FOCUSED);
    harness_settle();
    CHECK_COLOR(lv_obj_get_style_border_color(ta, LV_PART_MAIN), dark()->destructive);
    lv_theme_shadcn_set_error(ta, false);
    harness_settle();
    CHECK_COLOR(lv_obj_get_style_border_color(ta, LV_PART_MAIN), dark()->ring);
}

static void dropdown_error_open(void)
{
    lv_obj_t * dd = lv_dropdown_create(lv_screen_active());
    lv_theme_shadcn_set_error(dd, true);
    lv_obj_add_state(dd, LV_STATE_CHECKED);
    harness_settle();
    CHECK_COLOR(lv_obj_get_style_border_color(dd, LV_PART_MAIN), dark()->destructive);
    lv_theme_shadcn_set_error(dd, false);
    harness_settle();
    CHECK_COLOR(lv_obj_get_style_border_color(dd, LV_PART_MAIN), dark()->ring);
}

static void spinbox_matches_textarea(void)
{
    lv_obj_t * sb = lv_spinbox_create(lv_screen_active());
    CHECK_COLOR(lv_obj_get_style_border_color(sb, LV_PART_MAIN), dark()->input);
}

static void dropdown_and_list(void)
{
    lv_obj_t * dd = lv_dropdown_create(lv_screen_active());
    CHECK_COLOR(lv_obj_get_style_border_color(dd, LV_PART_MAIN), dark()->input);
    CHECK_INT(lv_obj_get_style_min_height(dd, LV_PART_MAIN), lv_display_dpx(harness_display(), 40));
    lv_dropdown_open(dd);
    lv_obj_t * list = lv_dropdown_get_list(dd);
    CHECK_COLOR(lv_obj_get_style_bg_color(list, LV_PART_MAIN), dark()->popover);
    CHECK_COLOR(lv_obj_get_style_bg_color(list, LV_PART_SELECTED), dark()->accent);
    CHECK_COLOR(lv_obj_get_style_text_color(list, LV_PART_SELECTED), dark()->accent_fg);
}

static void dropdown_list_spaces_options(void)
{
    lv_obj_t * dd = lv_dropdown_create(lv_screen_active());
    lv_dropdown_set_options_static(dd, "One\nTwo\nThree");
    lv_dropdown_open(dd);
    lv_obj_t * list = lv_dropdown_get_list(dd);
    int32_t sm = lv_display_dpx(harness_display(), 8);
    CHECK_INT(lv_obj_get_style_pad_left(list, LV_PART_MAIN), sm);
    CHECK_INT(lv_obj_get_style_pad_right(list, LV_PART_MAIN), sm);
    CHECK_INT(lv_obj_get_style_pad_top(list, LV_PART_MAIN), sm);
    CHECK_INT(lv_obj_get_style_pad_bottom(list, LV_PART_MAIN), sm);
    CHECK_INT(lv_obj_get_style_text_line_space(list, LV_PART_MAIN), lv_display_dpx(harness_display(), 12));
    CHECK_INT(lv_obj_get_style_radius(list, LV_PART_SELECTED), lv_display_dpx(harness_display(), 6));
    CHECK_INT(lv_obj_get_style_border_width(list, LV_PART_SELECTED), 0);
    lv_dropdown_close(dd);

    lv_theme_shadcn_init(harness_display(), dark(), harness_fonts(), LV_THEME_SHADCN_FLAG_COMPACT);
    lv_dropdown_open(dd);
    CHECK_INT(lv_obj_get_style_text_line_space(list, LV_PART_MAIN), lv_display_dpx(harness_display(), 8));
    CHECK_INT(lv_obj_get_style_pad_left(list, LV_PART_MAIN), lv_display_dpx(harness_display(), 4));
}

static void dropdown_trigger_has_no_scrollbar(void)
{
    lv_obj_t * dd = lv_dropdown_create(lv_screen_active());
    lv_dropdown_set_options_static(dd, "An option far wider than its dropdown");
    lv_obj_set_width(dd, 80);
    lv_obj_update_layout(dd);
    CHECK_INT(lv_obj_get_style_bg_opa(dd, LV_PART_SCROLLBAR), LV_OPA_TRANSP);
    lv_obj_t * ta = lv_textarea_create(lv_screen_active());
    CHECK_INT(lv_obj_get_style_bg_opa(ta, LV_PART_SCROLLBAR), LV_OPA_40);
}

static void dropdown_padding_matches_textarea(void)
{
    lv_obj_t * ta = lv_textarea_create(lv_screen_active());
    lv_obj_t * dd = lv_dropdown_create(lv_screen_active());
    int32_t pad = lv_display_dpx(harness_display(), 12);
    CHECK_INT(lv_obj_get_style_pad_left(ta, LV_PART_MAIN), pad);
    CHECK_INT(lv_obj_get_style_pad_right(ta, LV_PART_MAIN), pad);
    CHECK_INT(lv_obj_get_style_pad_left(dd, LV_PART_MAIN), pad);
    CHECK_INT(lv_obj_get_style_pad_right(dd, LV_PART_MAIN), pad);
}

static void roller_selected_row(void)
{
    lv_obj_t * r = lv_roller_create(lv_screen_active());
    CHECK_COLOR(lv_obj_get_style_bg_color(r, LV_PART_SELECTED), dark()->accent);
    CHECK_COLOR(lv_obj_get_style_text_color(r, LV_PART_SELECTED), dark()->accent_fg);
    CHECK_INT(lv_obj_get_style_text_align(r, LV_PART_MAIN), LV_TEXT_ALIGN_CENTER);
    CHECK(lv_obj_get_style_anim_duration(r, LV_PART_MAIN) > 0);
}

static void keyboard_keys(void)
{
    lv_obj_t * kb = lv_keyboard_create(lv_screen_active());
    CHECK_COLOR(lv_obj_get_style_bg_color(kb, LV_PART_MAIN), dark()->popover);
    CHECK_COLOR(lv_obj_get_style_bg_color(kb, LV_PART_ITEMS), dark()->secondary);
    CHECK_COLOR(lv_obj_get_style_text_color(kb, LV_PART_ITEMS), dark()->secondary_fg);
    lv_obj_add_state(kb, LV_STATE_CHECKED);
    CHECK_COLOR(lv_obj_get_style_bg_color(kb, LV_PART_ITEMS), dark()->input);
}

int main(void)
{
    RUN(textarea_input_box);
    RUN(textarea_error);
    RUN(dropdown_error_open);
    RUN(spinbox_matches_textarea);
    RUN(dropdown_and_list);
    RUN(dropdown_list_spaces_options);
    RUN(dropdown_trigger_has_no_scrollbar);
    RUN(dropdown_padding_matches_textarea);
    RUN(roller_selected_row);
    RUN(keyboard_keys);
    return harness_result();
}
