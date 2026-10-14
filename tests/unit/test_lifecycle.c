#include "harness.h"

static void init_activates_theme_and_styles_screen(void)
{
    const lv_theme_shadcn_palette_t * p = lv_theme_shadcn_palette_zinc_dark();
    lv_obj_t * scr = lv_obj_create(NULL);
    CHECK(lv_theme_shadcn_is_active(harness_display()));
    CHECK_COLOR(lv_obj_get_style_bg_color(scr, LV_PART_MAIN), p->background);
    CHECK_INT(lv_obj_get_style_bg_opa(scr, LV_PART_MAIN), LV_OPA_COVER);
    CHECK_COLOR(lv_obj_get_style_text_color(scr, LV_PART_MAIN), p->foreground);
    CHECK(lv_obj_get_style_text_font(scr, LV_PART_MAIN) == &lv_font_montserrat_14);
}

static void get_palette_returns_copy_of_active_palette(void)
{
    const lv_theme_shadcn_palette_t * p = lv_theme_shadcn_get_palette(harness_display());
    CHECK(p != NULL);
    CHECK(p != lv_theme_shadcn_palette_zinc_dark());
    CHECK_COLOR(p->primary, lv_theme_shadcn_palette_zinc_dark()->primary);
}

static void set_palette_restyles_existing_screen(void)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_theme_shadcn_set_palette(harness_display(), lv_theme_shadcn_palette_zinc_light());
    CHECK_COLOR(lv_obj_get_style_bg_color(scr, LV_PART_MAIN), lv_color_hex(0xffffff));
    CHECK_COLOR(lv_obj_get_style_text_color(scr, LV_PART_MAIN), lv_color_hex(0x09090b));
}

static void null_fonts_fall_back_to_default(void)
{
    lv_init();
    lv_display_t * disp = harness_create_display();
    lv_theme_shadcn_init(disp, lv_theme_shadcn_palette_zinc_dark(), NULL, LV_THEME_SHADCN_FLAG_NONE);
    lv_obj_t * scr = lv_obj_create(NULL);
    CHECK(lv_obj_get_style_text_font(scr, LV_PART_MAIN) == LV_FONT_DEFAULT);
    lv_deinit();
}

static void partial_fonts_fall_back_to_normal(void)
{
    lv_init();
    lv_display_t * disp = harness_create_display();
    lv_theme_shadcn_fonts_t fonts = { NULL, &lv_font_montserrat_20, NULL, NULL };
    lv_theme_shadcn_init(disp, lv_theme_shadcn_palette_zinc_dark(), &fonts, LV_THEME_SHADCN_FLAG_NONE);
    lv_obj_t * scr = lv_obj_create(NULL);
    CHECK(lv_obj_get_style_text_font(scr, LV_PART_MAIN) == &lv_font_montserrat_20);
    lv_deinit();
}

static void deinit_restores_previous_theme(void)
{
    lv_init();
    lv_display_t * disp = harness_create_display();
    lv_theme_t * before = lv_display_get_theme(disp);
    lv_theme_shadcn_init(disp, lv_theme_shadcn_palette_zinc_dark(), harness_fonts(), LV_THEME_SHADCN_FLAG_NONE);
    lv_theme_shadcn_deinit(disp);
    CHECK(!lv_theme_shadcn_is_active(disp));
    CHECK(lv_display_get_theme(disp) == before);
    CHECK(lv_theme_shadcn_get_palette(disp) == NULL);
    lv_deinit();
}

static void init_twice_reuses_state(void)
{
    lv_theme_t * a = lv_display_get_theme(harness_display());
    lv_theme_t * b = lv_theme_shadcn_init(harness_display(), lv_theme_shadcn_palette_zinc_light(), harness_fonts(), LV_THEME_SHADCN_FLAG_NONE);
    CHECK(a == b);
    CHECK_COLOR(lv_theme_shadcn_get_palette(harness_display())->background, lv_color_hex(0xffffff));
}

static void display_delete_frees_state(void)
{
    lv_init();
    lv_display_t * keep = harness_create_display();
    lv_display_t * gone = lv_display_create(100, 100);
    lv_theme_shadcn_init(gone, lv_theme_shadcn_palette_zinc_dark(), NULL, LV_THEME_SHADCN_FLAG_NONE);
    lv_display_delete(gone);
    lv_display_t * fresh = lv_display_create(100, 100);
    CHECK(!lv_theme_shadcn_is_active(fresh));
    CHECK(lv_theme_shadcn_get_palette(fresh) == NULL);
    CHECK(!lv_theme_shadcn_is_active(keep));
    lv_deinit();
}

