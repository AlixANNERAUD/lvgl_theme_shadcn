#include <string.h>
#include "harness.h"
#include "lv_theme_shadcn_scenes.h"

static void every_scene_fits_the_screen(void)
{
    static const char * const names[] = { "buttons", "toggles", "ranges", "inputs", "collections", "overlays" };
    uint32_t count = 0;
    const lv_theme_shadcn_scene_t * scenes = lv_theme_shadcn_scenes(&count);
    CHECK_INT(count, 6);
    for(uint32_t i = 0; i < count && i < 6; i++) {
        CHECK(strcmp(scenes[i].name, names[i]) == 0);
        lv_obj_t * screen = lv_obj_create(NULL);
        lv_obj_set_size(screen, 480, 320);
        lv_screen_load(screen);
        scenes[i].build(screen);
        harness_settle();
        lv_obj_update_layout(screen);
        printf("  %s\n", scenes[i].name);
        CHECK(lv_obj_get_child_count(screen) >= 1);
        CHECK(lv_obj_get_scroll_bottom(screen) <= 0);
        CHECK(lv_obj_get_scroll_right(screen) <= 0);
        lv_obj_t * old = screen;
        lv_screen_load(lv_obj_create(NULL));
        lv_obj_delete(old);
    }
}

int main(void)
{
    RUN(every_scene_fits_the_screen);
    return harness_result();
}
