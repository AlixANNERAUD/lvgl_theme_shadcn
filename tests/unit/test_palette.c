#include <string.h>
#include "harness.h"

static void zinc_dark_matches_spec(void)
{
    const lv_theme_shadcn_palette_t * p = lv_theme_shadcn_palette_zinc_dark();
    CHECK(p->dark);
    CHECK_COLOR(p->background, lv_color_hex(0x09090b));
    CHECK_COLOR(p->foreground, lv_color_hex(0xfafafa));
    CHECK_COLOR(p->card, lv_color_hex(0x18181b));
    CHECK_COLOR(p->primary, lv_color_hex(0xe4e4e7));
    CHECK_COLOR(p->primary_fg, lv_color_hex(0x18181b));
    CHECK_COLOR(p->muted_fg, lv_color_hex(0x9f9fa9));
    CHECK_COLOR(p->destructive, lv_color_hex(0xff6467));
    CHECK_COLOR(p->border, lv_color_hex(0x27272a));
    CHECK_COLOR(p->input, lv_color_hex(0x3f3f46));
    CHECK_COLOR(p->ring, lv_color_hex(0x71717b));
    CHECK_COLOR(p->chart[1], lv_color_hex(0x71717b));
}

static void zinc_light_matches_spec(void)
{
    const lv_theme_shadcn_palette_t * p = lv_theme_shadcn_palette_zinc_light();
    CHECK(!p->dark);
    CHECK_COLOR(p->background, lv_color_hex(0xffffff));
    CHECK_COLOR(p->foreground, lv_color_hex(0x09090b));
    CHECK_COLOR(p->primary, lv_color_hex(0x18181b));
    CHECK_COLOR(p->secondary, lv_color_hex(0xf4f4f5));
    CHECK_COLOR(p->muted_fg, lv_color_hex(0x71717b));
    CHECK_COLOR(p->destructive, lv_color_hex(0xe7000b));
    CHECK_COLOR(p->border, lv_color_hex(0xe4e4e7));
    CHECK_COLOR(p->ring, lv_color_hex(0x9f9fa9));
}

static void from_primary_replaces_primary_and_ring(void)
{
    lv_theme_shadcn_palette_t out;
    const lv_theme_shadcn_palette_t * base = lv_theme_shadcn_palette_zinc_dark();
    lv_theme_shadcn_palette_from_primary(&out, base, lv_color_hex(0xef4444), lv_color_hex(0xffffff));
    CHECK_COLOR(out.primary, lv_color_hex(0xef4444));
    CHECK_COLOR(out.primary_fg, lv_color_hex(0xffffff));
    CHECK_COLOR(out.ring, lv_color_mix(lv_color_hex(0xef4444), base->background, LV_OPA_50));
    CHECK_COLOR(out.card, base->card);
    CHECK(out.dark == base->dark);
}

static bool palettes_equal(const lv_theme_shadcn_palette_t * a, const lv_theme_shadcn_palette_t * b)
{
    const lv_color_t * ca = &a->background;
    const lv_color_t * cb = &b->background;
    for(int i = 0; i < 24; i++) {
        if(!lv_color_eq(ca[i], cb[i])) return false;
    }
    return a->dark == b->dark;
}

static void make_covers_every_combination(void)
{
    for(int b = 0; b < LV_THEME_SHADCN_BASE_COUNT; b++) {
        for(int d = 0; d < 2; d++) {
            lv_theme_shadcn_palette_t base;
            lv_theme_shadcn_palette_make(&base, (lv_theme_shadcn_base_t)b, LV_THEME_SHADCN_ACCENT_NONE, d);
            CHECK(base.dark == (d == 1));
            CHECK(!lv_color_eq(base.background, base.foreground));
            for(int a = 0; a < LV_THEME_SHADCN_ACCENT_COUNT; a++) {
                lv_theme_shadcn_palette_t p;
                lv_theme_shadcn_palette_make(&p, (lv_theme_shadcn_base_t)b, (lv_theme_shadcn_accent_t)a, d);
                CHECK(p.dark == (d == 1));
                CHECK_COLOR(p.background, base.background);
                CHECK_COLOR(p.card, base.card);
                CHECK_COLOR(p.border, base.border);
                CHECK_COLOR(p.destructive, base.destructive);
                if(a == LV_THEME_SHADCN_ACCENT_NONE) CHECK(palettes_equal(&p, &base));
                else CHECK(!lv_color_eq(p.primary, base.primary));
            }
        }
    }
}

