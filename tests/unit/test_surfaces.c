#include "harness.h"

static const lv_theme_shadcn_palette_t * dark(void)
{
    return lv_theme_shadcn_palette_zinc_dark();
}

static void plain_obj_is_a_card(void)
{
    lv_obj_t * card = lv_obj_create(lv_screen_active());
    CHECK_COLOR(lv_obj_get_style_bg_color(card, LV_PART_MAIN), dark()->card);
    CHECK_COLOR(lv_obj_get_style_text_color(card, LV_PART_MAIN), dark()->card_fg);
    CHECK_COLOR(lv_obj_get_style_border_color(card, LV_PART_MAIN), dark()->border);
    CHECK_INT(lv_obj_get_style_border_width(card, LV_PART_MAIN), lv_display_dpx(harness_display(), 1));
    CHECK_INT(lv_obj_get_style_radius(card, LV_PART_MAIN), lv_display_dpx(harness_display(), 8));
    CHECK_INT(lv_obj_get_style_pad_left(card, LV_PART_MAIN), lv_display_dpx(harness_display(), 16));
}

static void label_inherits_parent_text_color(void)
{
    lv_obj_t * card = lv_obj_create(lv_screen_active());
    lv_obj_t * label = lv_label_create(card);
    CHECK_COLOR(lv_obj_get_style_text_color(label, LV_PART_MAIN), dark()->card_fg);
    CHECK_INT(lv_obj_get_style_bg_opa(label, LV_PART_MAIN), LV_OPA_TRANSP);
}

static void set_surface_switches_background(void)
{
    lv_obj_t * box = lv_obj_create(lv_screen_active());
    lv_theme_shadcn_set_surface(box, LV_THEME_SHADCN_SURFACE_MUTED);
    CHECK_COLOR(lv_obj_get_style_bg_color(box, LV_PART_MAIN), dark()->muted);
    lv_theme_shadcn_set_surface(box, LV_THEME_SHADCN_SURFACE_POPOVER);
    CHECK_COLOR(lv_obj_get_style_bg_color(box, LV_PART_MAIN), dark()->popover);
    lv_theme_shadcn_set_surface(box, LV_THEME_SHADCN_SURFACE_CARD);
    CHECK_COLOR(lv_obj_get_style_bg_color(box, LV_PART_MAIN), dark()->card);
}

static void set_error_toggles_destructive_border(void)
{
    lv_obj_t * box = lv_obj_create(lv_screen_active());
    lv_theme_shadcn_set_error(box, true);
    CHECK_COLOR(lv_obj_get_style_border_color(box, LV_PART_MAIN), dark()->destructive);
    lv_theme_shadcn_set_error(box, false);
    CHECK_COLOR(lv_obj_get_style_border_color(box, LV_PART_MAIN), dark()->border);
}

static void disabled_obj_is_dimmed(void)
{
    lv_obj_t * box = lv_obj_create(lv_screen_active());
    lv_obj_add_state(box, LV_STATE_DISABLED);
    harness_settle();
    CHECK_INT(lv_obj_get_style_opa(box, LV_PART_MAIN), 97);
}

static void focus_key_shows_ring(void)
{
    lv_obj_t * box = lv_obj_create(lv_screen_active());
    lv_obj_add_state(box, LV_STATE_FOCUS_KEY);
    CHECK_INT(lv_obj_get_style_outline_width(box, LV_PART_MAIN), lv_display_dpx(harness_display(), 2));
    CHECK_COLOR(lv_obj_get_style_outline_color(box, LV_PART_MAIN), dark()->ring);
}

static void palette_switch_restyles_states(void)
{
    lv_obj_t * box = lv_obj_create(lv_screen_active());
    lv_obj_add_state(box, LV_STATE_FOCUS_KEY);
    lv_theme_shadcn_set_palette(harness_display(), lv_theme_shadcn_palette_zinc_light());
    CHECK_COLOR(lv_obj_get_style_bg_color(box, LV_PART_MAIN), lv_color_hex(0xffffff));
    CHECK_COLOR(lv_obj_get_style_outline_color(box, LV_PART_MAIN), lv_color_hex(0x9f9fa9));
}

static void tileview_and_win_parts(void)
{
    lv_obj_t * tv = lv_tileview_create(lv_screen_active());
    CHECK_INT(lv_obj_get_style_bg_opa(tv, LV_PART_MAIN), LV_OPA_TRANSP);
    lv_obj_t * win = lv_win_create(lv_screen_active());
    CHECK_COLOR(lv_obj_get_style_bg_color(lv_win_get_header(win), LV_PART_MAIN), dark()->muted);
    CHECK_COLOR(lv_obj_get_style_bg_color(lv_win_get_content(win), LV_PART_MAIN), dark()->background);
}

static void menu_section_is_card(void)
{
    lv_obj_t * menu = lv_menu_create(lv_screen_active());
    lv_obj_t * page = lv_menu_page_create(menu, NULL);
    lv_obj_t * section = lv_menu_section_create(page);
    lv_obj_t * row = lv_menu_cont_create(section);
    CHECK_COLOR(lv_obj_get_style_bg_color(menu, LV_PART_MAIN), dark()->background);
    CHECK_COLOR(lv_obj_get_style_bg_color(section, LV_PART_MAIN), dark()->card);
    lv_obj_add_state(row, LV_STATE_PRESSED);
    harness_settle();
    CHECK_COLOR(lv_obj_get_style_bg_color(row, LV_PART_MAIN), dark()->accent);
}

int main(void)
{
    RUN(plain_obj_is_a_card);
    RUN(label_inherits_parent_text_color);
    RUN(set_surface_switches_background);
    RUN(set_error_toggles_destructive_border);
    RUN(disabled_obj_is_dimmed);
    RUN(focus_key_shows_ring);
    RUN(palette_switch_restyles_states);
    RUN(tileview_and_win_parts);
    RUN(menu_section_is_card);
    return harness_result();
}
