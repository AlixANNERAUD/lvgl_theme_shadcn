#include "harness.h"

static const lv_theme_shadcn_fonts_t heading_fonts = {
    &lv_font_montserrat_12, &lv_font_montserrat_14, &lv_font_montserrat_20, NULL,
    &lv_font_montserrat_32, &lv_font_montserrat_28, &lv_font_montserrat_24
};

static int32_t dp(int32_t v)
{
    return lv_display_dpx(harness_display(), v);
}

static const lv_theme_shadcn_palette_t * palette(void)
{
    return lv_theme_shadcn_get_palette(harness_display());
}

static void init_with(const lv_theme_shadcn_fonts_t * fonts)
{
    lv_theme_shadcn_init(harness_display(), lv_theme_shadcn_palette_zinc_dark(), fonts, LV_THEME_SHADCN_FLAG_NONE);
}

static lv_obj_t * label(void)
{
    lv_obj_t * l = lv_label_create(lv_screen_active());
    lv_label_set_text(l, "Text");
    return l;
}

static const lv_font_t * font_of(lv_obj_t * obj)
{
    return lv_obj_get_style_text_font(obj, LV_PART_MAIN);
}

static void text_roles_pick_fonts(void)
{
    init_with(&heading_fonts);
    static const struct {
        lv_theme_shadcn_text_t role;
        const lv_font_t * font;
    } expected[] = {
        { LV_THEME_SHADCN_TEXT_DEFAULT, &lv_font_montserrat_14 },
        { LV_THEME_SHADCN_TEXT_H1, &lv_font_montserrat_32 },
        { LV_THEME_SHADCN_TEXT_H2, &lv_font_montserrat_28 },
        { LV_THEME_SHADCN_TEXT_H3, &lv_font_montserrat_24 },
        { LV_THEME_SHADCN_TEXT_H4, &lv_font_montserrat_20 },
        { LV_THEME_SHADCN_TEXT_P, &lv_font_montserrat_14 },
        { LV_THEME_SHADCN_TEXT_LEAD, &lv_font_montserrat_20 },
        { LV_THEME_SHADCN_TEXT_LARGE, &lv_font_montserrat_20 },
        { LV_THEME_SHADCN_TEXT_SMALL, &lv_font_montserrat_12 },
        { LV_THEME_SHADCN_TEXT_MUTED, &lv_font_montserrat_12 },
        { LV_THEME_SHADCN_TEXT_CODE, &lv_font_montserrat_14 },
    };
    for(size_t i = 0; i < sizeof(expected) / sizeof(expected[0]); i++) {
        lv_obj_t * l = label();
        lv_theme_shadcn_set_text_role(l, expected[i].role);
        CHECK(font_of(l) == expected[i].font);
    }
}

static void text_roles_pick_colors_and_spacing(void)
{
    const lv_theme_shadcn_palette_t * p = palette();
    lv_obj_t * plain = label();
    lv_obj_t * muted = label();
    lv_obj_t * lead = label();
    lv_obj_t * paragraph = label();
    lv_obj_t * code = label();
    lv_obj_t * h1 = label();
    lv_theme_shadcn_set_text_role(muted, LV_THEME_SHADCN_TEXT_MUTED);
    lv_theme_shadcn_set_text_role(lead, LV_THEME_SHADCN_TEXT_LEAD);
    lv_theme_shadcn_set_text_role(paragraph, LV_THEME_SHADCN_TEXT_P);
    lv_theme_shadcn_set_text_role(code, LV_THEME_SHADCN_TEXT_CODE);
    lv_theme_shadcn_set_text_role(h1, LV_THEME_SHADCN_TEXT_H1);
    CHECK_COLOR(lv_obj_get_style_text_color(plain, LV_PART_MAIN), p->foreground);
    CHECK_COLOR(lv_obj_get_style_text_color(muted, LV_PART_MAIN), p->muted_fg);
    CHECK_COLOR(lv_obj_get_style_text_color(lead, LV_PART_MAIN), p->muted_fg);
    CHECK_COLOR(lv_obj_get_style_text_color(h1, LV_PART_MAIN), p->foreground);
    CHECK(lv_obj_get_style_text_line_space(paragraph, LV_PART_MAIN) > 0);
    CHECK_INT(lv_obj_get_style_text_line_space(plain, LV_PART_MAIN), 0);
    CHECK_INT(lv_obj_get_style_bg_opa(plain, LV_PART_MAIN), LV_OPA_TRANSP);
    CHECK_INT(lv_obj_get_style_bg_opa(code, LV_PART_MAIN), LV_OPA_COVER);
    CHECK_COLOR(lv_obj_get_style_bg_color(code, LV_PART_MAIN), p->muted);
    CHECK_INT(lv_obj_get_style_radius(code, LV_PART_MAIN), dp(6));
    CHECK_INT(lv_obj_get_style_pad_left(code, LV_PART_MAIN), dp(4));
    CHECK_INT(lv_obj_get_style_pad_top(code, LV_PART_MAIN), dp(2));
}

