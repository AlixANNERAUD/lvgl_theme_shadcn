#include <SDL2/SDL.h>
#include "lv_theme_shadcn.h"
#include "lv_theme_shadcn_scenes.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 480
#define SCENE_WIDTH 480
#define SCENE_HEIGHT 320

static lv_display_t * display;

static void light_switch_cb(lv_event_t * e)
{
    lv_obj_t * sw = lv_event_get_target_obj(e);
    bool light = lv_obj_has_state(sw, LV_STATE_CHECKED);
    lv_theme_shadcn_set_palette(display, light ? lv_theme_shadcn_palette_zinc_light() :
                                lv_theme_shadcn_palette_zinc_dark());
}

static void build_header(lv_obj_t * screen)
{
    lv_obj_t * header = lv_obj_create(screen);
    lv_obj_set_size(header, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(header, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(header, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scrollable(header, false);

    lv_obj_t * title = lv_label_create(header);
    lv_label_set_text(title, "lvgl_theme_shadcn");
    lv_obj_set_flex_grow(title, 1);

    lv_obj_t * caption = lv_label_create(header);
    lv_label_set_text(caption, "Light");

    lv_obj_t * sw = lv_switch_create(header);
    lv_obj_add_event_cb(sw, light_switch_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

static void build_tabs(lv_obj_t * screen)
{
    lv_obj_t * tabview = lv_tabview_create(screen);
    lv_obj_set_width(tabview, LV_PCT(100));
    lv_obj_set_flex_grow(tabview, 1);

    uint32_t count = 0;
    const lv_theme_shadcn_scene_t * scenes = lv_theme_shadcn_scenes(&count);
    for(uint32_t i = 0; i < count; i++) {
        lv_obj_t * tab = lv_tabview_add_tab(tabview, scenes[i].name);
        lv_obj_set_style_pad_all(tab, 0, 0);

        lv_obj_t * container = lv_obj_create(tab);
        lv_obj_set_size(container, SCENE_WIDTH, SCENE_HEIGHT);
        lv_obj_set_style_pad_all(container, 0, 0);
        lv_obj_set_style_border_width(container, 0, 0);
        lv_obj_set_style_radius(container, 0, 0);
        lv_obj_set_scrollable(container, false);
        lv_obj_align(container, LV_ALIGN_TOP_MID, 0, 0);
        scenes[i].build(container);
    }
}

int main(void)
{
    lv_init();

    display = lv_sdl_window_create(WINDOW_WIDTH, WINDOW_HEIGHT);
    lv_sdl_window_set_title(display, "lvgl_theme_shadcn gallery");
    lv_sdl_mouse_create();
    lv_sdl_mousewheel_create();
    lv_sdl_keyboard_create();

    static const lv_theme_shadcn_fonts_t fonts = {
        &lv_font_montserrat_12, &lv_font_montserrat_14, &lv_font_montserrat_20, NULL
    };
    lv_theme_shadcn_init(display, lv_theme_shadcn_palette_zinc_dark(), &fonts, LV_THEME_SHADCN_FLAG_NONE);

    lv_obj_t * screen = lv_screen_active();
    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(screen, 0, 0);
    lv_obj_set_style_pad_row(screen, 0, 0);
    build_header(screen);
    build_tabs(screen);

    while(1) {
        lv_timer_handler();
        SDL_Delay(5);
    }
    return 0;
}
