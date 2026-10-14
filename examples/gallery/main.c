#include <SDL2/SDL.h>
#include "lv_theme_shadcn.h"
#include "lv_theme_shadcn_scenes.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 480
#define SCENE_WIDTH 480
#define SCENE_HEIGHT 320

static lv_display_t * display;
static lv_obj_t * base_dropdown;
static lv_obj_t * accent_dropdown;
static lv_obj_t * light_switch;

static void palette_changed_cb(lv_event_t * e)
{
    (void)e;
    lv_theme_shadcn_palette_t palette;
    lv_theme_shadcn_palette_make(&palette, (lv_theme_shadcn_base_t)lv_dropdown_get_selected(base_dropdown),
                                 (lv_theme_shadcn_accent_t)lv_dropdown_get_selected(accent_dropdown),
                                 !lv_obj_has_state(light_switch, LV_STATE_CHECKED));
    lv_theme_shadcn_set_palette(display, &palette);
}

static lv_obj_t * add_dropdown(lv_obj_t * parent, const char * caption, int count, const char * (*name)(int),
                               int selected)
{
    char options[256] = "";
    size_t used = 0;
    for(int i = 0; i < count; i++) {
        int written = lv_snprintf(options + used, sizeof(options) - used, i ? "\n%s" : "%s", name(i));
        if(written < 0 || (size_t)written >= sizeof(options) - used) break;
        used += (size_t)written;
    }

    lv_obj_t * label = lv_label_create(parent);
    lv_label_set_text(label, caption);

    lv_obj_t * dropdown = lv_dropdown_create(parent);
    lv_dropdown_set_options(dropdown, options);
    lv_dropdown_set_selected(dropdown, (uint32_t)selected);
    lv_obj_set_width(dropdown, 120);
    lv_obj_add_event_cb(dropdown, palette_changed_cb, LV_EVENT_VALUE_CHANGED, NULL);
    return dropdown;
}

static const char * base_name(int index)
{
    return lv_theme_shadcn_base_name((lv_theme_shadcn_base_t)index);
}

static const char * accent_name(int index)
{
    return lv_theme_shadcn_accent_name((lv_theme_shadcn_accent_t)index);
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

    base_dropdown = add_dropdown(header, "Base", LV_THEME_SHADCN_BASE_COUNT, base_name, LV_THEME_SHADCN_BASE_ZINC);
    accent_dropdown = add_dropdown(header, "Accent", LV_THEME_SHADCN_ACCENT_COUNT, accent_name,
                                   LV_THEME_SHADCN_ACCENT_NONE);

    lv_obj_t * caption = lv_label_create(header);
    lv_label_set_text(caption, "Light");

    light_switch = lv_switch_create(header);
    lv_obj_add_event_cb(light_switch, palette_changed_cb, LV_EVENT_VALUE_CHANGED, NULL);
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