static void make_zinc_none_matches_zinc_palettes(void)
{
    lv_theme_shadcn_palette_t p;
    lv_theme_shadcn_palette_make(&p, LV_THEME_SHADCN_BASE_ZINC, LV_THEME_SHADCN_ACCENT_NONE, true);
    CHECK(palettes_equal(&p, lv_theme_shadcn_palette_zinc_dark()));
    lv_theme_shadcn_palette_make(&p, LV_THEME_SHADCN_BASE_ZINC, LV_THEME_SHADCN_ACCENT_NONE, false);
    CHECK(palettes_equal(&p, lv_theme_shadcn_palette_zinc_light()));
}

static void make_applies_accent_colours(void)
{
    lv_theme_shadcn_palette_t p;
    lv_theme_shadcn_palette_make(&p, LV_THEME_SHADCN_BASE_NEUTRAL, LV_THEME_SHADCN_ACCENT_BLUE, false);
    CHECK_COLOR(p.primary, lv_color_hex(0x1447e6));
    CHECK_COLOR(p.chart[1], lv_color_hex(0x2b7fff));
    CHECK_COLOR(p.background, lv_color_hex(0xffffff));
    lv_theme_shadcn_palette_make(&p, LV_THEME_SHADCN_BASE_STONE, LV_THEME_SHADCN_ACCENT_RED, true);
    CHECK_COLOR(p.primary, lv_color_hex(0x9f0712));
    CHECK_COLOR(p.background, lv_color_hex(0x0c0a09));
}

static void make_handles_bad_arguments(void)
{
    lv_theme_shadcn_palette_make(NULL, LV_THEME_SHADCN_BASE_ZINC, LV_THEME_SHADCN_ACCENT_NONE, true);
    lv_theme_shadcn_palette_t p;
    lv_theme_shadcn_palette_make(&p, LV_THEME_SHADCN_BASE_COUNT, LV_THEME_SHADCN_ACCENT_COUNT, true);
    CHECK(palettes_equal(&p, lv_theme_shadcn_palette_zinc_dark()));
    lv_theme_shadcn_palette_make(&p, (lv_theme_shadcn_base_t)-1, (lv_theme_shadcn_accent_t)-1, false);
    CHECK(palettes_equal(&p, lv_theme_shadcn_palette_zinc_light()));
}

static void names_match_shadcn(void)
{
    CHECK(strcmp(lv_theme_shadcn_base_name(LV_THEME_SHADCN_BASE_NEUTRAL), "neutral") == 0);
    CHECK(strcmp(lv_theme_shadcn_base_name(LV_THEME_SHADCN_BASE_ZINC), "zinc") == 0);
    CHECK(strcmp(lv_theme_shadcn_base_name(LV_THEME_SHADCN_BASE_TAUPE), "taupe") == 0);
    CHECK(strcmp(lv_theme_shadcn_base_name(LV_THEME_SHADCN_BASE_COUNT), "zinc") == 0);
    CHECK(strcmp(lv_theme_shadcn_accent_name(LV_THEME_SHADCN_ACCENT_NONE), "none") == 0);
    CHECK(strcmp(lv_theme_shadcn_accent_name(LV_THEME_SHADCN_ACCENT_AMBER), "amber") == 0);
    CHECK(strcmp(lv_theme_shadcn_accent_name(LV_THEME_SHADCN_ACCENT_BLUE), "blue") == 0);
    CHECK(strcmp(lv_theme_shadcn_accent_name(LV_THEME_SHADCN_ACCENT_YELLOW), "yellow") == 0);
    CHECK(strcmp(lv_theme_shadcn_accent_name(LV_THEME_SHADCN_ACCENT_COUNT), "none") == 0);
    for(int b = 0; b < LV_THEME_SHADCN_BASE_COUNT; b++) {
        for(int o = b + 1; o < LV_THEME_SHADCN_BASE_COUNT; o++) {
            CHECK(strcmp(lv_theme_shadcn_base_name((lv_theme_shadcn_base_t)b), lv_theme_shadcn_base_name((lv_theme_shadcn_base_t)o)) != 0);
        }
    }
    for(int a = 0; a < LV_THEME_SHADCN_ACCENT_COUNT; a++) {
        for(int o = a + 1; o < LV_THEME_SHADCN_ACCENT_COUNT; o++) {
            CHECK(strcmp(lv_theme_shadcn_accent_name((lv_theme_shadcn_accent_t)a), lv_theme_shadcn_accent_name((lv_theme_shadcn_accent_t)o)) != 0);
        }
    }
}

