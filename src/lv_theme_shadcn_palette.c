#include "lv_theme_shadcn_private.h"

static bool base_valid(lv_theme_shadcn_base_t base)
{
    return (int)base >= 0 && base < LV_THEME_SHADCN_BASE_COUNT;
}

static bool accent_valid(lv_theme_shadcn_accent_t accent)
{
    return (int)accent >= 0 && accent < LV_THEME_SHADCN_ACCENT_COUNT;
}

const lv_theme_shadcn_palette_t * lv_theme_shadcn_palette_zinc_dark(void)
{
    return &lv_theme_shadcn_base_palettes[LV_THEME_SHADCN_BASE_ZINC][1];
}

const lv_theme_shadcn_palette_t * lv_theme_shadcn_palette_zinc_light(void)
{
    return &lv_theme_shadcn_base_palettes[LV_THEME_SHADCN_BASE_ZINC][0];
}

void lv_theme_shadcn_palette_from_primary(lv_theme_shadcn_palette_t * out, const lv_theme_shadcn_palette_t * base,
                                          lv_color_t primary, lv_color_t primary_fg)
{
    *out = *base;
    out->primary = primary;
    out->primary_fg = primary_fg;
    out->ring = lv_color_mix(primary, base->background, LV_OPA_50);
}

void lv_theme_shadcn_palette_make(lv_theme_shadcn_palette_t * out, lv_theme_shadcn_base_t base,
                                  lv_theme_shadcn_accent_t accent, bool dark)
{
    if(out == NULL) return;
    if(!base_valid(base)) base = LV_THEME_SHADCN_BASE_ZINC;
    if(!accent_valid(accent)) accent = LV_THEME_SHADCN_ACCENT_NONE;
    *out = lv_theme_shadcn_base_palettes[base][dark ? 1 : 0];
    if(accent == LV_THEME_SHADCN_ACCENT_NONE) return;
    const lv_theme_shadcn_accent_colors_t * colors = &lv_theme_shadcn_accent_palettes[accent - 1][dark ? 1 : 0];
    out->primary = colors->primary;
    out->primary_fg = colors->primary_fg;
    out->secondary = colors->secondary;
    out->secondary_fg = colors->secondary_fg;
    for(int i = 0; i < 5; i++) out->chart[i] = colors->chart[i];
}

const char * lv_theme_shadcn_base_name(lv_theme_shadcn_base_t base)
{
    return lv_theme_shadcn_base_names[base_valid(base) ? base : LV_THEME_SHADCN_BASE_ZINC];
}

const char * lv_theme_shadcn_accent_name(lv_theme_shadcn_accent_t accent)
{
    return lv_theme_shadcn_accent_names[accent_valid(accent) ? accent : LV_THEME_SHADCN_ACCENT_NONE];
}