static void text_role_replaces_previous(void)
{
    lv_obj_t * l = label();
    lv_theme_shadcn_set_text_role(l, LV_THEME_SHADCN_TEXT_CODE);
    lv_theme_shadcn_set_text_role(l, LV_THEME_SHADCN_TEXT_MUTED);
    CHECK_INT(lv_obj_get_style_bg_opa(l, LV_PART_MAIN), LV_OPA_TRANSP);
    CHECK_INT(lv_obj_get_style_pad_left(l, LV_PART_MAIN), 0);
    CHECK(font_of(l) == &lv_font_montserrat_12);
    lv_theme_shadcn_set_text_role(l, LV_THEME_SHADCN_TEXT_DEFAULT);
    CHECK(font_of(l) == &lv_font_montserrat_14);
    CHECK_COLOR(lv_obj_get_style_text_color(l, LV_PART_MAIN), palette()->foreground);
    lv_theme_shadcn_set_text_role(l, (lv_theme_shadcn_text_t)99);
    lv_theme_shadcn_set_text_role(NULL, LV_THEME_SHADCN_TEXT_H1);
    CHECK(font_of(l) == &lv_font_montserrat_14);
}

static void text_role_follows_palette(void)
{
    lv_obj_t * l = label();
    lv_theme_shadcn_set_text_role(l, LV_THEME_SHADCN_TEXT_MUTED);
    lv_theme_shadcn_set_palette(harness_display(), lv_theme_shadcn_palette_zinc_light());
    CHECK_COLOR(lv_obj_get_style_text_color(l, LV_PART_MAIN), lv_theme_shadcn_palette_zinc_light()->muted_fg);
}

static void text_role_is_inherited_by_children(void)
{
    lv_obj_t * box = lv_obj_create(lv_screen_active());
    lv_obj_t * child = lv_label_create(box);
    lv_theme_shadcn_set_text_role(box, LV_THEME_SHADCN_TEXT_MUTED);
    CHECK(font_of(child) == &lv_font_montserrat_12);
    CHECK_COLOR(lv_obj_get_style_text_color(child, LV_PART_MAIN), palette()->muted_fg);
}

