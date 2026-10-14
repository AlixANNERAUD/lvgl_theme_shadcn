#include <string.h>
#include "harness.h"
#include "lv_theme_shadcn_scenes.h"

static bool children_inside(lv_obj_t * parent)
{
    lv_area_t outer;
    lv_obj_get_coords(parent, &outer);
    for(uint32_t i = 0; i < lv_obj_get_child_count(parent); i++) {
        lv_area_t inner;
        lv_obj_get_coords(lv_obj_get_child(parent, i), &inner);
        if(inner.x1 < outer.x1 || inner.y1 < outer.y1 || inner.x2 > outer.x2 || inner.y2 > outer.y2) return false;
    }
    return true;
}

static void every_scene_fits_the_screen(void)
{
    static const char * const names[] = { "buttons", "toggles", "ranges", "inputs", "collections", "overlays" };
    uint32_t count = 0;
    const lv_theme_shadcn_scene_t * scenes = lv_theme_shadcn_scenes(&count);
    CHECK_INT(count, 6);
    if(count != 6) return;
    for(uint32_t i = 0; i < count; i++) {
        CHECK(strcmp(scenes[i].name, names[i]) == 0);
        lv_obj_t * screen = lv_obj_create(NULL);
        lv_obj_set_size(screen, 480, 320);
        lv_screen_load(screen);
        scenes[i].build(screen);
        harness_settle();
        lv_obj_update_layout(screen);
        CHECK(lv_obj_get_child_count(screen) >= 1);
        CHECK(lv_obj_get_scroll_bottom(screen) <= 0);
        CHECK(lv_obj_get_scroll_right(screen) <= 0);
        CHECK(children_inside(screen));
        for(uint32_t c = 0; c < lv_obj_get_child_count(screen); c++) CHECK(children_inside(lv_obj_get_child(screen, c)));
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
