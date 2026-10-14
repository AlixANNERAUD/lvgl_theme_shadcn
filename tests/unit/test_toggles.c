#include "harness.h"

static const lv_theme_shadcn_palette_t * dark(void)
{
    return lv_theme_shadcn_palette_zinc_dark();
}

static void switch_track_and_knob(void)
{
    lv_obj_t * sw = lv_switch_create(lv_screen_active());
    CHECK_COLOR(lv_obj_get_style_bg_color(sw, LV_PART_MAIN), dark()->input);
    CHECK_INT(lv_obj_get_style_radius(sw, LV_PART_MAIN), LV_RADIUS_CIRCLE);
    CHECK_COLOR(lv_obj_get_style_bg_color(sw, LV_PART_KNOB), dark()->foreground);
    lv_obj_add_state(sw, LV_STATE_CHECKED);
    harness_settle();
    CHECK_COLOR(lv_obj_get_style_bg_color(sw, LV_PART_INDICATOR), dark()->primary);
    CHECK_INT(lv_obj_get_style_bg_opa(sw, LV_PART_INDICATOR), LV_OPA_COVER);
    CHECK_COLOR(lv_obj_get_style_bg_color(sw, LV_PART_KNOB), dark()->primary_fg);
}

static void switch_knob_light_palette(void)
{
    lv_obj_t * sw = lv_switch_create(lv_screen_active());
    lv_theme_shadcn_set_palette(harness_display(), lv_theme_shadcn_palette_zinc_light());
    CHECK_COLOR(lv_obj_get_style_bg_color(sw, LV_PART_KNOB), lv_color_hex(0xffffff));
}

static void checkbox_marker(void)
{
    lv_obj_t * cb = lv_checkbox_create(lv_screen_active());
    CHECK_COLOR(lv_obj_get_style_border_color(cb, LV_PART_INDICATOR), dark()->input);
    CHECK_COLOR(lv_obj_get_style_bg_color(cb, LV_PART_INDICATOR), dark()->background);
    CHECK_INT(lv_obj_get_style_radius(cb, LV_PART_INDICATOR), lv_display_dpx(harness_display(), 6));
    CHECK(lv_obj_get_style_bg_image_src(cb, LV_PART_INDICATOR) == NULL);
    lv_obj_add_state(cb, LV_STATE_CHECKED);
    harness_settle();
    CHECK_COLOR(lv_obj_get_style_bg_color(cb, LV_PART_INDICATOR), dark()->primary);
    CHECK_COLOR(lv_obj_get_style_border_color(cb, LV_PART_INDICATOR), dark()->primary);
    CHECK_COLOR(lv_obj_get_style_text_color(cb, LV_PART_INDICATOR), dark()->primary_fg);
    CHECK(lv_obj_get_style_bg_image_src(cb, LV_PART_INDICATOR) != NULL);
}

static void checkbox_focus_ring_on_main(void)
{
    lv_obj_t * cb = lv_checkbox_create(lv_screen_active());
    lv_obj_add_state(cb, LV_STATE_FOCUS_KEY);
    CHECK_COLOR(lv_obj_get_style_outline_color(cb, LV_PART_MAIN), dark()->ring);
}

static void led_is_flat_primary(void)
{
    lv_obj_t * led = lv_led_create(lv_screen_active());
    CHECK_INT(lv_obj_get_style_shadow_width(led, LV_PART_MAIN), 0);
    CHECK_INT(lv_obj_get_style_radius(led, LV_PART_MAIN), LV_RADIUS_CIRCLE);
    CHECK_COLOR(lv_led_get_color(led), dark()->primary);
}

int main(void)
{
    RUN(switch_track_and_knob);
    RUN(switch_knob_light_palette);
    RUN(checkbox_marker);
    RUN(checkbox_focus_ring_on_main);
    RUN(led_is_flat_primary);
    return harness_result();
}
