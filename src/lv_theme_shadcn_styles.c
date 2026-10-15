#include "lv_theme_shadcn_private.h"

static const lv_style_prop_t transition_props[] = {
    LV_STYLE_BG_COLOR, LV_STYLE_BORDER_COLOR, LV_STYLE_OPA, LV_STYLE_PROP_INV
};

static const int32_t radius_centi_dp[LV_THEME_SHADCN_RADIUS_COUNT] = { 0, 480, 800, 1000, 1200, 1600 };

static int32_t dp(const lv_theme_shadcn_state_t * st, int32_t value)
{
    return lv_display_dpx(st->disp, value);
}

static int32_t radius_dp(const lv_theme_shadcn_state_t * st, int32_t scale_percent)
{
    int32_t centi = radius_centi_dp[st->radius] * scale_percent / 100;
    return (dp(st, centi) + 50) / 100;
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
    m->radius_sm = radius_dp(st, 60);
    m->radius_md = radius_dp(st, 80);
    m->radius_lg = radius_dp(st, 100);
    m->radius_xl = radius_dp(st, 140);
    m->border_width = dp(st, 1);
    m->ring_width = dp(st, 2);
    m->ring_gap = dp(st, compact ? 1 : 2);
    m->control_height = dp(st, compact ? 32 : 40);
    m->control_height_sm = dp(st, compact ? 24 : 32);
    m->control_height_lg = dp(st, compact ? 40 : 48);
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

    lv_style_set_radius(&s->card, m->radius_xl);

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

    lv_style_set_bg_color(&s->variant_hovered[LV_THEME_SHADCN_VARIANT_DEFAULT],
                          lv_color_mix(p->primary, p->background, LV_THEME_SHADCN_HOVER_OPA));
    lv_style_set_bg_color(&s->variant_hovered[LV_THEME_SHADCN_VARIANT_SECONDARY],
                          lv_color_mix(p->secondary, p->background, LV_THEME_SHADCN_HOVER_SECONDARY_OPA));
    lv_style_set_bg_color(&s->variant_hovered[LV_THEME_SHADCN_VARIANT_DESTRUCTIVE],
                          lv_color_mix(p->destructive, p->background, LV_THEME_SHADCN_HOVER_OPA));
    set_surface(&s->variant_hovered[LV_THEME_SHADCN_VARIANT_OUTLINE], p->accent, p->accent_fg);
    set_surface(&s->variant_hovered[LV_THEME_SHADCN_VARIANT_GHOST], p->accent, p->accent_fg);
}

int32_t lv_theme_shadcn_size_height(const lv_theme_shadcn_state_t * st, lv_theme_shadcn_size_t size)
{
    const lv_theme_shadcn_metrics_t * m = &st->metrics;
    switch(size) {
        case LV_THEME_SHADCN_SIZE_SM:
        case LV_THEME_SHADCN_SIZE_ICON_SM:
            return m->control_height_sm;
        case LV_THEME_SHADCN_SIZE_LG:
        case LV_THEME_SHADCN_SIZE_ICON_LG:
            return m->control_height_lg;
        default:
            return m->control_height;
    }
}

static void set_size(lv_theme_shadcn_state_t * st, lv_theme_shadcn_size_t size, int32_t pad_hor, int32_t gap,
                     const lv_font_t * font)
{
    lv_style_t * style = &S(st)->size[size - 1];
    int32_t h = lv_theme_shadcn_size_height(st, size);
    lv_style_set_height(style, h);
    lv_style_set_min_height(style, h);
    lv_style_set_pad_hor(style, pad_hor);
    lv_style_set_pad_gap(style, gap);
    if(size >= LV_THEME_SHADCN_SIZE_ICON) {
        lv_style_set_width(style, h);
        lv_style_set_min_width(style, h);
    }
    if(font) lv_style_set_text_font(style, font);
}