static void heading_fonts_fall_back_down_the_chain(void)
{
    lv_obj_t * h1 = label();
    lv_obj_t * h2 = label();
    lv_obj_t * h3 = label();
    lv_theme_shadcn_set_text_role(h1, LV_THEME_SHADCN_TEXT_H1);
    lv_theme_shadcn_set_text_role(h2, LV_THEME_SHADCN_TEXT_H2);
    lv_theme_shadcn_set_text_role(h3, LV_THEME_SHADCN_TEXT_H3);
    CHECK(font_of(h1) == &lv_font_montserrat_20);
    CHECK(font_of(h2) == &lv_font_montserrat_20);
    CHECK(font_of(h3) == &lv_font_montserrat_20);

    lv_theme_shadcn_fonts_t only_h2 = *harness_fonts();
    only_h2.h2 = &lv_font_montserrat_28;
    init_with(&only_h2);
    CHECK(font_of(h1) == &lv_font_montserrat_28);
    CHECK(font_of(h2) == &lv_font_montserrat_28);
    CHECK(font_of(h3) == &lv_font_montserrat_20);

    lv_theme_shadcn_fonts_t only_h3 = *harness_fonts();
    only_h3.h3 = &lv_font_montserrat_24;
    init_with(&only_h3);
    CHECK(font_of(h1) == &lv_font_montserrat_24);
    CHECK(font_of(h2) == &lv_font_montserrat_24);
    CHECK(font_of(h3) == &lv_font_montserrat_24);

    lv_theme_shadcn_fonts_t normal_only = { NULL, &lv_font_montserrat_14, NULL, NULL, NULL, NULL, NULL };
    init_with(&normal_only);
    CHECK(font_of(h1) == &lv_font_montserrat_14);
    init_with(NULL);
    CHECK(font_of(h1) == LV_FONT_DEFAULT);
}

static lv_obj_t * role_obj(lv_theme_shadcn_role_t role)
{
    lv_obj_t * obj = lv_obj_create(lv_screen_active());
    lv_theme_shadcn_set_role(obj, role);
    return obj;
}

static void badges_are_pills(void)
{
    const lv_theme_shadcn_palette_t * p = palette();
    static const lv_theme_shadcn_role_t badges[] = {
        LV_THEME_SHADCN_ROLE_BADGE, LV_THEME_SHADCN_ROLE_BADGE_SECONDARY,
        LV_THEME_SHADCN_ROLE_BADGE_OUTLINE, LV_THEME_SHADCN_ROLE_BADGE_DESTRUCTIVE
    };
    lv_obj_t * objs[4];
    for(int i = 0; i < 4; i++) {
        objs[i] = role_obj(badges[i]);
        lv_obj_t * text = lv_label_create(objs[i]);
        lv_label_set_text(text, "Badge");
        CHECK_INT(lv_obj_get_style_radius(objs[i], LV_PART_MAIN), LV_RADIUS_CIRCLE);
        CHECK_INT(lv_obj_get_style_pad_left(objs[i], LV_PART_MAIN), dp(8));
        CHECK_INT(lv_obj_get_style_pad_top(objs[i], LV_PART_MAIN), dp(2));
        CHECK_INT(lv_obj_get_style_border_width(objs[i], LV_PART_MAIN), dp(1));
        CHECK(font_of(objs[i]) == &lv_font_montserrat_12);
        CHECK(font_of(text) == &lv_font_montserrat_12);
        CHECK_INT(lv_obj_get_style_width(objs[i], LV_PART_MAIN), LV_SIZE_CONTENT);
        CHECK_INT(lv_obj_get_style_height(objs[i], LV_PART_MAIN), LV_SIZE_CONTENT);
    }
    lv_obj_update_layout(lv_screen_active());
    CHECK(lv_obj_get_height(objs[0]) < dp(28));
    CHECK_COLOR(lv_obj_get_style_bg_color(objs[0], LV_PART_MAIN), p->primary);
    CHECK_COLOR(lv_obj_get_style_text_color(objs[0], LV_PART_MAIN), p->primary_fg);
    CHECK_INT(lv_obj_get_style_border_opa(objs[0], LV_PART_MAIN), LV_OPA_TRANSP);
    CHECK_COLOR(lv_obj_get_style_bg_color(objs[1], LV_PART_MAIN), p->secondary);
    CHECK_COLOR(lv_obj_get_style_text_color(objs[1], LV_PART_MAIN), p->secondary_fg);
    CHECK_INT(lv_obj_get_style_bg_opa(objs[2], LV_PART_MAIN), LV_OPA_TRANSP);
    CHECK_COLOR(lv_obj_get_style_border_color(objs[2], LV_PART_MAIN), p->border);
    CHECK_INT(lv_obj_get_style_border_opa(objs[2], LV_PART_MAIN), LV_OPA_COVER);
    CHECK_COLOR(lv_obj_get_style_text_color(objs[2], LV_PART_MAIN), p->foreground);
    CHECK_COLOR(lv_obj_get_style_bg_color(objs[3], LV_PART_MAIN), p->destructive);
    CHECK_COLOR(lv_obj_get_style_text_color(objs[3], LV_PART_MAIN), p->destructive_fg);
    CHECK_INT(lv_obj_get_style_bg_opa(objs[3], LV_PART_MAIN), LV_OPA_60);
    lv_theme_shadcn_set_palette(harness_display(), lv_theme_shadcn_palette_zinc_light());
    CHECK_INT(lv_obj_get_style_bg_opa(objs[3], LV_PART_MAIN), LV_OPA_COVER);
}

