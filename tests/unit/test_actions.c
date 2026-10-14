#include "harness.h"

static const lv_theme_shadcn_palette_t * dark(void)
{
    return lv_theme_shadcn_palette_zinc_dark();
}

static lv_color_t pressed(lv_color_t bg, lv_color_t fg)
{
    return lv_color_mix(fg, bg, 26);
}

static void button_is_primary(void)
{
    lv_obj_t * btn = lv_button_create(lv_screen_active());
    CHECK_COLOR(lv_obj_get_style_bg_color(btn, LV_PART_MAIN), dark()->primary);
    CHECK_COLOR(lv_obj_get_style_text_color(btn, LV_PART_MAIN), dark()->primary_fg);
    CHECK_INT(lv_obj_get_style_min_height(btn, LV_PART_MAIN), lv_display_dpx(harness_display(), 40));
    CHECK_INT(lv_obj_get_style_radius(btn, LV_PART_MAIN), lv_display_dpx(harness_display(), 8));
    CHECK_INT(lv_obj_get_style_shadow_width(btn, LV_PART_MAIN), 0);
}

static void button_pressed_mixes_text_color(void)
{
    lv_obj_t * btn = lv_button_create(lv_screen_active());
    lv_obj_add_state(btn, LV_STATE_PRESSED);
    harness_settle();
    CHECK_COLOR(lv_obj_get_style_bg_color(btn, LV_PART_MAIN), pressed(dark()->primary, dark()->primary_fg));
}

static void button_touch_target_is_48dp(void)
{
    lv_obj_t * btn = lv_button_create(lv_screen_active());
    lv_obj_set_pos(btn, 100, 100);
    lv_obj_update_layout(btn);
    lv_area_t a;
    lv_obj_get_coords(btn, &a);
    int32_t extra = (lv_display_dpx(harness_display(), 48) - lv_display_dpx(harness_display(), 40)) / 2;
    CHECK(extra > 1);
    lv_point_t inside = { a.x1 - extra + 1, a.y1 + 1 };
    lv_point_t outside = { a.x1 - extra - 1, a.y1 + 1 };
    CHECK(lv_obj_hit_test(btn, &inside));
    CHECK(!lv_obj_hit_test(btn, &outside));
}

static void variants_replace_each_other(void)
{
    lv_obj_t * btn = lv_button_create(lv_screen_active());
    lv_theme_shadcn_set_variant(btn, LV_THEME_SHADCN_VARIANT_OUTLINE);
    CHECK_INT(lv_obj_get_style_bg_opa(btn, LV_PART_MAIN), LV_OPA_TRANSP);
    CHECK_COLOR(lv_obj_get_style_border_color(btn, LV_PART_MAIN), dark()->input);
    CHECK_INT(lv_obj_get_style_border_width(btn, LV_PART_MAIN), lv_display_dpx(harness_display(), 1));
    lv_theme_shadcn_set_variant(btn, LV_THEME_SHADCN_VARIANT_GHOST);
    lv_theme_shadcn_set_variant(btn, LV_THEME_SHADCN_VARIANT_SECONDARY);
    CHECK_COLOR(lv_obj_get_style_bg_color(btn, LV_PART_MAIN), dark()->secondary);
    CHECK_INT(lv_obj_get_style_bg_opa(btn, LV_PART_MAIN), LV_OPA_COVER);
    CHECK_INT(lv_obj_get_style_border_width(btn, LV_PART_MAIN), 0);
    lv_obj_add_state(btn, LV_STATE_PRESSED);
    harness_settle();
    CHECK_COLOR(lv_obj_get_style_bg_color(btn, LV_PART_MAIN), pressed(dark()->secondary, dark()->secondary_fg));
}

static void ghost_pressed_uses_accent(void)
{
    lv_obj_t * btn = lv_button_create(lv_screen_active());
    lv_theme_shadcn_set_variant(btn, LV_THEME_SHADCN_VARIANT_GHOST);
    lv_obj_add_state(btn, LV_STATE_PRESSED);
    harness_settle();
    CHECK_COLOR(lv_obj_get_style_bg_color(btn, LV_PART_MAIN), dark()->accent);
    CHECK_INT(lv_obj_get_style_bg_opa(btn, LV_PART_MAIN), LV_OPA_COVER);
}

static void destructive_variant(void)
{
    lv_obj_t * btn = lv_button_create(lv_screen_active());
    lv_theme_shadcn_set_variant(btn, LV_THEME_SHADCN_VARIANT_DESTRUCTIVE);
    CHECK_COLOR(lv_obj_get_style_bg_color(btn, LV_PART_MAIN), dark()->destructive);
    CHECK_COLOR(lv_obj_get_style_text_color(btn, LV_PART_MAIN), dark()->destructive_fg);
}

static void helpers_ignore_foreign_display(void)
{
    lv_display_t * other = lv_display_create(100, 100);
    lv_obj_t * scr = lv_display_get_screen_active(other);
    lv_obj_t * btn = lv_button_create(scr);
    lv_theme_shadcn_set_variant(btn, LV_THEME_SHADCN_VARIANT_OUTLINE);
    lv_theme_shadcn_set_surface(btn, LV_THEME_SHADCN_SURFACE_CARD);
    lv_theme_shadcn_set_error(btn, true);
    lv_theme_shadcn_set_variant(NULL, LV_THEME_SHADCN_VARIANT_OUTLINE);
    CHECK(!lv_color_eq(lv_obj_get_style_border_color(btn, LV_PART_MAIN), dark()->destructive));
    lv_display_delete(other);
}

static void buttonmatrix_items_are_outline(void)
{
    static const char * map[] = { "A", "B", "" };
    lv_obj_t * bm = lv_buttonmatrix_create(lv_screen_active());
    lv_buttonmatrix_set_map(bm, map);
    CHECK_INT(lv_obj_get_style_bg_opa(bm, LV_PART_MAIN), LV_OPA_TRANSP);
    CHECK_COLOR(lv_obj_get_style_border_color(bm, LV_PART_ITEMS), dark()->input);
    lv_obj_add_state(bm, LV_STATE_CHECKED);
    CHECK_COLOR(lv_obj_get_style_bg_color(bm, LV_PART_ITEMS), dark()->accent);
}

static void button_label_is_centred(void)
{
    lv_obj_t * btn = lv_button_create(lv_screen_active());
    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "Save");
    lv_obj_update_layout(btn);
    lv_area_t b;
    lv_area_t l;
    lv_obj_get_coords(btn, &b);
    lv_obj_get_coords(label, &l);
    int32_t diff = (b.y1 + b.y2) - (l.y1 + l.y2);
    CHECK(diff >= -2 && diff <= 2);
}

static void variant_change_keeps_item_styles(void)
{
    static const char * map[] = { "A", "B", "" };
    lv_obj_t * bm = lv_buttonmatrix_create(lv_screen_active());
    lv_buttonmatrix_set_map(bm, map);
    lv_theme_shadcn_set_variant(bm, LV_THEME_SHADCN_VARIANT_DEFAULT);
    CHECK_COLOR(lv_obj_get_style_border_color(bm, LV_PART_ITEMS), dark()->input);
}

int main(void)
{
    RUN(button_is_primary);
    RUN(button_pressed_mixes_text_color);
    RUN(button_touch_target_is_48dp);
    RUN(variants_replace_each_other);
    RUN(ghost_pressed_uses_accent);
    RUN(destructive_variant);
    RUN(helpers_ignore_foreign_display);
    RUN(buttonmatrix_items_are_outline);
    RUN(button_label_is_centred);
    RUN(variant_change_keeps_item_styles);
    return harness_result();
}
