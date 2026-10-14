#include <stdio.h>
#include "lv_theme_shadcn.h"

#define WIDTH 320
#define HEIGHT 240

static uint32_t buffer[WIDTH * 24];
static uint32_t flushes;

static void flush_cb(lv_display_t * display, const lv_area_t * area, uint8_t * pixels)
{
    LV_UNUSED(area);
    LV_UNUSED(pixels);
    flushes++;
    lv_display_flush_ready(display);
}

int main(void)
{
    lv_init();

    lv_display_t * display = lv_display_create(WIDTH, HEIGHT);
    lv_display_set_buffers(display, buffer, NULL, sizeof(buffer), LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(display, flush_cb);

    static const lv_theme_shadcn_fonts_t fonts = {
        &lv_font_montserrat_12, &lv_font_montserrat_14, &lv_font_montserrat_20, NULL
    };
    if(lv_theme_shadcn_init(display, lv_theme_shadcn_palette_zinc_dark(), &fonts, LV_THEME_SHADCN_FLAG_NONE) == NULL) {
        return 1;
    }

    lv_obj_t * button = lv_button_create(lv_screen_active());
    lv_obj_center(button);
    lv_obj_t * label = lv_label_create(button);
    lv_label_set_text(label, "Hello shadcn");

    for(int i = 0; i < 10; i++) {
        lv_tick_inc(10);
        lv_timer_handler();
    }

    printf("lvgl_theme_shadcn: %u flushes\n", (unsigned)flushes);
    return flushes > 0 ? 0 : 1;
}
