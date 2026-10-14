#include "harness.h"

int harness_failures;

static lv_display_t * display;
static uint8_t draw_buffer[480 * 40 * 4];

static void flush_cb(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map)
{
    (void)area;
    (void)px_map;
    lv_display_flush_ready(disp);
}

lv_display_t * harness_create_display(void)
{
    lv_display_t * disp = lv_display_create(480, 320);
    lv_display_set_buffers(disp, draw_buffer, NULL, sizeof(draw_buffer), LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(disp, flush_cb);
    return disp;
}

const lv_theme_shadcn_fonts_t * harness_fonts(void)
{
    static const lv_theme_shadcn_fonts_t fonts = {
        &lv_font_montserrat_12, &lv_font_montserrat_14, &lv_font_montserrat_20, NULL
    };
    return &fonts;
}

void harness_begin(const lv_theme_shadcn_palette_t * palette)
{
    (void)palette;
    lv_init();
    display = harness_create_display();
}

void harness_end(void)
{
    lv_deinit();
    display = NULL;
}

void harness_settle(void)
{
    for(int i = 0; i < 20; i++) {
        lv_tick_inc(20);
        lv_timer_handler();
    }
}

lv_display_t * harness_display(void)
{
    return display;
}

int harness_result(void)
{
    printf(harness_failures ? "FAILED (%d)\n" : "OK\n", harness_failures);
    return harness_failures ? 1 : 0;
}