static void badge_works_on_a_label(void)
{
    lv_obj_t * l = label();
    lv_theme_shadcn_set_role(l, LV_THEME_SHADCN_ROLE_BADGE);
    CHECK_INT(lv_obj_get_style_bg_opa(l, LV_PART_MAIN), LV_OPA_COVER);
    CHECK_COLOR(lv_obj_get_style_bg_color(l, LV_PART_MAIN), palette()->primary);
    CHECK(font_of(l) == &lv_font_montserrat_12);
}

static void alerts_are_bordered_boxes(void)
{
    const lv_theme_shadcn_palette_t * p = palette();
    lv_obj_t * alert = role_obj(LV_THEME_SHADCN_ROLE_ALERT);
    lv_obj_t * destructive = role_obj(LV_THEME_SHADCN_ROLE_ALERT_DESTRUCTIVE);
    lv_obj_t * objs[2] = { alert, destructive };
    for(int i = 0; i < 2; i++) {
        CHECK_COLOR(lv_obj_get_style_bg_color(objs[i], LV_PART_MAIN), p->card);
        CHECK_INT(lv_obj_get_style_bg_opa(objs[i], LV_PART_MAIN), LV_OPA_COVER);
        CHECK_COLOR(lv_obj_get_style_border_color(objs[i], LV_PART_MAIN), p->border);
        CHECK_INT(lv_obj_get_style_border_width(objs[i], LV_PART_MAIN), dp(1));
        CHECK_INT(lv_obj_get_style_radius(objs[i], LV_PART_MAIN), dp(10));
        CHECK_INT(lv_obj_get_style_pad_left(objs[i], LV_PART_MAIN), dp(16));
        CHECK_INT(lv_obj_get_style_pad_top(objs[i], LV_PART_MAIN), dp(12));
        CHECK_INT(lv_obj_get_style_width(objs[i], LV_PART_MAIN), lv_pct(100));
        CHECK_INT(lv_obj_get_style_height(objs[i], LV_PART_MAIN), LV_SIZE_CONTENT);
    }
    CHECK_COLOR(lv_obj_get_style_text_color(alert, LV_PART_MAIN), p->card_fg);
    CHECK_COLOR(lv_obj_get_style_text_color(destructive, LV_PART_MAIN), p->destructive);
}

static void separators_are_hairlines(void)
{
    const lv_theme_shadcn_palette_t * p = palette();
    lv_obj_t * row = role_obj(LV_THEME_SHADCN_ROLE_SEPARATOR);
    lv_obj_t * column = role_obj(LV_THEME_SHADCN_ROLE_SEPARATOR_VERTICAL);
    CHECK_COLOR(lv_obj_get_style_bg_color(row, LV_PART_MAIN), p->border);
    CHECK_INT(lv_obj_get_style_bg_opa(row, LV_PART_MAIN), LV_OPA_COVER);
    CHECK_INT(lv_obj_get_style_border_width(row, LV_PART_MAIN), 0);
    CHECK_INT(lv_obj_get_style_radius(row, LV_PART_MAIN), 0);
    CHECK_INT(lv_obj_get_style_width(row, LV_PART_MAIN), lv_pct(100));
    CHECK_INT(lv_obj_get_style_height(row, LV_PART_MAIN), dp(1));
    CHECK_INT(lv_obj_get_style_width(column, LV_PART_MAIN), dp(1));
    CHECK_INT(lv_obj_get_style_height(column, LV_PART_MAIN), lv_pct(100));
    lv_obj_update_layout(lv_screen_active());
    CHECK_INT(lv_obj_get_height(row), dp(1));
    CHECK_INT(lv_obj_get_width(row), lv_obj_get_content_width(lv_screen_active()));
    CHECK_INT(lv_obj_get_width(column), dp(1));
}