static void resolution_change_rescales_metrics(void)
{
    lv_obj_t * btn = lv_obj_create(lv_screen_active());
    int32_t before = lv_obj_get_style_radius(btn, LV_PART_MAIN);
    lv_display_set_dpi(harness_display(), 320);
    lv_display_set_resolution(harness_display(), 320, 240);
    CHECK_INT(lv_obj_get_style_radius(btn, LV_PART_MAIN), before * 2);
}

static void compact_flag_shrinks_spacing(void)
{
    lv_init();
    lv_display_t * disp = harness_create_display();
    lv_theme_shadcn_init(disp, lv_theme_shadcn_palette_zinc_dark(), NULL, LV_THEME_SHADCN_FLAG_COMPACT);
    lv_obj_t * card = lv_obj_create(lv_screen_active());
    CHECK_INT(lv_obj_get_style_pad_left(card, LV_PART_MAIN), lv_display_dpx(disp, 12));
    lv_deinit();
}

static void init_styles_existing_active_screen(void)
{
    CHECK_COLOR(lv_obj_get_style_bg_color(lv_screen_active(), LV_PART_MAIN), lv_theme_shadcn_palette_zinc_dark()->background);
}

static void init_leaves_system_layers_transparent(void)
{
    CHECK(lv_obj_get_style_bg_opa(lv_layer_top(), LV_PART_MAIN) == LV_OPA_TRANSP);
    CHECK(lv_obj_get_style_bg_opa(lv_layer_sys(), LV_PART_MAIN) == LV_OPA_TRANSP);
    CHECK(lv_obj_get_style_bg_opa(lv_screen_active(), LV_PART_MAIN) == LV_OPA_COVER);
}

static void churn_heap(void)
{
    for(int i = 0; i < 16; i++) {
        lv_obj_t * o = lv_obj_create(NULL);
        lv_obj_t * c = lv_label_create(o);
        lv_label_set_text(c, "churn");
        lv_obj_delete(o);
    }
    lv_tick_inc(50);
    lv_timer_handler();
}

static void deinit_with_live_objects_detaches_styles(void)
{
    lv_init();
    lv_display_t * disp = harness_create_display();
    lv_theme_shadcn_init(disp, lv_theme_shadcn_palette_zinc_dark(), harness_fonts(), LV_THEME_SHADCN_FLAG_NONE);
    lv_obj_t * parent = lv_obj_create(lv_screen_active());
    lv_obj_t * child = lv_obj_create(parent);
    lv_color_t themed = lv_theme_shadcn_palette_zinc_dark()->background;
    CHECK_COLOR(lv_obj_get_style_bg_color(lv_screen_active(), LV_PART_MAIN), themed);
    lv_theme_shadcn_deinit(disp);
    churn_heap();
    CHECK(!lv_color_eq(lv_obj_get_style_bg_color(lv_screen_active(), LV_PART_MAIN), themed));
    CHECK(!lv_color_eq(lv_obj_get_style_bg_color(parent, LV_PART_MAIN), themed));
    CHECK(!lv_color_eq(lv_obj_get_style_bg_color(child, LV_PART_MAIN), themed));
    lv_deinit();
}

static void display_delete_with_live_objects_is_safe(void)
{
    lv_init();
    lv_display_t * keep = harness_create_display();
    lv_display_t * gone = lv_display_create(100, 100);
    lv_theme_shadcn_init(gone, lv_theme_shadcn_palette_zinc_dark(), NULL, LV_THEME_SHADCN_FLAG_NONE);
    lv_obj_t * o = lv_obj_create(lv_display_get_screen_active(gone));
    lv_obj_create(o);
    lv_display_delete(gone);
    churn_heap();
    CHECK(!lv_theme_shadcn_is_active(keep));
    lv_deinit();
}

int main(void)
{
    RUN(init_activates_theme_and_styles_screen);
    RUN(init_styles_existing_active_screen);
    RUN(init_leaves_system_layers_transparent);
    RUN_RAW(deinit_with_live_objects_detaches_styles);
    RUN_RAW(display_delete_with_live_objects_is_safe);
    RUN(get_palette_returns_copy_of_active_palette);
    RUN(set_palette_restyles_existing_screen);
    RUN_RAW(null_fonts_fall_back_to_default);
    RUN_RAW(partial_fonts_fall_back_to_normal);
    RUN_RAW(deinit_restores_previous_theme);
    RUN(init_twice_reuses_state);
    RUN_RAW(display_delete_frees_state);
    RUN(resolution_change_rescales_metrics);
    RUN_RAW(compact_flag_shrinks_spacing);
    return harness_result();
}
