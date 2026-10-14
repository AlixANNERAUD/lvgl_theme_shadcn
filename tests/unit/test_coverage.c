#include "harness.h"

typedef lv_obj_t * (*create_fn)(lv_obj_t * parent);

static const create_fn creators[] = {
    lv_obj_create, lv_label_create, lv_button_create, lv_imagebutton_create, lv_buttonmatrix_create,
    lv_switch_create, lv_checkbox_create, lv_led_create, lv_slider_create, lv_bar_create, lv_arc_create,
    lv_scale_create, lv_textarea_create, lv_spinbox_create, lv_dropdown_create, lv_roller_create,
    lv_keyboard_create, lv_list_create, lv_table_create, lv_tabview_create, lv_calendar_create,
    lv_chart_create, lv_menu_create, lv_tileview_create, lv_win_create, lv_spangroup_create,
    lv_image_create, lv_line_create, lv_canvas_create,
};

static const lv_state_t states[] = {
    LV_STATE_DEFAULT, LV_STATE_PRESSED, LV_STATE_FOCUSED, LV_STATE_FOCUS_KEY, LV_STATE_CHECKED,
    LV_STATE_DISABLED, LV_STATE_CHECKED | LV_STATE_PRESSED,
};

static void every_widget_in_every_state_renders(void)
{
    const lv_theme_shadcn_palette_t * palettes[] = {
        lv_theme_shadcn_palette_zinc_dark(), lv_theme_shadcn_palette_zinc_light()
    };
    for(size_t p = 0; p < 2; p++) {
        lv_theme_shadcn_set_palette(harness_display(), palettes[p]);
        for(size_t s = 0; s < sizeof(states) / sizeof(states[0]); s++) {
            lv_obj_clean(lv_screen_active());
            for(size_t c = 0; c < sizeof(creators) / sizeof(creators[0]); c++) {
                lv_obj_t * obj = creators[c](lv_screen_active());
                lv_obj_add_state(obj, states[s]);
            }
            harness_settle();
            lv_obj_invalidate(lv_screen_active());
            lv_refr_now(harness_display());
        }
    }
    CHECK(true);
}

static void spinner_renders(void)
{
    lv_spinner_create(lv_screen_active());
    harness_settle();
    lv_refr_now(harness_display());
    CHECK(true);
}

int main(void)
{
    RUN(every_widget_in_every_state_renders);
    RUN(spinner_renders);
    return harness_result();
}