static void skeleton_pulses(void)
{
    const lv_theme_shadcn_palette_t * p = palette();
    lv_obj_t * sk = role_obj(LV_THEME_SHADCN_ROLE_SKELETON);
    CHECK_COLOR(lv_obj_get_style_bg_color(sk, LV_PART_MAIN), p->muted);
    CHECK_INT(lv_obj_get_style_radius(sk, LV_PART_MAIN), dp(8));
    CHECK_INT(lv_obj_get_style_border_width(sk, LV_PART_MAIN), 0);
    CHECK(lv_anim_count_running() >= 1);
    lv_opa_t lowest = LV_OPA_COVER;
    lv_opa_t highest = LV_OPA_TRANSP;
    for(int i = 0; i < 120; i++) {
        lv_tick_inc(20);
        lv_timer_handler();
        lv_opa_t opa = lv_obj_get_style_opa(sk, LV_PART_MAIN);
        if(opa < lowest) lowest = opa;
        if(opa > highest) highest = opa;
    }
    CHECK(lowest <= LV_OPA_50 + 2);
    CHECK(lowest >= LV_OPA_50 - 2);
    CHECK(highest >= LV_OPA_COVER - 2);
    lv_theme_shadcn_set_role(sk, LV_THEME_SHADCN_ROLE_NONE);
    CHECK_INT(lv_obj_get_style_opa(sk, LV_PART_MAIN), LV_OPA_COVER);
    CHECK_INT(lv_anim_count_running(), 0);
}

static void skeleton_animation_dies_with_object(void)
{
    lv_obj_t * parent = lv_obj_create(lv_screen_active());
    for(int i = 0; i < 4; i++) role_obj(LV_THEME_SHADCN_ROLE_SKELETON);
    lv_obj_t * child = lv_obj_create(parent);
    lv_theme_shadcn_set_role(child, LV_THEME_SHADCN_ROLE_SKELETON);
    lv_theme_shadcn_set_role(child, LV_THEME_SHADCN_ROLE_SKELETON);
    CHECK_INT(lv_anim_count_running(), 5);
    lv_obj_delete(parent);
    CHECK_INT(lv_anim_count_running(), 4);
    lv_obj_clean(lv_screen_active());
    CHECK_INT(lv_anim_count_running(), 0);
    harness_settle();
    lv_obj_t * sk = role_obj(LV_THEME_SHADCN_ROLE_SKELETON);
    lv_obj_delete_async(sk);
    harness_settle();
    CHECK_INT(lv_anim_count_running(), 0);
}

static void skeleton_animation_stops_on_deinit(void)
{
    lv_obj_t * sk = role_obj(LV_THEME_SHADCN_ROLE_SKELETON);
    harness_settle();
    lv_theme_shadcn_deinit(harness_display());
    CHECK_INT(lv_anim_count_running(), 0);
    CHECK_INT(lv_obj_get_style_opa(sk, LV_PART_MAIN), LV_OPA_COVER);
    harness_settle();
}

static void reduced_motion_disables_pulse(void)
{
    lv_theme_shadcn_init(harness_display(), lv_theme_shadcn_palette_zinc_dark(), harness_fonts(),
                         LV_THEME_SHADCN_FLAG_REDUCED_MOTION);
    lv_obj_t * sk = role_obj(LV_THEME_SHADCN_ROLE_SKELETON);
    CHECK_INT(lv_anim_count_running(), 0);
    harness_settle();
    CHECK_INT(lv_obj_get_style_opa(sk, LV_PART_MAIN), LV_OPA_COVER);
    CHECK_COLOR(lv_obj_get_style_bg_color(sk, LV_PART_MAIN), palette()->muted);
}

