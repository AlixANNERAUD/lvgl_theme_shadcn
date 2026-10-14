#include "harness.h"

static const lv_theme_shadcn_palette_t * dark(void)
{
    return lv_theme_shadcn_palette_zinc_dark();
}

static void slider_parts(void)
{
    lv_obj_t * s = lv_slider_create(lv_screen_active());
    CHECK_COLOR(lv_obj_get_style_bg_color(s, LV_PART_MAIN), dark()->secondary);
    CHECK_COLOR(lv_obj_get_style_bg_color(s, LV_PART_INDICATOR), dark()->primary);
    CHECK_COLOR(lv_obj_get_style_bg_color(s, LV_PART_KNOB), dark()->background);
    CHECK_COLOR(lv_obj_get_style_border_color(s, LV_PART_KNOB), dark()->primary);
    CHECK_INT(lv_obj_get_style_border_width(s, LV_PART_KNOB), lv_display_dpx(harness_display(), 2));
}

static void bar_parts(void)
{
    lv_obj_t * b = lv_bar_create(lv_screen_active());
    CHECK_COLOR(lv_obj_get_style_bg_color(b, LV_PART_MAIN), dark()->secondary);
    CHECK_COLOR(lv_obj_get_style_bg_color(b, LV_PART_INDICATOR), dark()->primary);
}

static void bar_disabled_dims(void)
{
    lv_obj_t * b = lv_bar_create(lv_screen_active());
    lv_obj_add_state(b, LV_STATE_DISABLED);
    harness_settle();
    CHECK_INT(lv_obj_get_style_opa(b, LV_PART_MAIN), 97);
}

static void arc_parts(void)
{
    lv_obj_t * a = lv_arc_create(lv_screen_active());
    CHECK_COLOR(lv_obj_get_style_arc_color(a, LV_PART_MAIN), dark()->secondary);
    CHECK_COLOR(lv_obj_get_style_arc_color(a, LV_PART_INDICATOR), dark()->primary);
    CHECK(lv_obj_get_style_arc_rounded(a, LV_PART_INDICATOR));
    CHECK_INT(lv_obj_get_style_arc_width(a, LV_PART_MAIN), lv_display_dpx(harness_display(), 8));
    CHECK_INT(lv_obj_get_style_arc_width(a, LV_PART_INDICATOR), lv_display_dpx(harness_display(), 8));
}

static void spinner_parts(void)
{
    lv_obj_t * sp = lv_spinner_create(lv_screen_active());
    CHECK_COLOR(lv_obj_get_style_arc_color(sp, LV_PART_MAIN), dark()->secondary);
    CHECK_COLOR(lv_obj_get_style_arc_color(sp, LV_PART_INDICATOR), dark()->primary);
    CHECK_INT(lv_obj_get_style_arc_width(sp, LV_PART_INDICATOR), lv_display_dpx(harness_display(), 8));
}

static void scale_parts(void)
{
    lv_obj_t * sc = lv_scale_create(lv_screen_active());
    CHECK_COLOR(lv_obj_get_style_line_color(sc, LV_PART_MAIN), dark()->border);
    CHECK_COLOR(lv_obj_get_style_line_color(sc, LV_PART_INDICATOR), dark()->muted_fg);
    CHECK_COLOR(lv_obj_get_style_text_color(sc, LV_PART_INDICATOR), dark()->muted_fg);
    CHECK_COLOR(lv_obj_get_style_line_color(sc, LV_PART_ITEMS), dark()->border);
    CHECK(lv_obj_get_style_line_width(sc, LV_PART_MAIN) > 0);
    CHECK(lv_obj_get_style_line_width(sc, LV_PART_ITEMS) > 0);
    CHECK_INT(lv_obj_get_style_length(sc, LV_PART_ITEMS), lv_display_dpx(harness_display(), 4));
    CHECK(lv_obj_get_style_length(sc, LV_PART_ITEMS) > 0);
    CHECK(lv_obj_get_style_length(sc, LV_PART_INDICATOR) > lv_obj_get_style_length(sc, LV_PART_ITEMS));
}

static void slider_disabled_dims(void)
{
    lv_obj_t * s = lv_slider_create(lv_screen_active());
    lv_obj_add_state(s, LV_STATE_DISABLED);
    harness_settle();
    CHECK_INT(lv_obj_get_style_opa(s, LV_PART_MAIN), 97);
}

int main(void)
{
    RUN(slider_parts);
    RUN(bar_parts);
    RUN(bar_disabled_dims);
    RUN(arc_parts);
    RUN(spinner_parts);
    RUN(scale_parts);
    RUN(slider_disabled_dims);
    return harness_result();
}
