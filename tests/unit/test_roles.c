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

int main(void)
{
    RUN(text_roles_pick_fonts);
    RUN(text_roles_pick_colors_and_spacing);
    RUN(text_role_replaces_previous);
    RUN(text_role_follows_palette);
    RUN(text_role_is_inherited_by_children);
    RUN(heading_fonts_fall_back_down_the_chain);
    return harness_result();
}