static void card_parts_follow_shadcn(void)
{
    const lv_theme_shadcn_palette_t * p = palette();
    lv_obj_t * card = lv_obj_create(lv_screen_active());
    lv_obj_t * header = lv_obj_create(card);
    lv_theme_shadcn_set_role(header, LV_THEME_SHADCN_ROLE_CARD_HEADER);
    lv_obj_t * title = lv_label_create(header);
    lv_theme_shadcn_set_role(title, LV_THEME_SHADCN_ROLE_CARD_TITLE);
    lv_obj_t * description = lv_label_create(header);
    lv_theme_shadcn_set_role(description, LV_THEME_SHADCN_ROLE_CARD_DESCRIPTION);
    lv_obj_t * content = lv_obj_create(card);
    lv_theme_shadcn_set_role(content, LV_THEME_SHADCN_ROLE_CARD_CONTENT);
    lv_obj_t * footer = lv_obj_create(card);
    lv_theme_shadcn_set_role(footer, LV_THEME_SHADCN_ROLE_CARD_FOOTER);

    lv_obj_t * parts[3] = { header, content, footer };
    for(int i = 0; i < 3; i++) {
        CHECK_INT(lv_obj_get_style_bg_opa(parts[i], LV_PART_MAIN), LV_OPA_TRANSP);
        CHECK_INT(lv_obj_get_style_border_width(parts[i], LV_PART_MAIN), 0);
        CHECK_INT(lv_obj_get_style_pad_left(parts[i], LV_PART_MAIN), 0);
        CHECK_INT(lv_obj_get_style_pad_top(parts[i], LV_PART_MAIN), 0);
        CHECK_INT(lv_obj_get_style_width(parts[i], LV_PART_MAIN), lv_pct(100));
        CHECK_INT(lv_obj_get_style_height(parts[i], LV_PART_MAIN), LV_SIZE_CONTENT);
    }
    CHECK_INT(lv_obj_get_style_pad_row(header, LV_PART_MAIN), dp(4));
    CHECK_INT(lv_obj_get_style_flex_flow(header, LV_PART_MAIN), LV_FLEX_FLOW_COLUMN);
    CHECK_INT(lv_obj_get_style_flex_flow(footer, LV_PART_MAIN), LV_FLEX_FLOW_ROW);
    CHECK_INT(lv_obj_get_style_flex_cross_place(footer, LV_PART_MAIN), LV_FLEX_ALIGN_CENTER);
    CHECK_INT(lv_obj_get_style_pad_column(footer, LV_PART_MAIN), dp(8));
    CHECK(font_of(title) == &lv_font_montserrat_20);
    CHECK_COLOR(lv_obj_get_style_text_color(title, LV_PART_MAIN), p->card_fg);
    CHECK(font_of(description) == &lv_font_montserrat_12);
    CHECK_COLOR(lv_obj_get_style_text_color(description, LV_PART_MAIN), p->muted_fg);
}

static void role_replaces_previous(void)
{
    const lv_theme_shadcn_palette_t * p = palette();
    lv_obj_t * obj = role_obj(LV_THEME_SHADCN_ROLE_BADGE_DESTRUCTIVE);
    lv_theme_shadcn_set_role(obj, LV_THEME_SHADCN_ROLE_SEPARATOR);
    CHECK_COLOR(lv_obj_get_style_bg_color(obj, LV_PART_MAIN), p->border);
    CHECK_INT(lv_obj_get_style_radius(obj, LV_PART_MAIN), 0);
    lv_theme_shadcn_set_role(obj, LV_THEME_SHADCN_ROLE_NONE);
    CHECK_COLOR(lv_obj_get_style_bg_color(obj, LV_PART_MAIN), p->card);
    CHECK_INT(lv_obj_get_style_radius(obj, LV_PART_MAIN), dp(14));
    CHECK_INT(lv_obj_get_style_width(obj, LV_PART_MAIN), LV_DPI_DEF);
    lv_theme_shadcn_set_role(obj, (lv_theme_shadcn_role_t)99);
    lv_theme_shadcn_set_role(obj, (lv_theme_shadcn_role_t) -1);
    lv_theme_shadcn_set_role(NULL, LV_THEME_SHADCN_ROLE_BADGE);
    CHECK_COLOR(lv_obj_get_style_bg_color(obj, LV_PART_MAIN), p->card);
}