static void check_widgets(lv_obj_t * screen, lv_obj_t * card, lv_obj_t * button, lv_obj_t * slider, lv_obj_t * ta,
                          const lv_theme_shadcn_palette_t * p)
{
    CHECK_COLOR(lv_obj_get_style_bg_color(screen, LV_PART_MAIN), p->background);
    CHECK_COLOR(lv_obj_get_style_bg_color(card, LV_PART_MAIN), p->card);
    CHECK_COLOR(lv_obj_get_style_border_color(card, LV_PART_MAIN), p->border);
    CHECK_COLOR(lv_obj_get_style_bg_color(button, LV_PART_MAIN), p->primary);
    CHECK_COLOR(lv_obj_get_style_text_color(button, LV_PART_MAIN), p->primary_fg);
    CHECK_COLOR(lv_obj_get_style_bg_color(slider, LV_PART_MAIN), p->secondary);
    CHECK_COLOR(lv_obj_get_style_bg_color(slider, LV_PART_INDICATOR), p->primary);
    CHECK_COLOR(lv_obj_get_style_border_color(ta, LV_PART_MAIN), p->input);
}

static void set_palette_follows_every_combination(void)
{
    lv_obj_t * screen = lv_screen_active();
    lv_obj_t * card = lv_obj_create(screen);
    lv_obj_t * button = lv_button_create(card);
    lv_label_set_text(lv_label_create(button), "Go");
    lv_obj_t * slider = lv_slider_create(card);
    lv_obj_t * ta = lv_textarea_create(card);
    harness_settle();
    for(int round = 0; round < 3; round++) {
        for(int b = 0; b < LV_THEME_SHADCN_BASE_COUNT; b++) {
            for(int a = 0; a < LV_THEME_SHADCN_ACCENT_COUNT; a++) {
                lv_theme_shadcn_palette_t p;
                bool dark = ((a + b + round) & 1) != 0;
                lv_theme_shadcn_palette_make(&p, (lv_theme_shadcn_base_t)b, (lv_theme_shadcn_accent_t)a, dark);
                lv_theme_shadcn_set_palette(harness_display(), &p);
                check_widgets(screen, card, button, slider, ta, &p);
                CHECK(palettes_equal(lv_theme_shadcn_get_palette(harness_display()), &p));
            }
        }
        harness_settle();
    }
    lv_theme_shadcn_palette_t p;
    lv_theme_shadcn_palette_make(&p, LV_THEME_SHADCN_BASE_SLATE, LV_THEME_SHADCN_ACCENT_ROSE, false);
    lv_theme_shadcn_set_palette(harness_display(), &p);
    harness_settle();
    check_widgets(screen, card, button, slider, ta, &p);
    CHECK_INT(lv_obj_get_child_count(card), 3);
}

int main(void)
{
    RUN_RAW(zinc_dark_matches_spec);
    RUN_RAW(zinc_light_matches_spec);
    RUN_RAW(from_primary_replaces_primary_and_ring);
    RUN_RAW(make_covers_every_combination);
    RUN_RAW(make_zinc_none_matches_zinc_palettes);
    RUN_RAW(make_applies_accent_colours);
    RUN_RAW(make_handles_bad_arguments);
    RUN_RAW(names_match_shadcn);
    RUN(set_palette_follows_every_combination);
    return harness_result();
}
