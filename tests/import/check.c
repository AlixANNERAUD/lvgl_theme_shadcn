#include <stdio.h>
#include <string.h>
#include "lv_theme_shadcn.h"
#include "neutral.inc"
#include "slate_v3.inc"
#include "tweak.inc"

static int failures;

static void same(const char * what, lv_color_t a, lv_color_t b)
{
    if(memcmp(&a, &b, sizeof(a)) != 0) {
        printf("mismatch: %s\n", what);
        failures++;
    }
}

#define SAME(field) same(#field, got->field, want->field)

static void compare(const lv_theme_shadcn_palette_t * got, const lv_theme_shadcn_palette_t * want, int skip_edges)
{
    int i;
    SAME(background);
    SAME(foreground);
    SAME(card);
    SAME(card_fg);
    SAME(popover);
    SAME(popover_fg);
    SAME(primary);
    SAME(primary_fg);
    SAME(secondary);
    SAME(secondary_fg);
    SAME(muted);
    SAME(muted_fg);
    SAME(accent);
    SAME(accent_fg);
    SAME(destructive);
    SAME(destructive_fg);
    SAME(ring);
    if(!skip_edges) {
        SAME(border);
        SAME(input);
    }
    for(i = 0; i < 5; i++) same("chart", got->chart[i], want->chart[i]);
    if(got->dark != want->dark) {
        printf("mismatch: dark flag\n");
        failures++;
    }
}

int main(void)
{
    lv_theme_shadcn_palette_t want;
    lv_theme_shadcn_palette_make(&want, LV_THEME_SHADCN_BASE_NEUTRAL, LV_THEME_SHADCN_ACCENT_NONE, false);
    compare(&neutral_light, &want, 0);
    lv_theme_shadcn_palette_make(&want, LV_THEME_SHADCN_BASE_NEUTRAL, LV_THEME_SHADCN_ACCENT_NONE, true);
    compare(&neutral_dark, &want, 1);
    same("neutral dark border", neutral_dark.border, lv_color_hex(0x222222));
    same("neutral dark input", neutral_dark.input, lv_color_hex(0x2f2f2f));
    same("v3 primary", slate_v3_light.primary, lv_color_hex(0x0f172a));
    same("v3 dark ring", slate_v3_dark.ring, lv_color_hex(0xcbd5e1));
    same("tweak primary", tweak_light.primary, lv_color_hex(0x7c3bed));
    same("tweak chart", tweak_dark.chart[4], lv_color_hex(0xf87171));
    if(!slate_v3_dark.dark || slate_v3_light.dark) failures++;
    return failures ? 1 : 0;
}