static void text_and_component_roles_are_independent(void)
{
    lv_obj_t * l = label();
    lv_theme_shadcn_set_role(l, LV_THEME_SHADCN_ROLE_BADGE_OUTLINE);
    lv_theme_shadcn_set_text_role(l, LV_THEME_SHADCN_TEXT_H3);
    lv_theme_shadcn_set_text_role(l, LV_THEME_SHADCN_TEXT_DEFAULT);
    CHECK(font_of(l) == &lv_font_montserrat_12);
    CHECK_COLOR(lv_obj_get_style_border_color(l, LV_PART_MAIN), palette()->border);
}

static void roles_follow_palette(void)
{
    lv_obj_t * badge = role_obj(LV_THEME_SHADCN_ROLE_BADGE);
    lv_obj_t * alert = role_obj(LV_THEME_SHADCN_ROLE_ALERT_DESTRUCTIVE);
    lv_obj_t * sep = role_obj(LV_THEME_SHADCN_ROLE_SEPARATOR);
    lv_theme_shadcn_palette_t blue;
    lv_theme_shadcn_palette_make(&blue, LV_THEME_SHADCN_BASE_SLATE, LV_THEME_SHADCN_ACCENT_BLUE, false);
    lv_theme_shadcn_set_palette(harness_display(), &blue);
    CHECK_COLOR(lv_obj_get_style_bg_color(badge, LV_PART_MAIN), blue.primary);
    CHECK_COLOR(lv_obj_get_style_text_color(alert, LV_PART_MAIN), blue.destructive);
    CHECK_COLOR(lv_obj_get_style_bg_color(sep, LV_PART_MAIN), blue.border);
}

static void roles_follow_radius(void)
{
    lv_obj_t * alert = role_obj(LV_THEME_SHADCN_ROLE_ALERT);
    lv_obj_t * sk = role_obj(LV_THEME_SHADCN_ROLE_SKELETON);
    lv_obj_t * badge = role_obj(LV_THEME_SHADCN_ROLE_BADGE);
    lv_theme_shadcn_set_radius(harness_display(), LV_THEME_SHADCN_RADIUS_NONE);
    CHECK_INT(lv_obj_get_style_radius(alert, LV_PART_MAIN), 0);
    CHECK_INT(lv_obj_get_style_radius(sk, LV_PART_MAIN), 0);
    CHECK_INT(lv_obj_get_style_radius(badge, LV_PART_MAIN), LV_RADIUS_CIRCLE);
}

int main(void)
{
    RUN(text_roles_pick_fonts);
    RUN(text_roles_pick_colors_and_spacing);
    RUN(text_role_replaces_previous);
    RUN(text_role_follows_palette);
    RUN(text_role_is_inherited_by_children);
    RUN(heading_fonts_fall_back_down_the_chain);
    RUN(badges_are_pills);
    RUN(badge_works_on_a_label);
    RUN(alerts_are_bordered_boxes);
    RUN(separators_are_hairlines);
    RUN(skeleton_pulses);
    RUN(skeleton_animation_dies_with_object);
    RUN(skeleton_animation_stops_on_deinit);
    RUN(reduced_motion_disables_pulse);
    RUN(card_parts_follow_shadcn);
    RUN(role_replaces_previous);
    RUN(text_and_component_roles_are_independent);
    RUN(roles_follow_palette);
    RUN(roles_follow_radius);
    return harness_result();
}