static void build_sizes(lv_theme_shadcn_state_t * st)
{
    const lv_theme_shadcn_metrics_t * m = &st->metrics;
    int32_t gap_sm = (m->space_xs + m->space_sm) / 2;
    set_size(st, LV_THEME_SHADCN_SIZE_SM, m->space_md, gap_sm, st->fonts.small);
    set_size(st, LV_THEME_SHADCN_SIZE_LG, m->space_xl, m->space_sm, NULL);
    set_size(st, LV_THEME_SHADCN_SIZE_ICON, 0, m->space_sm, NULL);
    set_size(st, LV_THEME_SHADCN_SIZE_ICON_SM, 0, gap_sm, st->fonts.small);
    set_size(st, LV_THEME_SHADCN_SIZE_ICON_LG, 0, m->space_sm, NULL);
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

    lv_style_set_bg_color(&s->switch_track_hovered, lv_color_mix(p->input, p->foreground, LV_THEME_SHADCN_HOVER_OPA));

    lv_color_t primary_hovered = lv_color_mix(p->primary, p->background, LV_THEME_SHADCN_HOVER_OPA);
    lv_style_set_bg_color(&s->fill_primary_hovered, primary_hovered);
    lv_style_set_border_color(&s->fill_primary_hovered, primary_hovered);

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

    lv_style_set_bg_color(&s->checkbox_marker_hovered, p->accent);

    lv_style_set_bg_color(&s->led, lv_color_white());
    lv_style_set_bg_opa(&s->led, LV_OPA_COVER);
    lv_style_set_border_width(&s->led, 0);
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

    lv_style_set_line_color(&s->scale_minor, p->border);
    lv_style_set_line_width(&s->scale_minor, m->border_width);
    lv_style_set_length(&s->scale_minor, m->space_xs);

    lv_style_set_line_color(&s->scale_ticks, p->muted_fg);
    lv_style_set_line_width(&s->scale_ticks, m->ring_width);
    lv_style_set_length(&s->scale_ticks, m->space_sm);
    lv_style_set_text_color(&s->scale_ticks, p->muted_fg);

    lv_style_set_line_color(&s->line, p->foreground);
    lv_style_set_line_width(&s->line, m->ring_width);
    lv_style_set_line_rounded(&s->line, true);
}

static void build_inputs(lv_theme_shadcn_state_t * st)
{
    const lv_theme_shadcn_palette_t * p = &st->palette;
    const lv_theme_shadcn_metrics_t * m = &st->metrics;
    lv_theme_shadcn_styles_t * s = S(st);

    set_surface(&s->input, p->background, p->foreground);
    lv_style_set_border_width(&s->input, m->border_width);
    lv_style_set_border_color(&s->input, p->input);
    lv_style_set_border_opa(&s->input, LV_OPA_COVER);
    lv_style_set_radius(&s->input, m->radius_md);
    lv_style_set_pad_hor(&s->input, m->space_md);
    lv_style_set_pad_ver(&s->input, m->space_sm);

    lv_style_set_text_color(&s->placeholder, p->muted_fg);

    lv_style_set_border_color(&s->cursor, p->foreground);
    lv_style_set_border_width(&s->cursor, m->ring_width);
    lv_style_set_border_side(&s->cursor, LV_BORDER_SIDE_LEFT);
    lv_style_set_pad_left(&s->cursor, -m->border_width);
    lv_style_set_anim_duration(&s->cursor, 400);

    lv_style_set_border_width(&s->popover_frame, m->border_width);
    lv_style_set_border_color(&s->popover_frame, p->border);
    lv_style_set_border_opa(&s->popover_frame, LV_OPA_COVER);
    lv_style_set_radius(&s->popover_frame, m->radius_md);
    lv_style_set_pad_all(&s->popover_frame, m->space_xs);

    set_surface(&s->key, p->secondary, p->secondary_fg);
    lv_style_set_radius(&s->key, m->radius_md);
    lv_style_set_border_width(&s->key, 0);
    lv_style_set_shadow_width(&s->key, 0);

    lv_style_set_bg_color(&s->key_pressed, lv_color_mix(p->secondary_fg, p->secondary, LV_THEME_SHADCN_PRESS_MIX));

    set_surface(&s->key_control, p->input, p->foreground);

    lv_style_set_text_align(&s->roller, LV_TEXT_ALIGN_CENTER);
    lv_style_set_text_line_space(&s->roller, m->space_md);
    lv_style_set_anim_duration(&s->roller, 200);
}

