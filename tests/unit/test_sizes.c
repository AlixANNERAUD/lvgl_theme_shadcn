#include "harness.h"

static int32_t dp(int32_t v)
{
    return lv_display_dpx(harness_display(), v);
}

static int32_t touch_extra(lv_obj_t * obj)
{
    lv_obj_update_layout(obj);
    lv_area_t a;
    lv_obj_get_coords(obj, &a);
    int32_t y = (a.y1 + a.y2) / 2;
    int32_t extra = 0;
    for(int32_t e = 1; e < 40; e++) {
        lv_point_t p = { a.x1 - e, y };
        if(!lv_obj_hit_test(obj, &p)) break;
        extra = e;
    }
    return extra;
}

static lv_obj_t * labelled_button(void)
{
    lv_obj_t * btn = lv_button_create(lv_screen_active());
    lv_obj_set_pos(btn, 100, 100);
    lv_label_set_text(lv_label_create(btn), "Save");
    return btn;
}

static void small_button(void)
{
    lv_obj_t * btn = labelled_button();
    lv_theme_shadcn_set_size(btn, LV_THEME_SHADCN_SIZE_SM);
    lv_obj_update_layout(btn);
    CHECK_INT(lv_obj_get_height(btn), dp(32));
    CHECK_INT(lv_obj_get_style_pad_left(btn, LV_PART_MAIN), dp(12));
    CHECK_INT(lv_obj_get_style_pad_column(btn, LV_PART_MAIN), dp(6));
    CHECK(lv_obj_get_style_text_font(btn, LV_PART_MAIN) == harness_fonts()->small);
    CHECK(lv_obj_get_style_text_font(lv_obj_get_child(btn, 0), LV_PART_MAIN) == harness_fonts()->small);
    CHECK_INT(lv_obj_get_style_radius(btn, LV_PART_MAIN), dp(8));
    CHECK_INT(touch_extra(btn), (dp(48) - dp(32)) / 2);
}

static void large_button(void)
{
    lv_obj_t * btn = labelled_button();
    lv_theme_shadcn_set_size(btn, LV_THEME_SHADCN_SIZE_LG);
    lv_obj_update_layout(btn);
    CHECK_INT(lv_obj_get_height(btn), dp(48));
    CHECK_INT(lv_obj_get_style_pad_left(btn, LV_PART_MAIN), dp(24));
    CHECK(lv_obj_get_style_text_font(btn, LV_PART_MAIN) == harness_fonts()->normal);
    CHECK_INT(touch_extra(btn), 0);
}

static void icon_buttons_are_square(void)
{
    static const lv_theme_shadcn_size_t sizes[] = {
        LV_THEME_SHADCN_SIZE_ICON_SM, LV_THEME_SHADCN_SIZE_ICON, LV_THEME_SHADCN_SIZE_ICON_LG
    };
    static const int32_t expected[] = { 32, 40, 48 };
    for(int i = 0; i < 3; i++) {
        lv_obj_t * btn = lv_button_create(lv_screen_active());
        lv_obj_set_pos(btn, 100, 100);
        lv_label_set_text(lv_label_create(btn), LV_SYMBOL_SETTINGS);
        lv_theme_shadcn_set_size(btn, sizes[i]);
        lv_obj_update_layout(btn);
        CHECK_INT(lv_obj_get_width(btn), dp(expected[i]));
        CHECK_INT(lv_obj_get_height(btn), dp(expected[i]));
        CHECK_INT(lv_obj_get_style_pad_left(btn, LV_PART_MAIN), 0);
        CHECK_INT(touch_extra(btn), (dp(48) - dp(expected[i])) / 2);
        lv_area_t b;
        lv_area_t l;
        lv_obj_get_coords(btn, &b);
        lv_obj_get_coords(lv_obj_get_child(btn, 0), &l);
        int32_t dx = (b.x1 + b.x2) - (l.x1 + l.x2);
        int32_t dy = (b.y1 + b.y2) - (l.y1 + l.y2);
        CHECK(dx >= -2 && dx <= 2);
        CHECK(dy >= -2 && dy <= 2);
        lv_obj_delete(btn);
    }
}

static void sizes_replace_each_other(void)
{
    lv_obj_t * btn = labelled_button();
    lv_obj_update_layout(btn);
    int32_t content_w = lv_obj_get_width(btn);
    lv_theme_shadcn_set_size(btn, LV_THEME_SHADCN_SIZE_ICON_LG);
    lv_theme_shadcn_set_size(btn, LV_THEME_SHADCN_SIZE_SM);
    lv_obj_update_layout(btn);
    CHECK_INT(lv_obj_get_height(btn), dp(32));
    CHECK(lv_obj_get_width(btn) != dp(48));
    CHECK_INT(lv_obj_get_style_pad_left(btn, LV_PART_MAIN), dp(12));
    lv_theme_shadcn_set_size(btn, LV_THEME_SHADCN_SIZE_DEFAULT);
    lv_obj_update_layout(btn);
    CHECK_INT(lv_obj_get_height(btn), dp(40));
    CHECK_INT(lv_obj_get_width(btn), content_w);
    CHECK_INT(lv_obj_get_style_pad_left(btn, LV_PART_MAIN), dp(16));
    CHECK(lv_obj_get_style_text_font(btn, LV_PART_MAIN) == harness_fonts()->normal);
    CHECK_INT(touch_extra(btn), (dp(48) - dp(40)) / 2);
}

