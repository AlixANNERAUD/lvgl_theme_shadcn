#include "lv_theme_shadcn_private.h"

#define HEX(c) LV_COLOR_MAKE(((c) >> 16) & 0xff, ((c) >> 8) & 0xff, (c) & 0xff)

static const lv_theme_shadcn_palette_t zinc_dark = {
    .background = HEX(0x09090b), .foreground = HEX(0xfafafa),
    .card = HEX(0x18181b), .card_fg = HEX(0xfafafa),
    .popover = HEX(0x18181b), .popover_fg = HEX(0xfafafa),
    .primary = HEX(0xe4e4e7), .primary_fg = HEX(0x18181b),
    .secondary = HEX(0x27272a), .secondary_fg = HEX(0xfafafa),
    .muted = HEX(0x27272a), .muted_fg = HEX(0x9f9fa9),
    .accent = HEX(0x27272a), .accent_fg = HEX(0xfafafa),
    .destructive = HEX(0xff6467), .destructive_fg = HEX(0xfafafa),
    .border = HEX(0x27272a), .input = HEX(0x3f3f46), .ring = HEX(0x71717b),
    .chart = { HEX(0x8ec5ff), HEX(0x2b7fff), HEX(0x155dfc), HEX(0x1447e6), HEX(0x193cb8) },
    .dark = true,
};

static const lv_theme_shadcn_palette_t zinc_light = {
    .background = HEX(0xffffff), .foreground = HEX(0x09090b),
    .card = HEX(0xffffff), .card_fg = HEX(0x09090b),
    .popover = HEX(0xffffff), .popover_fg = HEX(0x09090b),
    .primary = HEX(0x18181b), .primary_fg = HEX(0xfafafa),
    .secondary = HEX(0xf4f4f5), .secondary_fg = HEX(0x18181b),
    .muted = HEX(0xf4f4f5), .muted_fg = HEX(0x71717b),
    .accent = HEX(0xf4f4f5), .accent_fg = HEX(0x18181b),
    .destructive = HEX(0xe7000b), .destructive_fg = HEX(0xfafafa),
    .border = HEX(0xe4e4e7), .input = HEX(0xe4e4e7), .ring = HEX(0x9f9fa9),
    .chart = { HEX(0x8ec5ff), HEX(0x2b7fff), HEX(0x155dfc), HEX(0x1447e6), HEX(0x193cb8) },
    .dark = false,
};

const lv_theme_shadcn_palette_t * lv_theme_shadcn_palette_zinc_dark(void)
{
    return &zinc_dark;
}

const lv_theme_shadcn_palette_t * lv_theme_shadcn_palette_zinc_light(void)
{
    return &zinc_light;
}

void lv_theme_shadcn_palette_from_primary(lv_theme_shadcn_palette_t * out, const lv_theme_shadcn_palette_t * base,
                                          lv_color_t primary, lv_color_t primary_fg)
{
    *out = *base;
    out->primary = primary;
    out->primary_fg = primary_fg;
    out->ring = lv_color_mix(primary, base->background, LV_OPA_50);
}
