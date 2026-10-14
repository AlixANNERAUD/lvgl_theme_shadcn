#include "lv_theme_shadcn_private.h"

static lv_theme_shadcn_state_t * states;

lv_theme_shadcn_state_t * lv_theme_shadcn_state_find(const lv_display_t * disp)
{
    for(lv_theme_shadcn_state_t * st = states; st; st = st->next) {
        if(st->disp == disp) return st;
    }
    return NULL;
}

lv_theme_shadcn_state_t * lv_theme_shadcn_state_from_obj(const lv_obj_t * obj)
{
    if(obj == NULL) return NULL;
    lv_display_t * disp = lv_obj_get_display(obj);
    lv_theme_shadcn_state_t * st = lv_theme_shadcn_state_find(disp);
    if(st == NULL || lv_display_get_theme(disp) != st->theme) return NULL;
    return st;
}

static void resolve_fonts(lv_theme_shadcn_state_t * st, const lv_theme_shadcn_fonts_t * fonts)
{
    lv_theme_shadcn_fonts_t f = { NULL, NULL, NULL, NULL };
    if(fonts) f = *fonts;
    if(f.normal == NULL) f.normal = LV_FONT_DEFAULT;
    if(f.small == NULL) f.small = f.normal;
    if(f.large == NULL) f.large = f.normal;
    if(f.mono == NULL) f.mono = f.normal;
    st->fonts = f;
}

static void rebuild(lv_theme_shadcn_state_t * st)
{
    lv_theme_shadcn_metrics_resolve(st);
    lv_theme_shadcn_styles_build(st);
    if(lv_display_get_theme(st->disp) == st->theme) lv_obj_report_style_change(NULL);
}

static void unlink_state(lv_theme_shadcn_state_t * st)
{
    lv_theme_shadcn_state_t ** link = &states;
    while(*link && *link != st) link = &(*link)->next;
    if(*link) *link = st->next;
}

static void free_state_cb(void * arg)
{
    lv_theme_shadcn_state_t * st = arg;
    lv_theme_shadcn_styles_reset(st);
    lv_theme_delete(st->theme);
    lv_free(st);
}

static void free_state(lv_theme_shadcn_state_t * st)
{
    unlink_state(st);
    st->disp = NULL;
    lv_async_call(free_state_cb, st);
}

static void display_event_cb(lv_event_t * e)
{
    lv_theme_shadcn_state_t * st = lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_RESOLUTION_CHANGED) {
        rebuild(st);
    }
    else if(code == LV_EVENT_DELETE) {
        free_state(st);
    }
}

lv_theme_t * lv_theme_shadcn_init(lv_display_t * disp, const lv_theme_shadcn_palette_t * palette,
                                  const lv_theme_shadcn_fonts_t * fonts, lv_theme_shadcn_flags_t flags)
{
    if(disp == NULL) disp = lv_display_get_default();
    if(disp == NULL || palette == NULL) return NULL;

    lv_theme_shadcn_state_t * st = lv_theme_shadcn_state_find(disp);
    if(st == NULL) {
        st = lv_malloc_zeroed(sizeof(*st));
        LV_ASSERT_MALLOC(st);
        if(st == NULL) return NULL;
        st->disp = disp;
        st->theme = lv_theme_create();
        lv_theme_set_apply_cb(st->theme, lv_theme_shadcn_apply_cb);
        lv_theme_set_user_data(st->theme, st);
        st->previous_theme = lv_display_get_theme(disp);
        st->next = states;
        states = st;
        lv_display_add_event_cb(disp, display_event_cb, LV_EVENT_RESOLUTION_CHANGED, st);
        lv_display_add_event_cb(disp, display_event_cb, LV_EVENT_DELETE, st);
    }

    st->palette = *palette;
    st->flags = flags;
    resolve_fonts(st, fonts);
    lv_display_set_theme(disp, st->theme);
    rebuild(st);
    return st->theme;
}

void lv_theme_shadcn_deinit(lv_display_t * disp)
{
    if(disp == NULL) disp = lv_display_get_default();
    lv_theme_shadcn_state_t * st = lv_theme_shadcn_state_find(disp);
    if(st == NULL) return;
    lv_display_remove_event_cb_with_user_data(disp, display_event_cb, st);
    if(lv_display_get_theme(disp) == st->theme) lv_display_set_theme(disp, st->previous_theme);
    free_state(st);
}

bool lv_theme_shadcn_is_active(lv_display_t * disp)
{
    if(disp == NULL) disp = lv_display_get_default();
    lv_theme_shadcn_state_t * st = lv_theme_shadcn_state_find(disp);
    return st != NULL && lv_display_get_theme(disp) == st->theme;
}

void lv_theme_shadcn_set_palette(lv_display_t * disp, const lv_theme_shadcn_palette_t * palette)
{
    if(disp == NULL) disp = lv_display_get_default();
    lv_theme_shadcn_state_t * st = lv_theme_shadcn_state_find(disp);
    if(st == NULL || palette == NULL) return;
    st->palette = *palette;
    rebuild(st);
}

const lv_theme_shadcn_palette_t * lv_theme_shadcn_get_palette(lv_display_t * disp)
{
    if(disp == NULL) disp = lv_display_get_default();
    lv_theme_shadcn_state_t * st = lv_theme_shadcn_state_find(disp);
    return st ? &st->palette : NULL;
}
