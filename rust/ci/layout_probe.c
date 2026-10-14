#include <stddef.h>
#include "lv_theme_shadcn.h"

size_t lv_theme_shadcn_probe_palette_size(void)
{
    return sizeof(lv_theme_shadcn_palette_t);
}

size_t lv_theme_shadcn_probe_palette_primary_offset(void)
{
    return offsetof(lv_theme_shadcn_palette_t, primary);
}

size_t lv_theme_shadcn_probe_palette_chart_offset(void)
{
    return offsetof(lv_theme_shadcn_palette_t, chart);
}

size_t lv_theme_shadcn_probe_palette_dark_offset(void)
{
    return offsetof(lv_theme_shadcn_palette_t, dark);
}
