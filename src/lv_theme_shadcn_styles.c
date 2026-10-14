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

static void set_surface(lv_style_t * style, lv_color_t bg, lv_color_t fg)
{
    lv_style_set_bg_color(style, bg);
    lv_style_set_bg_opa(style, LV_OPA_COVER);
    lv_style_set_text_color(style, fg);
}

static void set_pad(lv_style_t * style, int32_t pad)
{
    lv_style_set_pad_all(style, pad);
    lv_style_set_pad_gap(style, pad);
}

static void build_surfaces(lv_theme_shadcn_state_t * st)
{
    const lv_theme_shadcn_palette_t * p = &st->palette;
    const lv_theme_shadcn_metrics_t * m = &st->metrics;
    lv_theme_shadcn_styles_t * s = S(st);

    set_surface(&s->surface[LV_THEME_SHADCN_SURFACE_BACKGROUND], p->background, p->foreground);
    set_surface(&s->surface[LV_THEME_SHADCN_SURFACE_CARD], p->card, p->card_fg);
    set_surface(&s->surface[LV_THEME_SHADCN_SURFACE_POPOVER], p->popover, p->popover_fg);
    set_surface(&s->surface[LV_THEME_SHADCN_SURFACE_MUTED], p->muted, p->muted_fg);

    lv_style_set_border_width(&s->container, m->border_width);
    lv_style_set_border_color(&s->container, p->border);
    lv_style_set_border_opa(&s->container, LV_OPA_COVER);
    lv_style_set_radius(&s->container, m->radius_md);
    lv_style_set_pad_all(&s->container, m->space_lg);
    lv_style_set_pad_gap(&s->container, m->space_md);

    lv_style_set_bg_opa(&s->transparent, LV_OPA_TRANSP);
    lv_style_set_border_width(&s->transparent, 0);
    lv_style_set_radius(&s->transparent, 0);
    set_pad(&s->transparent, 0);

    set_pad(&s->pad_none, 0);
    set_pad(&s->pad_xs, m->space_xs);
    set_pad(&s->pad_sm, m->space_sm);
    set_pad(&s->pad_md, m->space_md);
    set_pad(&s->pad_lg, m->space_lg);

    lv_style_set_text_color(&s->text_muted, p->muted_fg);
    lv_style_set_text_font(&s->font_small, st->fonts.small);
    lv_style_set_text_font(&s->font_large, st->fonts.large);

    set_surface(&s->accent, p->accent, p->accent_fg);

    lv_style_set_border_color(&s->focused_input, p->ring);

    lv_style_set_border_color(&s->error, p->destructive);
    lv_style_set_text_color(&s->error, p->destructive);
}

static void set_filled(lv_style_t * normal, lv_style_t * pressed, lv_color_t bg, lv_color_t fg)
{
    lv_style_set_bg_color(normal, bg);
    lv_style_set_bg_opa(normal, LV_OPA_COVER);
    lv_style_set_text_color(normal, fg);
    lv_style_set_border_width(normal, 0);
    lv_style_set_bg_color(pressed, lv_color_mix(fg, bg, LV_THEME_SHADCN_PRESS_MIX));
}

static void build_actions(lv_theme_shadcn_state_t * st)
{
    const lv_theme_shadcn_palette_t * p = &st->palette;
    const lv_theme_shadcn_metrics_t * m = &st->metrics;
    lv_theme_shadcn_styles_t * s = S(st);

    lv_style_set_min_height(&s->control, m->control_height);
    lv_style_set_height(&s->control, m->control_height);
    lv_style_set_radius(&s->control, m->radius_md);
    lv_style_set_pad_hor(&s->control, m->space_lg);
    lv_style_set_pad_ver(&s->control, m->space_sm);
    lv_style_set_pad_gap(&s->control, m->space_sm);
    lv_style_set_shadow_width(&s->control, 0);
#if LV_USE_FLEX
    lv_style_set_layout(&s->control, LV_LAYOUT_FLEX);
    lv_style_set_flex_flow(&s->control, LV_FLEX_FLOW_ROW);
    lv_style_set_flex_main_place(&s->control, LV_FLEX_ALIGN_CENTER);
    lv_style_set_flex_cross_place(&s->control, LV_FLEX_ALIGN_CENTER);
    lv_style_set_flex_track_place(&s->control, LV_FLEX_ALIGN_CENTER);
#endif

    set_filled(&s->variant[LV_THEME_SHADCN_VARIANT_DEFAULT], &s->variant_pressed[LV_THEME_SHADCN_VARIANT_DEFAULT], p->primary, p->primary_fg);
    set_filled(&s->variant[LV_THEME_SHADCN_VARIANT_SECONDARY], &s->variant_pressed[LV_THEME_SHADCN_VARIANT_SECONDARY], p->secondary, p->secondary_fg);
    set_filled(&s->variant[LV_THEME_SHADCN_VARIANT_DESTRUCTIVE], &s->variant_pressed[LV_THEME_SHADCN_VARIANT_DESTRUCTIVE], p->destructive, p->destructive_fg);

    lv_style_t * outline = &s->variant[LV_THEME_SHADCN_VARIANT_OUTLINE];
    lv_style_set_bg_opa(outline, LV_OPA_TRANSP);
    lv_style_set_border_width(outline, m->border_width);
    lv_style_set_border_color(outline, p->input);
    lv_style_set_border_opa(outline, LV_OPA_COVER);
    lv_style_set_text_color(outline, p->foreground);

    lv_style_t * ghost = &s->variant[LV_THEME_SHADCN_VARIANT_GHOST];
    lv_style_set_bg_opa(ghost, LV_OPA_TRANSP);
    lv_style_set_border_width(ghost, 0);
    lv_style_set_text_color(ghost, p->foreground);

    set_surface(&s->variant_pressed[LV_THEME_SHADCN_VARIANT_OUTLINE], p->accent, p->accent_fg);
    set_surface(&s->variant_pressed[LV_THEME_SHADCN_VARIANT_GHOST], p->accent, p->accent_fg);
}

