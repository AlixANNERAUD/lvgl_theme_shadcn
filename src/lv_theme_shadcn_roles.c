#include "lv_theme_shadcn_private.h"

static void pulse_cb(void * var, int32_t value)
{
    lv_obj_set_style_opa(var, (lv_opa_t)value, 0);
}

static void start_pulse(lv_obj_t * obj)
{
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_exec_cb(&a, pulse_cb);
    lv_anim_set_values(&a, LV_OPA_COVER, LV_OPA_50);
    lv_anim_set_duration(&a, LV_THEME_SHADCN_PULSE_MS);
    lv_anim_set_reverse_duration(&a, LV_THEME_SHADCN_PULSE_MS);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);
    lv_anim_start(&a);
}

void lv_theme_shadcn_stop_pulse(lv_obj_t * obj)
{
    if(lv_anim_delete(obj, pulse_cb)) lv_obj_remove_local_style_prop(obj, LV_STYLE_OPA, 0);
}

void lv_theme_shadcn_set_role(lv_obj_t * obj, lv_theme_shadcn_role_t role)
{
    lv_theme_shadcn_state_t * st = lv_theme_shadcn_state_from_obj(obj);
    if(st == NULL || (int)role < 0 || role >= LV_THEME_SHADCN_ROLE_COUNT) return;
#if LV_USE_CHECKBOX
    bool checkbox = lv_obj_check_type(obj, &lv_checkbox_class);
#else
    bool checkbox = false;
#endif
    if(role == LV_THEME_SHADCN_ROLE_RADIO && !checkbox) return;
    lv_theme_shadcn_stop_pulse(obj);
    lv_style_t * radio = &S(st)->role[LV_THEME_SHADCN_ROLE_RADIO - 1];
    bool was_radio = lv_obj_get_style_enabled(obj, radio, LV_PART_INDICATOR);
    for(int i = 0; i < LV_THEME_SHADCN_ROLE_COUNT - 1; i++) {
        lv_obj_remove_style(obj, &S(st)->role[i], LV_PART_ANY | LV_STATE_ANY);
    }
    if(was_radio && role != LV_THEME_SHADCN_ROLE_RADIO) lv_obj_set_radio_button(obj, false);
    if(role == LV_THEME_SHADCN_ROLE_NONE) return;
    if(role == LV_THEME_SHADCN_ROLE_RADIO) {
        /* Also on the states the checkbox styles: their border colour and check mark would otherwise win. */
        lv_obj_add_style(obj, radio, LV_PART_INDICATOR);
        lv_obj_add_style(obj, radio, LV_PART_INDICATOR | LV_STATE_CHECKED);
        lv_obj_add_style(obj, radio, LV_PART_INDICATOR | LV_STATE_CHECKED | LV_STATE_HOVERED);
        lv_obj_set_radio_button(obj, true);
        return;
    }
    lv_obj_add_style(obj, &S(st)->role[role - 1], 0);
    if(role == LV_THEME_SHADCN_ROLE_SKELETON && (st->flags & LV_THEME_SHADCN_FLAG_REDUCED_MOTION) == 0) {
        start_pulse(obj);
    }
}
