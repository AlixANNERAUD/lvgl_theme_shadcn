#include "harness.h"

static int32_t dp(int32_t v)
{
    return lv_display_dpx(harness_display(), v);
}

static void default_radius_matches_shadcn(void)
{
    CHECK_INT(lv_theme_shadcn_get_radius(harness_display()), LV_THEME_SHADCN_RADIUS_DEFAULT);
    lv_obj_t * card = lv_obj_create(lv_screen_active());
    lv_obj_t * btn = lv_button_create(lv_screen_active());
    lv_obj_t * cb = lv_checkbox_create(lv_screen_active());
    lv_obj_t * ta = lv_textarea_create(lv_screen_active());
    lv_obj_t * mb = lv_msgbox_create(NULL);
    CHECK_INT(lv_obj_get_style_radius(card, LV_PART_MAIN), dp(14));
    CHECK_INT(lv_obj_get_style_radius(btn, LV_PART_MAIN), dp(8));
    CHECK_INT(lv_obj_get_style_radius(cb, LV_PART_INDICATOR), dp(6));
    CHECK_INT(lv_obj_get_style_radius(ta, LV_PART_MAIN), dp(8));
    CHECK_INT(lv_obj_get_style_radius(mb, LV_PART_MAIN), dp(10));
}

static void set_radius_restyles_existing_objects(void)
{
    lv_obj_t * card = lv_obj_create(lv_screen_active());
    lv_obj_t * btn = lv_button_create(lv_screen_active());
    lv_obj_t * cb = lv_checkbox_create(lv_screen_active());
    lv_obj_t * mb = lv_msgbox_create(NULL);

    lv_theme_shadcn_set_radius(harness_display(), LV_THEME_SHADCN_RADIUS_NONE);
    CHECK_INT(lv_theme_shadcn_get_radius(harness_display()), LV_THEME_SHADCN_RADIUS_NONE);
    CHECK_INT(lv_obj_get_style_radius(card, LV_PART_MAIN), 0);
    CHECK_INT(lv_obj_get_style_radius(btn, LV_PART_MAIN), 0);
    CHECK_INT(lv_obj_get_style_radius(cb, LV_PART_INDICATOR), 0);
    CHECK_INT(lv_obj_get_style_radius(mb, LV_PART_MAIN), 0);

    lv_theme_shadcn_set_radius(harness_display(), LV_THEME_SHADCN_RADIUS_XL);
    CHECK_INT(lv_theme_shadcn_get_radius(harness_display()), LV_THEME_SHADCN_RADIUS_XL);
    CHECK_INT(lv_obj_get_style_radius(card, LV_PART_MAIN), dp(22));
    CHECK_INT(lv_obj_get_style_radius(btn, LV_PART_MAIN), dp(13));
    CHECK_INT(lv_obj_get_style_radius(cb, LV_PART_INDICATOR), dp(10));
    CHECK_INT(lv_obj_get_style_radius(mb, LV_PART_MAIN), dp(16));

    lv_theme_shadcn_set_radius(harness_display(), LV_THEME_SHADCN_RADIUS_SM);
    CHECK_INT(lv_obj_get_style_radius(btn, LV_PART_MAIN), dp(4));
    CHECK_INT(lv_obj_get_style_radius(mb, LV_PART_MAIN), dp(5));

    lv_theme_shadcn_set_radius(harness_display(), LV_THEME_SHADCN_RADIUS_DEFAULT);
    CHECK_INT(lv_obj_get_style_radius(btn, LV_PART_MAIN), dp(8));
    CHECK_INT(lv_obj_get_style_radius(card, LV_PART_MAIN), dp(14));
}

static void every_radius_is_ordered(void)
{
    lv_obj_t * card = lv_obj_create(lv_screen_active());
    lv_obj_t * btn = lv_button_create(lv_screen_active());
    lv_obj_t * cb = lv_checkbox_create(lv_screen_active());
    int32_t previous = -1;
    for(int r = LV_THEME_SHADCN_RADIUS_NONE; r <= LV_THEME_SHADCN_RADIUS_XL; r++) {
        lv_theme_shadcn_set_radius(harness_display(), (lv_theme_shadcn_radius_t)r);
        int32_t sm = lv_obj_get_style_radius(cb, LV_PART_INDICATOR);
        int32_t md = lv_obj_get_style_radius(btn, LV_PART_MAIN);
        int32_t xl = lv_obj_get_style_radius(card, LV_PART_MAIN);
        CHECK(sm >= 0);
        CHECK(sm <= md);
        CHECK(md <= xl);
        CHECK(md > previous || (r == 0 && md == 0));
        previous = md;
    }
}

static void radius_survives_palette_and_resolution_changes(void)
{
    lv_obj_t * btn = lv_button_create(lv_screen_active());
    lv_theme_shadcn_set_radius(harness_display(), LV_THEME_SHADCN_RADIUS_LG);
    lv_theme_shadcn_set_palette(harness_display(), lv_theme_shadcn_palette_zinc_light());
    CHECK_INT(lv_theme_shadcn_get_radius(harness_display()), LV_THEME_SHADCN_RADIUS_LG);
    CHECK_INT(lv_obj_get_style_radius(btn, LV_PART_MAIN), dp(10));
    lv_display_set_dpi(harness_display(), 320);
    lv_display_set_resolution(harness_display(), 320, 240);
    CHECK_INT(lv_obj_get_style_radius(btn, LV_PART_MAIN), 19);
    lv_theme_shadcn_init(harness_display(), lv_theme_shadcn_palette_zinc_dark(), harness_fonts(), LV_THEME_SHADCN_FLAG_NONE);
    CHECK_INT(lv_theme_shadcn_get_radius(harness_display()), LV_THEME_SHADCN_RADIUS_LG);
}

static void invalid_radius_and_missing_theme_are_ignored(void)
{
    lv_obj_t * btn = lv_button_create(lv_screen_active());
    lv_theme_shadcn_set_radius(harness_display(), (lv_theme_shadcn_radius_t)42);
    lv_theme_shadcn_set_radius(harness_display(), (lv_theme_shadcn_radius_t) -1);
    CHECK_INT(lv_theme_shadcn_get_radius(harness_display()), LV_THEME_SHADCN_RADIUS_DEFAULT);
    CHECK_INT(lv_obj_get_style_radius(btn, LV_PART_MAIN), dp(8));
    lv_display_t * other = lv_display_create(100, 100);
    lv_theme_shadcn_set_radius(other, LV_THEME_SHADCN_RADIUS_NONE);
    CHECK_INT(lv_theme_shadcn_get_radius(other), LV_THEME_SHADCN_RADIUS_DEFAULT);
    CHECK_INT(lv_obj_get_style_radius(btn, LV_PART_MAIN), dp(8));
    lv_display_delete(other);
}

int main(void)
{
    RUN(default_radius_matches_shadcn);
    RUN(set_radius_restyles_existing_objects);
    RUN(every_radius_is_ordered);
    RUN(radius_survives_palette_and_resolution_changes);
    RUN(invalid_radius_and_missing_theme_are_ignored);
    return harness_result();
}
