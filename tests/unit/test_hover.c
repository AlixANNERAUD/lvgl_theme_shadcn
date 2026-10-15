#include "harness.h"

static lv_point_t pointer_point;
static lv_indev_state_t pointer_state;
static lv_indev_t * pointer;

static void pointer_read_cb(lv_indev_t * indev, lv_indev_data_t * data)
{
    (void)indev;
    data->point = pointer_point;
    data->state = pointer_state;
}

static void pointer_create_at(int32_t x, int32_t y)
{
    pointer_point.x = x;
    pointer_point.y = y;
    pointer_state = LV_INDEV_STATE_RELEASED;
    pointer = lv_indev_create();
    lv_indev_set_type(pointer, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(pointer, pointer_read_cb);
    lv_indev_set_display(pointer, harness_display());
    lv_indev_read(pointer);
}

static void pointer_create(void)
{
    pointer_create_at(1, 1);
}

static void pointer_at(lv_obj_t * obj, lv_indev_state_t state)
{
    lv_obj_update_layout(obj);
    lv_area_t a;
    lv_obj_get_coords(obj, &a);
    pointer_point.x = (a.x1 + a.x2) / 2;
    pointer_point.y = (a.y1 + a.y2) / 2;
    pointer_state = state;
    lv_indev_read(pointer);
    harness_settle();
}

static void pointer_away(void)
{
    pointer_point.x = 1;
    pointer_point.y = 1;
    pointer_state = LV_INDEV_STATE_RELEASED;
    lv_indev_read(pointer);
    harness_settle();
}

static const lv_theme_shadcn_palette_t * dark(void)
{
    return lv_theme_shadcn_palette_zinc_dark();
}

static lv_obj_t * centred_button(lv_theme_shadcn_variant_t variant)
{
    lv_obj_t * btn = lv_button_create(lv_screen_active());
    lv_obj_set_size(btn, 120, 40);
    lv_obj_center(btn);
    lv_theme_shadcn_set_variant(btn, variant);
    return btn;
}

static void mouse_hover_follows_shadcn(void)
{
    pointer_create();
    const lv_theme_shadcn_palette_t * p = dark();
    struct {
        lv_theme_shadcn_variant_t variant;
        lv_color_t bg;
        lv_color_t fg;
    } cases[] = {
        { LV_THEME_SHADCN_VARIANT_DEFAULT, lv_color_mix(p->primary, p->background, LV_OPA_90), p->primary_fg },
        { LV_THEME_SHADCN_VARIANT_SECONDARY, lv_color_mix(p->secondary, p->background, LV_OPA_80), p->secondary_fg },
        { LV_THEME_SHADCN_VARIANT_OUTLINE, p->accent, p->accent_fg },
        { LV_THEME_SHADCN_VARIANT_GHOST, p->accent, p->accent_fg },
        { LV_THEME_SHADCN_VARIANT_DESTRUCTIVE, lv_color_mix(p->destructive, p->background, LV_OPA_90), p->destructive_fg },
    };
    for(size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
        lv_obj_t * btn = centred_button(cases[i].variant);
        pointer_at(btn, LV_INDEV_STATE_RELEASED);
        CHECK(lv_obj_has_state(btn, LV_STATE_HOVERED));
        CHECK_COLOR(lv_obj_get_style_bg_color(btn, LV_PART_MAIN), cases[i].bg);
        CHECK_INT(lv_obj_get_style_bg_opa(btn, LV_PART_MAIN), LV_OPA_COVER);
        CHECK_COLOR(lv_obj_get_style_text_color(btn, LV_PART_MAIN), cases[i].fg);
        pointer_away();
        CHECK(!lv_obj_has_state(btn, LV_STATE_HOVERED));
        lv_obj_delete(btn);
    }
}

static void pressed_wins_over_hover(void)
{
    pointer_create();
    lv_obj_t * btn = centred_button(LV_THEME_SHADCN_VARIANT_DEFAULT);
    pointer_at(btn, LV_INDEV_STATE_RELEASED);
    pointer_at(btn, LV_INDEV_STATE_PRESSED);
    CHECK(lv_obj_has_state(btn, LV_STATE_HOVERED));
    CHECK_COLOR(lv_obj_get_style_bg_color(btn, LV_PART_MAIN), lv_color_mix(dark()->primary_fg, dark()->primary, 26));
}

static void touch_press_never_leaves_hover(void)
{
    pointer_create_at(0, 0);
    lv_obj_t * a = centred_button(LV_THEME_SHADCN_VARIANT_DEFAULT);
    lv_obj_t * b = centred_button(LV_THEME_SHADCN_VARIANT_OUTLINE);
    lv_obj_align(b, LV_ALIGN_TOP_LEFT, 20, 20);
    pointer_at(a, LV_INDEV_STATE_PRESSED);
    CHECK(lv_obj_has_state(a, LV_STATE_PRESSED));
    CHECK(!lv_obj_has_state(a, LV_STATE_HOVERED));
    pointer_at(a, LV_INDEV_STATE_RELEASED);
    CHECK(!lv_obj_has_state(a, LV_STATE_PRESSED));
    CHECK(!lv_obj_has_state(a, LV_STATE_HOVERED));
    CHECK_COLOR(lv_obj_get_style_bg_color(a, LV_PART_MAIN), dark()->primary);
    pointer_at(b, LV_INDEV_STATE_PRESSED);
    pointer_at(b, LV_INDEV_STATE_RELEASED);
    CHECK(!lv_obj_has_state(a, LV_STATE_HOVERED));
    CHECK(!lv_obj_has_state(b, LV_STATE_HOVERED));
    CHECK_INT(lv_obj_get_style_bg_opa(b, LV_PART_MAIN), LV_OPA_TRANSP);
}

static void variant_change_replaces_hover(void)
{
    lv_obj_t * btn = centred_button(LV_THEME_SHADCN_VARIANT_OUTLINE);
    lv_theme_shadcn_set_variant(btn, LV_THEME_SHADCN_VARIANT_DEFAULT);
    lv_obj_add_state(btn, LV_STATE_HOVERED);
    harness_settle();
    CHECK_COLOR(lv_obj_get_style_bg_color(btn, LV_PART_MAIN), lv_color_mix(dark()->primary, dark()->background, LV_OPA_90));
    CHECK_COLOR(lv_obj_get_style_text_color(btn, LV_PART_MAIN), dark()->primary_fg);
}

static void hover_follows_palette_changes(void)
{
    lv_obj_t * btn = centred_button(LV_THEME_SHADCN_VARIANT_SECONDARY);
    lv_obj_add_state(btn, LV_STATE_HOVERED);
    const lv_theme_shadcn_palette_t * light = lv_theme_shadcn_palette_zinc_light();
    lv_theme_shadcn_set_palette(harness_display(), light);
    harness_settle();
    CHECK_COLOR(lv_obj_get_style_bg_color(btn, LV_PART_MAIN), lv_color_mix(light->secondary, light->background, LV_OPA_80));
}

static void list_and_menu_items_hover_with_accent(void)
{
    lv_obj_t * list = lv_list_create(lv_screen_active());
    lv_obj_t * item = lv_list_add_button(list, NULL, "Item");
    lv_obj_add_state(item, LV_STATE_HOVERED);
    harness_settle();
    CHECK_COLOR(lv_obj_get_style_bg_color(item, LV_PART_MAIN), dark()->accent);
    CHECK_INT(lv_obj_get_style_bg_opa(item, LV_PART_MAIN), LV_OPA_COVER);
    CHECK_COLOR(lv_obj_get_style_text_color(item, LV_PART_MAIN), dark()->accent_fg);

    lv_obj_t * menu = lv_menu_create(lv_screen_active());
    lv_obj_t * page = lv_menu_page_create(menu, NULL);
    lv_obj_t * cont = lv_menu_cont_create(page);
    lv_obj_add_state(cont, LV_STATE_HOVERED);
    harness_settle();
    CHECK_COLOR(lv_obj_get_style_bg_color(cont, LV_PART_MAIN), dark()->accent);
}

static void toggles_hover_subtly(void)
{
    const lv_theme_shadcn_palette_t * p = dark();
    lv_obj_t * cb = lv_checkbox_create(lv_screen_active());
    lv_obj_add_state(cb, LV_STATE_HOVERED);
    harness_settle();
    CHECK_COLOR(lv_obj_get_style_bg_color(cb, LV_PART_INDICATOR), p->accent);
    lv_obj_add_state(cb, LV_STATE_CHECKED);
    harness_settle();
    CHECK_COLOR(lv_obj_get_style_bg_color(cb, LV_PART_INDICATOR), lv_color_mix(p->primary, p->background, LV_OPA_90));
    CHECK(lv_obj_get_style_bg_image_src(cb, LV_PART_INDICATOR) != NULL);

    lv_obj_t * sw = lv_switch_create(lv_screen_active());
    lv_obj_add_state(sw, LV_STATE_HOVERED);
    harness_settle();
    CHECK_COLOR(lv_obj_get_style_bg_color(sw, LV_PART_MAIN), lv_color_mix(p->input, p->foreground, LV_OPA_90));
    lv_obj_add_state(sw, LV_STATE_CHECKED);
    harness_settle();
    CHECK_COLOR(lv_obj_get_style_bg_color(sw, LV_PART_INDICATOR), lv_color_mix(p->primary, p->background, LV_OPA_90));
}

static void no_hover_without_state(void)
{
    lv_obj_t * btn = centred_button(LV_THEME_SHADCN_VARIANT_OUTLINE);
    lv_obj_t * cb = lv_checkbox_create(lv_screen_active());
    harness_settle();
    CHECK_INT(lv_obj_get_style_bg_opa(btn, LV_PART_MAIN), LV_OPA_TRANSP);
    CHECK_COLOR(lv_obj_get_style_bg_color(cb, LV_PART_INDICATOR), dark()->background);
}

int main(void)
{
    RUN(mouse_hover_follows_shadcn);
    RUN(pressed_wins_over_hover);
    RUN(touch_press_never_leaves_hover);
    RUN(variant_change_replaces_hover);
    RUN(hover_follows_palette_changes);
    RUN(list_and_menu_items_hover_with_accent);
    RUN(toggles_hover_subtly);
    RUN(no_hover_without_state);
    return harness_result();
}
