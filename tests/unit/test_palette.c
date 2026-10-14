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
    CHECK_COLOR(p->chart[1], lv_color_hex(0x2b7fff));
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

int main(void)
{
    RUN_RAW(zinc_dark_matches_spec);
    RUN_RAW(zinc_light_matches_spec);
    RUN_RAW(from_primary_replaces_primary_and_ring);
    return harness_result();
}
