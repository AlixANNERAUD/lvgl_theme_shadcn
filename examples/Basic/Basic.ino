#include <lvgl.h>
#include <lv_theme_shadcn.h>

static const uint32_t SCREEN_WIDTH = 320;
static const uint32_t SCREEN_HEIGHT = 240;
static const uint32_t DRAW_LINES = 20;

static uint16_t draw_buffer[SCREEN_WIDTH * DRAW_LINES];

static void flush_to_panel(lv_display_t * display, const lv_area_t * area, uint8_t * pixels)
{
    LV_UNUSED(area);
    LV_UNUSED(pixels);
    lv_display_flush_ready(display);
}

static uint32_t read_milliseconds(void)
{
    return millis();
}

static void build_screen(void)
{
    lv_obj_t * card = lv_obj_create(lv_screen_active());
    lv_obj_set_size(card, 280, LV_SIZE_CONTENT);
    lv_obj_center(card);
    lv_obj_set_flex_flow(card, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_gap(card, 12, 0);

    lv_obj_t * heading = lv_label_create(card);
    lv_label_set_text(heading, "Hello shadcn");
    lv_theme_shadcn_set_text_role(heading, LV_THEME_SHADCN_TEXT_H3);

    lv_obj_t * primary_button = lv_button_create(card);
    lv_obj_t * primary_label = lv_label_create(primary_button);
    lv_label_set_text(primary_label, "Primary");

    lv_obj_t * outline_button = lv_button_create(card);
    lv_theme_shadcn_set_variant(outline_button, LV_THEME_SHADCN_VARIANT_OUTLINE);
    lv_obj_t * outline_label = lv_label_create(outline_button);
    lv_label_set_text(outline_label, "Outline");
}

void setup()
{
    lv_init();
    lv_tick_set_cb(read_milliseconds);

    lv_display_t * display = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_display_set_color_format(display, LV_COLOR_FORMAT_RGB565);
    lv_display_set_buffers(display, draw_buffer, NULL, sizeof(draw_buffer), LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(display, flush_to_panel);

    static const lv_theme_shadcn_fonts_t fonts = {
        .small = &lv_font_montserrat_12,
        .normal = &lv_font_montserrat_14,
        .large = &lv_font_montserrat_16,
        .h1 = &lv_font_montserrat_24,
        .h2 = &lv_font_montserrat_20,
        .h3 = &lv_font_montserrat_16,
    };
    lv_theme_shadcn_init(display, lv_theme_shadcn_palette_zinc_dark(), &fonts, LV_THEME_SHADCN_FLAG_NONE);

    build_screen();
}

void loop()
{
    lv_timer_handler();
    delay(5);
}