static void build_toggles(lv_theme_shadcn_state_t * st)
{
    const lv_theme_shadcn_palette_t * p = &st->palette;
    const lv_theme_shadcn_metrics_t * m = &st->metrics;
    lv_theme_shadcn_styles_t * s = S(st);

    lv_style_set_bg_color(&s->fill_primary, p->primary);
    lv_style_set_bg_opa(&s->fill_primary, LV_OPA_COVER);
    lv_style_set_radius(&s->fill_primary, LV_RADIUS_CIRCLE);

    lv_style_set_bg_color(&s->switch_track, p->input);
    lv_style_set_bg_opa(&s->switch_track, LV_OPA_COVER);
    lv_style_set_radius(&s->switch_track, LV_RADIUS_CIRCLE);
    lv_style_set_border_width(&s->switch_track, 0);

    lv_style_set_bg_color(&s->switch_knob, p->dark ? p->foreground : p->background);
    lv_style_set_bg_opa(&s->switch_knob, LV_OPA_COVER);
    lv_style_set_radius(&s->switch_knob, LV_RADIUS_CIRCLE);
    lv_style_set_pad_all(&s->switch_knob, -m->space_xs / 2);

    lv_style_set_bg_color(&s->switch_knob_checked, p->primary_fg);

    lv_style_set_pad_all(&s->checkbox_main, 0);
    lv_style_set_pad_column(&s->checkbox_main, m->space_sm);

    lv_style_set_bg_color(&s->checkbox_marker, p->background);
    lv_style_set_bg_opa(&s->checkbox_marker, LV_OPA_COVER);
    lv_style_set_border_width(&s->checkbox_marker, m->border_width);
    lv_style_set_border_color(&s->checkbox_marker, p->input);
    lv_style_set_border_opa(&s->checkbox_marker, LV_OPA_COVER);
    lv_style_set_radius(&s->checkbox_marker, m->radius_sm);
    lv_style_set_pad_all(&s->checkbox_marker, m->space_xs / 2);
    lv_style_set_text_font(&s->checkbox_marker, st->fonts.small);

    lv_style_set_bg_color(&s->checkbox_marker_checked, p->primary);
    lv_style_set_border_color(&s->checkbox_marker_checked, p->primary);
    lv_style_set_text_color(&s->checkbox_marker_checked, p->primary_fg);
    lv_style_set_bg_image_src(&s->checkbox_marker_checked, LV_SYMBOL_OK);

    lv_style_set_shadow_width(&s->led, 0);
    lv_style_set_shadow_spread(&s->led, 0);
    lv_style_set_radius(&s->led, LV_RADIUS_CIRCLE);
}

static void build_ranges(lv_theme_shadcn_state_t * st)
{
    const lv_theme_shadcn_palette_t * p = &st->palette;
    const lv_theme_shadcn_metrics_t * m = &st->metrics;
    lv_theme_shadcn_styles_t * s = S(st);

    lv_style_set_bg_color(&s->track, p->secondary);
    lv_style_set_bg_opa(&s->track, LV_OPA_COVER);
    lv_style_set_radius(&s->track, LV_RADIUS_CIRCLE);
    lv_style_set_border_width(&s->track, 0);

    lv_style_set_bg_color(&s->knob, p->background);
    lv_style_set_bg_opa(&s->knob, LV_OPA_COVER);
    lv_style_set_border_width(&s->knob, m->ring_width);
    lv_style_set_border_color(&s->knob, p->primary);
    lv_style_set_border_opa(&s->knob, LV_OPA_COVER);
    lv_style_set_radius(&s->knob, LV_RADIUS_CIRCLE);
    lv_style_set_pad_all(&s->knob, m->space_xs);

    lv_style_set_arc_color(&s->arc_track, p->secondary);
    lv_style_set_arc_width(&s->arc_track, m->space_sm);
    lv_style_set_arc_rounded(&s->arc_track, true);
    lv_style_set_bg_opa(&s->arc_track, LV_OPA_TRANSP);

    lv_style_set_arc_color(&s->arc_indicator, p->primary);
    lv_style_set_arc_width(&s->arc_indicator, m->space_sm);
    lv_style_set_arc_rounded(&s->arc_indicator, true);

    lv_style_set_line_color(&s->scale_main, p->border);
    lv_style_set_line_width(&s->scale_main, m->ring_width);
    lv_style_set_arc_color(&s->scale_main, p->border);
    lv_style_set_arc_width(&s->scale_main, m->ring_width);
    lv_style_set_text_color(&s->scale_main, p->muted_fg);

    lv_style_set_line_color(&s->scale_ticks, p->muted_fg);
    lv_style_set_line_width(&s->scale_ticks, m->ring_width);
    lv_style_set_length(&s->scale_ticks, m->space_sm);
    lv_style_set_text_color(&s->scale_ticks, p->muted_fg);
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
    build_surfaces(st);
    build_actions(st);
    build_toggles(st);
    build_ranges(st);
}