static void size_and_variant_are_independent(void)
{
    lv_obj_t * btn = labelled_button();
    lv_theme_shadcn_set_size(btn, LV_THEME_SHADCN_SIZE_SM);
    lv_theme_shadcn_set_variant(btn, LV_THEME_SHADCN_VARIANT_OUTLINE);
    lv_obj_update_layout(btn);
    CHECK_INT(lv_obj_get_height(btn), dp(32));
    CHECK_COLOR(lv_obj_get_style_border_color(btn, LV_PART_MAIN), lv_theme_shadcn_palette_zinc_dark()->input);
    lv_theme_shadcn_set_size(btn, LV_THEME_SHADCN_SIZE_LG);
    CHECK_INT(lv_obj_get_style_border_width(btn, LV_PART_MAIN), dp(1));
}

static void size_survives_rebuilds(void)
{
    lv_obj_t * btn = labelled_button();
    lv_theme_shadcn_set_size(btn, LV_THEME_SHADCN_SIZE_SM);
    lv_theme_shadcn_set_palette(harness_display(), lv_theme_shadcn_palette_zinc_light());
    lv_theme_shadcn_set_radius(harness_display(), LV_THEME_SHADCN_RADIUS_NONE);
    lv_obj_update_layout(btn);
    CHECK_INT(lv_obj_get_height(btn), dp(32));
    CHECK_INT(lv_obj_get_style_radius(btn, LV_PART_MAIN), 0);
}

static void dropdown_takes_sizes(void)
{
    lv_obj_t * dd = lv_dropdown_create(lv_screen_active());
    lv_theme_shadcn_set_size(dd, LV_THEME_SHADCN_SIZE_SM);
    lv_obj_update_layout(dd);
    CHECK_INT(lv_obj_get_height(dd), dp(32));
    lv_theme_shadcn_set_size(dd, LV_THEME_SHADCN_SIZE_LG);
    lv_obj_update_layout(dd);
    CHECK_INT(lv_obj_get_height(dd), dp(48));
}

static void invalid_size_and_foreign_display_are_ignored(void)
{
    lv_obj_t * btn = labelled_button();
    lv_theme_shadcn_set_size(btn, (lv_theme_shadcn_size_t)42);
    lv_theme_shadcn_set_size(NULL, LV_THEME_SHADCN_SIZE_SM);
    lv_obj_update_layout(btn);
    CHECK_INT(lv_obj_get_height(btn), dp(40));
    lv_display_t * other = lv_display_create(100, 100);
    lv_obj_t * foreign = lv_button_create(lv_display_get_screen_active(other));
    lv_theme_shadcn_set_size(foreign, LV_THEME_SHADCN_SIZE_ICON);
    lv_obj_update_layout(foreign);
    CHECK(lv_obj_get_height(foreign) != dp(40) || lv_obj_get_width(foreign) != dp(40));
    lv_display_delete(other);
}

static void compact_sizes_shrink(void)
{
    lv_init();
    lv_display_t * disp = harness_create_display();
    lv_theme_shadcn_init(disp, lv_theme_shadcn_palette_zinc_dark(), harness_fonts(), LV_THEME_SHADCN_FLAG_COMPACT);
    static const lv_theme_shadcn_size_t sizes[] = {
        LV_THEME_SHADCN_SIZE_SM, LV_THEME_SHADCN_SIZE_DEFAULT, LV_THEME_SHADCN_SIZE_LG, LV_THEME_SHADCN_SIZE_ICON_SM
    };
    static const int32_t expected[] = { 24, 32, 40, 24 };
    for(int i = 0; i < 4; i++) {
        lv_obj_t * btn = lv_button_create(lv_screen_active());
        lv_obj_set_pos(btn, 100, 100);
        lv_label_set_text(lv_label_create(btn), "Go");
        lv_theme_shadcn_set_size(btn, sizes[i]);
        lv_obj_update_layout(btn);
        CHECK_INT(lv_obj_get_height(btn), lv_display_dpx(disp, expected[i]));
        CHECK_INT(touch_extra(btn), (lv_display_dpx(disp, 40) - lv_display_dpx(disp, expected[i])) / 2);
        lv_obj_delete(btn);
    }
    lv_deinit();
}

int main(void)
{
    RUN(small_button);
    RUN(large_button);
    RUN(icon_buttons_are_square);
    RUN(sizes_replace_each_other);
    RUN(size_and_variant_are_independent);
    RUN(size_survives_rebuilds);
    RUN(dropdown_takes_sizes);
    RUN(invalid_size_and_foreign_display_are_ignored);
    RUN_RAW(compact_sizes_shrink);
    return harness_result();
}