static void build_collections(lv_theme_shadcn_state_t * st)
{
    const lv_theme_shadcn_palette_t * p = &st->palette;
    const lv_theme_shadcn_metrics_t * m = &st->metrics;
    lv_theme_shadcn_styles_t * s = S(st);

    lv_style_set_bg_opa(&s->list_text, LV_OPA_TRANSP);
    lv_style_set_text_color(&s->list_text, p->muted_fg);
    lv_style_set_text_font(&s->list_text, st->fonts.small);
    lv_style_set_pad_hor(&s->list_text, m->space_md);
    lv_style_set_pad_ver(&s->list_text, m->space_sm);

    lv_style_set_bg_opa(&s->list_button, LV_OPA_TRANSP);
    lv_style_set_text_color(&s->list_button, p->foreground);
    lv_style_set_border_width(&s->list_button, m->border_width);
    lv_style_set_border_color(&s->list_button, p->border);
    lv_style_set_border_side(&s->list_button, LV_BORDER_SIDE_BOTTOM);
    lv_style_set_radius(&s->list_button, 0);
    lv_style_set_pad_all(&s->list_button, m->space_md);
    lv_style_set_pad_gap(&s->list_button, m->space_md);
    lv_style_set_min_height(&s->list_button, m->touch_target);

    lv_style_set_clip_corner(&s->clip_corner, true);

    lv_style_set_bg_opa(&s->table_cell, LV_OPA_TRANSP);
    lv_style_set_text_color(&s->table_cell, p->foreground);
    lv_style_set_border_width(&s->table_cell, m->border_width);
    lv_style_set_border_color(&s->table_cell, p->border);
    lv_style_set_border_opa(&s->table_cell, LV_OPA_COVER);
    lv_style_set_border_side(&s->table_cell, LV_BORDER_SIDE_BOTTOM);
    lv_style_set_pad_all(&s->table_cell, m->space_sm);

    set_surface(&s->tab_bar, p->muted, p->muted_fg);
    lv_style_set_radius(&s->tab_bar, m->radius_md);
    lv_style_set_pad_all(&s->tab_bar, m->space_xs);
    lv_style_set_pad_gap(&s->tab_bar, m->space_xs);
    lv_style_set_border_width(&s->tab_bar, 0);

    lv_style_set_bg_opa(&s->tab_button, LV_OPA_TRANSP);
    lv_style_set_text_color(&s->tab_button, p->muted_fg);
    lv_style_set_radius(&s->tab_button, m->radius_sm);
    lv_style_set_border_width(&s->tab_button, 0);
    lv_style_set_min_height(&s->tab_button, 0);
    lv_style_set_pad_ver(&s->tab_button, m->space_xs);

    set_surface(&s->tab_button_checked, p->background, p->foreground);

    set_surface(&s->calendar_day, p->card, p->card_fg);
    lv_style_set_border_width(&s->calendar_day, m->border_width);
    lv_style_set_border_color(&s->calendar_day, p->card);
    lv_style_set_border_opa(&s->calendar_day, LV_OPA_COVER);
    lv_style_set_radius(&s->calendar_day, m->radius_sm);

    lv_style_set_line_color(&s->chart, p->border);
    lv_style_set_line_width(&s->chart, m->border_width);

    lv_style_set_line_width(&s->chart_series, dp(st, 3));
    lv_style_set_radius(&s->chart_series, dp(st, 3));
    lv_style_set_size(&s->chart_series, dp(st, 8), dp(st, 8));
    lv_style_set_pad_column(&s->chart_series, dp(st, 2));
}

static void build_overlays(lv_theme_shadcn_state_t * st)
{
    const lv_theme_shadcn_palette_t * p = &st->palette;
    const lv_theme_shadcn_metrics_t * m = &st->metrics;
    lv_theme_shadcn_styles_t * s = S(st);

    lv_style_set_bg_color(&s->backdrop, lv_color_black());
    lv_style_set_bg_opa(&s->backdrop, LV_OPA_50);

    lv_style_set_border_width(&s->msgbox, m->border_width);
    lv_style_set_border_color(&s->msgbox, p->border);
    lv_style_set_border_opa(&s->msgbox, LV_OPA_COVER);
    lv_style_set_radius(&s->msgbox, m->radius_lg);
    lv_style_set_pad_all(&s->msgbox, m->space_xl);
    lv_style_set_pad_gap(&s->msgbox, m->space_md);
    lv_style_set_shadow_width(&s->msgbox, 0);

    lv_style_set_bg_opa(&s->msgbox_footer, LV_OPA_TRANSP);
    lv_style_set_border_width(&s->msgbox_footer, 0);
    lv_style_set_pad_all(&s->msgbox_footer, 0);
    lv_style_set_pad_column(&s->msgbox_footer, m->space_sm);
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
    build_sizes(st);
    build_toggles(st);
    build_ranges(st);
    build_inputs(st);
    build_collections(st);
    build_overlays(st);
}
