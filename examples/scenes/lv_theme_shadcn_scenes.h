#ifndef LV_THEME_SHADCN_SCENES_H
#define LV_THEME_SHADCN_SCENES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lv_theme_shadcn.h"

typedef struct {
    const char * name;
    void (*build)(lv_obj_t * parent);
} lv_theme_shadcn_scene_t;

const lv_theme_shadcn_scene_t * lv_theme_shadcn_scenes(uint32_t * count);

#ifdef __cplusplus
}
#endif

#endif
