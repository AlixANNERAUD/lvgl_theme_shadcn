#include "harness.h"

static const lv_theme_shadcn_palette_t * dark(void)
{
    return lv_theme_shadcn_palette_zinc_dark();
}

static void msgbox_parts(void)
{
    lv_obj_t * mb = lv_msgbox_create(NULL);
    lv_msgbox_add_title(mb, "Title");
    lv_msgbox_add_text(mb, "Body");
    lv_obj_t * ok = lv_msgbox_add_footer_button(mb, "OK");
    lv_obj_t * close = lv_msgbox_add_close_button(mb);
    lv_obj_t * backdrop = lv_obj_get_parent(mb);
    CHECK_COLOR(lv_obj_get_style_bg_color(backdrop, LV_PART_MAIN), lv_color_black());
    CHECK_INT(lv_obj_get_style_bg_opa(backdrop, LV_PART_MAIN), LV_OPA_50);
    CHECK_COLOR(lv_obj_get_style_bg_color(mb, LV_PART_MAIN), dark()->popover);
    CHECK_INT(lv_obj_get_style_radius(mb, LV_PART_MAIN), lv_display_dpx(harness_display(), 10));
    CHECK_INT(lv_obj_get_style_shadow_width(mb, LV_PART_MAIN), 0);
    CHECK_COLOR(lv_obj_get_style_bg_color(ok, LV_PART_MAIN), dark()->primary);
    CHECK_INT(lv_obj_get_style_bg_opa(close, LV_PART_MAIN), LV_OPA_TRANSP);
    CHECK(lv_obj_get_style_text_font(lv_msgbox_get_header(mb), LV_PART_MAIN) == &lv_font_montserrat_20);
    CHECK_COLOR(lv_obj_get_style_text_color(lv_msgbox_get_content(mb), LV_PART_MAIN), dark()->muted_fg);
}

int main(void)
{
    RUN(msgbox_parts);
    return harness_result();
}
