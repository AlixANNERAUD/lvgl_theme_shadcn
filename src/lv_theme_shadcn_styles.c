#include "lv_theme_shadcn_private.h"

static const lv_style_prop_t transition_props[] = {
    LV_STYLE_BG_COLOR, LV_STYLE_BORDER_COLOR, LV_STYLE_OPA, LV_STYLE_PROP_INV
};

static int32_t dp(const lv_theme_shadcn_state_t * st, int32_t value)
{
    return lv_display_dpx(st->disp, value);
}

void lv_theme_shadcn_metrics_resolve(lv_theme_shadcn_state_t * st)
{
    bool compact = (st->flags & LV_THEME_SHADCN_FLAG_COMPACT) != 0;
    lv_theme_shadcn_metrics_t * m = &st->metrics;
    m->space_xs = dp(st, compact ? 2 : 4);
    m->space_sm = dp(st, compact ? 4 : 8);
    m->space_md = dp(st, compact ? 8 : 12);
    m->space_lg = dp(st, compact ? 12 : 16);
    m->space_xl = dp(st, compact ? 16 : 24);
    m->radius_sm = dp(st, 6);
    m->radius_md = dp(st, 8);
    m->radius_lg = dp(st, 10);
    m->border_width = dp(st, 1);
    m->ring_width = dp(st, 2);
    m->ring_gap = dp(st, compact ? 1 : 2);
    m->control_height = dp(st, compact ? 32 : 40);
    m->touch_target = dp(st, compact ? 40 : 48);
    m->scrollbar_width = dp(st, compact ? 3 : 4);
}

static void build_base(lv_theme_shadcn_state_t * st)
{
    const lv_theme_shadcn_palette_t * p = &st->palette;
    const lv_theme_shadcn_metrics_t * m = &st->metrics;
    lv_theme_shadcn_styles_t * s = S(st);

    lv_style_set_bg_color(&s->screen, p->background);
    lv_style_set_bg_opa(&s->screen, LV_OPA_COVER);
    lv_style_set_text_color(&s->screen, p->foreground);
    lv_style_set_text_font(&s->screen, st->fonts.normal);

    lv_style_set_bg_color(&s->scrollbar, p->muted_fg);
    lv_style_set_bg_opa(&s->scrollbar, LV_OPA_40);
    lv_style_set_radius(&s->scrollbar, LV_RADIUS_CIRCLE);
    lv_style_set_width(&s->scrollbar, m->scrollbar_width);
    lv_style_set_pad_right(&s->scrollbar, m->space_xs);
    lv_style_set_pad_bottom(&s->scrollbar, m->space_xs);

    lv_style_set_bg_opa(&s->scrollbar_scrolled, LV_OPA_70);

    lv_style_transition_dsc_init(&st->transition_dsc, transition_props, lv_anim_path_linear,
                                 LV_THEME_SHADCN_TRANSITION_MS, 0, NULL);
    lv_style_set_transition(&s->transition, &st->transition_dsc);

    lv_style_set_outline_width(&s->focus_ring, m->ring_width);
    lv_style_set_outline_pad(&s->focus_ring, m->ring_gap);
    lv_style_set_outline_color(&s->focus_ring, p->ring);
    lv_style_set_outline_opa(&s->focus_ring, LV_OPA_COVER);

    lv_style_set_opa(&s->disabled, LV_THEME_SHADCN_DISABLED_OPA);
}

void lv_theme_shadcn_styles_reset(lv_theme_shadcn_state_t * st)
{
    lv_style_t * styles = (lv_style_t *)S(st);
    size_t count = sizeof(lv_theme_shadcn_styles_t) / sizeof(lv_style_t);
    for(size_t i = 0; i < count; i++) {
        if(st->styles_ready) lv_style_reset(&styles[i]);
        else lv_style_init(&styles[i]);
    }
    st->styles_ready = true;
}

void lv_theme_shadcn_styles_build(lv_theme_shadcn_state_t * st)
{
    lv_theme_shadcn_styles_reset(st);
    build_base(st);
}
