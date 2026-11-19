#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "lv_theme_shadcn_scenes.h"
#include "src/libs/lodepng/lodepng.h"

#define WIDTH 480
#define HEIGHT 320
#define CHANNEL_TOLERANCE 8
#define MAX_DIFF_PERMILLE 1
#define SETTLE_STEPS 50
#define SETTLE_TICK_MS 20

static uint8_t draw_buffer[WIDTH * 40 * 4];
LV_DRAW_BUF_DEFINE_STATIC(snapshot_a, WIDTH, HEIGHT, LV_COLOR_FORMAT_XRGB8888);
LV_DRAW_BUF_DEFINE_STATIC(snapshot_b, WIDTH, HEIGHT, LV_COLOR_FORMAT_XRGB8888);

static void flush_cb(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map)
{
    (void)area;
    (void)px_map;
    lv_display_flush_ready(disp);
}

/* Scenes rendered only here, for cases the gallery scenes do not show. */
static lv_obj_t * test_label(lv_obj_t * parent, const char * str)
{
    lv_obj_t * label = lv_label_create(parent);
    lv_label_set_text(label, str);
    return label;
}

static lv_obj_t * test_button(lv_obj_t * parent, const char * str, lv_theme_shadcn_variant_t variant, lv_state_t state)
{
    lv_obj_t * btn = lv_button_create(parent);
    test_label(btn, str);
    lv_theme_shadcn_set_variant(btn, variant);
    lv_obj_add_state(btn, state);
    return btn;
}

static lv_obj_t * test_column(lv_obj_t * parent)
{
    lv_obj_t * column = lv_obj_create(parent);
    lv_obj_set_size(column, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(column, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_scrollable(column, false);
    return column;
}

static void build_surface_transparent(lv_obj_t * parent)
{
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(parent, 16, 0);
    lv_obj_set_style_pad_gap(parent, 16, 0);

    lv_obj_t * card = test_column(parent);
    test_label(card, "Transparent rows inside a card");
    lv_obj_t * toolbar = lv_obj_create(card);
    lv_theme_shadcn_set_surface(toolbar, LV_THEME_SHADCN_SURFACE_TRANSPARENT);
    lv_obj_set_size(toolbar, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(toolbar, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_column(toolbar, 8, 0);
    test_button(toolbar, "Save", LV_THEME_SHADCN_VARIANT_DEFAULT, LV_STATE_DEFAULT);
    test_button(toolbar, "Cancel", LV_THEME_SHADCN_VARIANT_OUTLINE, LV_STATE_DEFAULT);
    lv_obj_t * details = test_column(card);
    lv_theme_shadcn_set_surface(details, LV_THEME_SHADCN_SURFACE_TRANSPARENT);
    lv_obj_set_style_pad_row(details, 4, 0);
    test_label(details, "No background, border, radius or padding.");
    lv_theme_shadcn_set_text_role(test_label(details, "Text keeps the card colour."), LV_THEME_SHADCN_TEXT_MUTED);

    lv_obj_t * restored = test_column(parent);
    lv_theme_shadcn_set_surface(restored, LV_THEME_SHADCN_SURFACE_TRANSPARENT);
    lv_theme_shadcn_set_surface(restored, LV_THEME_SHADCN_SURFACE_CARD);
    test_label(restored, "Switched back to a card");
}

static void build_dropdown_open(lv_obj_t * parent)
{
    lv_obj_t * overflowing = lv_dropdown_create(parent);
    lv_dropdown_set_options_static(overflowing, "A very long option that overflows");
    lv_obj_set_width(overflowing, 180);
    lv_obj_set_pos(overflowing, 16, 16);

    lv_obj_t * dropdown = lv_dropdown_create(parent);
    lv_dropdown_set_options_static(dropdown, "Apple\nBanana\nBlueberry\nGrapes\nPineapple");
    lv_dropdown_set_selected(dropdown, 1);
    lv_obj_set_width(dropdown, 180);
    lv_obj_set_pos(dropdown, 240, 16);
    lv_dropdown_open(dropdown);
}

static lv_obj_t * test_row(lv_obj_t * parent, lv_theme_shadcn_surface_t surface)
{
    lv_obj_t * row = lv_obj_create(parent);
    lv_theme_shadcn_set_surface(row, surface);
    lv_obj_set_size(row, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_scrollable(row, false);
    return row;
}

static void build_pressed_on_surfaces(lv_obj_t * parent)
{
    static const lv_theme_shadcn_surface_t surfaces[] = {
        LV_THEME_SHADCN_SURFACE_BACKGROUND, LV_THEME_SHADCN_SURFACE_CARD, LV_THEME_SHADCN_SURFACE_MUTED
    };
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(parent, 12, 0);
    lv_obj_set_style_pad_gap(parent, 8, 0);
    for(size_t i = 0; i < sizeof(surfaces) / sizeof(surfaces[0]); i++) {
        lv_obj_t * row = test_row(parent, surfaces[i]);
        test_button(row, "Ghost", LV_THEME_SHADCN_VARIANT_GHOST, LV_STATE_PRESSED);
        test_button(row, "Outline", LV_THEME_SHADCN_VARIANT_OUTLINE, LV_STATE_PRESSED);
        test_button(row, "Hover", LV_THEME_SHADCN_VARIANT_GHOST, LV_STATE_HOVERED);
        test_button(row, "Idle", LV_THEME_SHADCN_VARIANT_OUTLINE, LV_STATE_DEFAULT);
    }
}

static void build_toggle_buttons(lv_obj_t * parent)
{
    static const lv_theme_shadcn_surface_t surfaces[] = { LV_THEME_SHADCN_SURFACE_CARD, LV_THEME_SHADCN_SURFACE_MUTED };
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(parent, 12, 0);
    lv_obj_set_style_pad_gap(parent, 8, 0);
    for(size_t i = 0; i < sizeof(surfaces) / sizeof(surfaces[0]); i++) {
        lv_obj_t * row = test_row(parent, surfaces[i]);
        static const lv_theme_shadcn_variant_t variants[] = {
            LV_THEME_SHADCN_VARIANT_GHOST, LV_THEME_SHADCN_VARIANT_GHOST,
            LV_THEME_SHADCN_VARIANT_OUTLINE, LV_THEME_SHADCN_VARIANT_OUTLINE, LV_THEME_SHADCN_VARIANT_DEFAULT
        };
        static const char * const labels[] = { "Bold", "Italic", "Grid", "List", "Filled" };
        for(size_t v = 0; v < sizeof(variants) / sizeof(variants[0]); v++) {
            lv_obj_t * btn = test_button(row, labels[v], variants[v], v % 2 == 0 ? LV_STATE_CHECKED : LV_STATE_DEFAULT);
            lv_obj_set_checkable(btn, true);
        }
    }
}

static void build_radio_group(lv_obj_t * parent)
{
    static const char * const options[] = { "Default", "Comfortable", "Compact" };
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_all(parent, 16, 0);
    lv_obj_set_style_pad_gap(parent, 16, 0);
    static const lv_theme_shadcn_surface_t surfaces[] = { LV_THEME_SHADCN_SURFACE_CARD, LV_THEME_SHADCN_SURFACE_MUTED };
    for(size_t s = 0; s < sizeof(surfaces) / sizeof(surfaces[0]); s++) {
        lv_obj_t * group = lv_obj_create(parent);
        lv_theme_shadcn_set_surface(group, surfaces[s]);
        lv_obj_set_size(group, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(group, LV_FLEX_FLOW_COLUMN);
        for(size_t i = 0; i < sizeof(options) / sizeof(options[0]); i++) {
            lv_obj_t * cb = lv_checkbox_create(group);
            lv_checkbox_set_text_static(cb, options[i]);
            lv_theme_shadcn_set_role(cb, LV_THEME_SHADCN_ROLE_RADIO);
            if(i == 1) lv_obj_add_state(cb, LV_STATE_CHECKED);
        }
        lv_obj_t * plain = lv_checkbox_create(group);
        lv_checkbox_set_text_static(plain, "Checkbox");
        lv_obj_add_state(plain, LV_STATE_CHECKED);
    }
}

static const lv_theme_shadcn_scene_t test_scenes[] = {
    { "surface-transparent", build_surface_transparent },
    { "dropdown-open", build_dropdown_open },
    { "pressed-on-surfaces", build_pressed_on_surfaces },
    { "toggle-buttons", build_toggle_buttons },
    { "radio-group", build_radio_group },
};

static unsigned char * to_rgba(const lv_draw_buf_t * snap)
{
    unsigned char * rgba = malloc((size_t)WIDTH * HEIGHT * 4);
    if(rgba == NULL) return NULL;
    for(int y = 0; y < HEIGHT; y++) {
        const uint8_t * src = (const uint8_t *)snap->data + (size_t)y * snap->header.stride;
        unsigned char * dst = rgba + (size_t)y * WIDTH * 4;
        for(int x = 0; x < WIDTH; x++) {
            dst[x * 4 + 0] = src[x * 4 + 2];
            dst[x * 4 + 1] = src[x * 4 + 1];
            dst[x * 4 + 2] = src[x * 4 + 0];
            dst[x * 4 + 3] = 255;
        }
    }
    return rgba;
}

static bool render(const lv_theme_shadcn_scene_t * scene, lv_draw_buf_t * snap)
{
    lv_obj_t * previous = lv_screen_active();
    lv_obj_t * screen = lv_obj_create(NULL);
    lv_obj_set_size(screen, WIDTH, HEIGHT);
    lv_screen_load(screen);
    if(previous != NULL) lv_obj_delete(previous);
    scene->build(screen);
    for(int i = 0; i < SETTLE_STEPS; i++) {
        lv_tick_inc(SETTLE_TICK_MS);
        lv_timer_handler();
    }
    bool ok = lv_snapshot_take_to_draw_buf(screen, LV_COLOR_FORMAT_XRGB8888, snap) == LV_RESULT_OK;
    lv_screen_load(lv_obj_create(NULL));
    lv_obj_delete(screen);
    return ok;
}

static bool snapshots_equal(const lv_draw_buf_t * a, const lv_draw_buf_t * b)
{
    for(int y = 0; y < HEIGHT; y++) {
        const uint8_t * pa = (const uint8_t *)a->data + (size_t)y * a->header.stride;
        const uint8_t * pb = (const uint8_t *)b->data + (size_t)y * b->header.stride;
        if(memcmp(pa, pb, (size_t)WIDTH * 4) != 0) return false;
    }
    return true;
}

static bool write_png(const char * path, const unsigned char * rgba)
{
    unsigned char * png = NULL;
    size_t size = 0;
    if(lodepng_encode32(&png, &size, rgba, WIDTH, HEIGHT) != 0) return false;
    FILE * file = fopen(path, "wb");
    bool ok = file != NULL && fwrite(png, 1, size, file) == size;
    if(file != NULL && fclose(file) != 0) ok = false;
    lv_free(png);
    return ok;
}

static lv_draw_buf_t * read_png(const char * path, unsigned * width, unsigned * height)
{
    FILE * file = fopen(path, "rb");
    if(file == NULL) return NULL;
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    unsigned char * png = size > 0 ? malloc((size_t)size) : NULL;
    bool ok = png != NULL && fread(png, 1, (size_t)size, file) == (size_t)size;
    fclose(file);
    unsigned char * raw = NULL;
    if(ok) ok = lodepng_decode32(&raw, width, height, png, (size_t)size) == 0;
    free(png);
    lv_draw_buf_t * decoded = (lv_draw_buf_t *)raw;
    return ok ? decoded : NULL;
}

static unsigned count_diff(const unsigned char * actual, const unsigned char * ref, unsigned char * diff)
{
    unsigned count = 0;
    for(size_t i = 0; i < (size_t)WIDTH * HEIGHT; i++) {
        bool differs = false;
        for(int c = 0; c < 3; c++) {
            int d = (int)actual[i * 4 + c] - (int)ref[i * 4 + c];
            if(d > CHANNEL_TOLERANCE || d < -CHANNEL_TOLERANCE) differs = true;
        }
        if(differs) {
            count++;
            diff[i * 4 + 0] = 255;
            diff[i * 4 + 1] = 0;
            diff[i * 4 + 2] = 0;
        } else {
            diff[i * 4 + 0] = actual[i * 4 + 0] / 3;
            diff[i * 4 + 1] = actual[i * 4 + 1] / 3;
            diff[i * 4 + 2] = actual[i * 4 + 2] / 3;
        }
        diff[i * 4 + 3] = 255;
    }
    return count;
}

static void check(const lv_theme_shadcn_scene_t * scene, const char * name, const char * ref_dir, const char * out_dir,
                  bool update, int * failed, int * missing)
{
    char path[512];
    unsigned char * actual = render(scene, &snapshot_a) ? to_rgba(&snapshot_a) : NULL;
    if(actual == NULL) {
        printf("FAIL %s (render error)\n", name);
        (*failed)++;
        return;
    }

    snprintf(path, sizeof(path), "%s/%s.png", out_dir, name);
    if(!write_png(path, actual)) {
        printf("FAIL %s (cannot write %s)\n", name, path);
        (*failed)++;
    } else if(update) {
        snprintf(path, sizeof(path), "%s/%s.png", ref_dir, name);
        if(write_png(path, actual)) printf("UPDATED %s\n", name);
        else {
            printf("FAIL %s (cannot write %s)\n", name, path);
            (*failed)++;
        }
    } else {
        snprintf(path, sizeof(path), "%s/%s.png", ref_dir, name);
        unsigned rw = 0;
        unsigned rh = 0;
        lv_draw_buf_t * ref = read_png(path, &rw, &rh);
        if(ref == NULL) {
            printf("MISSING %s (0 px)\n", name);
            (*missing)++;
        } else if(rw != WIDTH || rh != HEIGHT) {
            printf("FAIL %s (size %ux%u)\n", name, rw, rh);
            (*failed)++;
        } else {
            unsigned char * diff = malloc((size_t)WIDTH * HEIGHT * 4);
            unsigned n = diff ? count_diff(actual, (const unsigned char *)ref->data, diff) : (unsigned)WIDTH * HEIGHT;
            if((uint64_t)n * 1000 > (uint64_t)WIDTH * HEIGHT * MAX_DIFF_PERMILLE) {
                printf("FAIL %s (%u px)\n", name, n);
                (*failed)++;
                if(diff) {
                    snprintf(path, sizeof(path), "%s/%s-diff.png", out_dir, name);
                    write_png(path, diff);
                }
            } else {
                printf("PASS %s (%u px)\n", name, n);
            }
            free(diff);
        }
        if(ref != NULL) lv_draw_buf_destroy(ref);
    }
    free(actual);
}

int main(int argc, char ** argv)
{
    bool update = false;
    if(argc == 4 && strcmp(argv[3], "--update") == 0) update = true;
    else if(argc != 3) {
        fprintf(stderr, "usage: %s <ref_dir> <out_dir> [--update]\n", argv[0]);
        return 2;
    }
    const char * ref_dir = argv[1];
    const char * out_dir = argv[2];
    mkdir(out_dir, 0777);
    if(update) mkdir(ref_dir, 0777);

    lv_init();
    lv_display_t * display = lv_display_create(WIDTH, HEIGHT);
    lv_display_set_buffers(display, draw_buffer, NULL, sizeof(draw_buffer), LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(display, flush_cb);
    static const lv_theme_shadcn_fonts_t fonts = {
        .small = &lv_font_montserrat_12,
        .normal = &lv_font_montserrat_14,
        .large = &lv_font_montserrat_20,
        .h1 = &lv_font_montserrat_32,
        .h2 = &lv_font_montserrat_28,
        .h3 = &lv_font_montserrat_24,
    };
    lv_theme_shadcn_init(display, lv_theme_shadcn_palette_zinc_dark(), &fonts, LV_THEME_SHADCN_FLAG_REDUCED_MOTION);

    uint32_t count = 0;
    const lv_theme_shadcn_scene_t * scenes = lv_theme_shadcn_scenes(&count);
    if(count == 0) {
        fprintf(stderr, "no scenes\n");
        return 1;
    }

    LV_DRAW_BUF_INIT_STATIC(snapshot_a);
    LV_DRAW_BUF_INIT_STATIC(snapshot_b);
    if(!render(&scenes[0], &snapshot_a) || !render(&scenes[0], &snapshot_b) ||
       !snapshots_equal(&snapshot_a, &snapshot_b)) {
        fprintf(stderr, "FAIL determinism: %s rendered twice differs\n", scenes[0].name);
        return 1;
    }

    const char * palette_names[2] = { "dark", "light" };
    const lv_theme_shadcn_palette_t * palettes[2] = {
        lv_theme_shadcn_palette_zinc_dark(), lv_theme_shadcn_palette_zinc_light()
    };

    int failed = 0;
    int missing = 0;
    const lv_theme_shadcn_scene_t * showcase = NULL;
    for(int p = 0; p < 2; p++) {
        lv_theme_shadcn_set_palette(display, palettes[p]);
        for(uint32_t s = 0; s < count; s++) {
            char name[96];
            snprintf(name, sizeof(name), "%s-%s", scenes[s].name, palette_names[p]);
            check(&scenes[s], name, ref_dir, out_dir, update, &failed, &missing);
            if(strcmp(scenes[s].name, "showcase") == 0) showcase = &scenes[s];
        }
        for(size_t s = 0; s < sizeof(test_scenes) / sizeof(test_scenes[0]); s++) {
            char name[96];
            snprintf(name, sizeof(name), "%s-%s", test_scenes[s].name, palette_names[p]);
            check(&test_scenes[s], name, ref_dir, out_dir, update, &failed, &missing);
        }
    }

    if(showcase == NULL) {
        printf("FAIL showcase scene missing\n");
        failed++;
    } else {
        lv_theme_shadcn_palette_t palette;
        for(int a = LV_THEME_SHADCN_ACCENT_NONE + 1; a < LV_THEME_SHADCN_ACCENT_COUNT; a++) {
            char name[96];
            lv_theme_shadcn_palette_make(&palette, LV_THEME_SHADCN_BASE_ZINC, (lv_theme_shadcn_accent_t)a, true);
            lv_theme_shadcn_set_palette(display, &palette);
            snprintf(name, sizeof(name), "palette-accent-%s-dark", lv_theme_shadcn_accent_name((lv_theme_shadcn_accent_t)a));
            check(showcase, name, ref_dir, out_dir, update, &failed, &missing);
        }
        for(int b = 0; b < LV_THEME_SHADCN_BASE_COUNT; b++) {
            char name[96];
            lv_theme_shadcn_palette_make(&palette, (lv_theme_shadcn_base_t)b, LV_THEME_SHADCN_ACCENT_NONE, false);
            lv_theme_shadcn_set_palette(display, &palette);
            snprintf(name, sizeof(name), "palette-base-%s-light", lv_theme_shadcn_base_name((lv_theme_shadcn_base_t)b));
            check(showcase, name, ref_dir, out_dir, update, &failed, &missing);
        }
    }
    lv_deinit();

    if(missing) printf("missing references: run tests/screenshots/update.sh to create them\n");
    if(failed || missing) return 1;
    return 0;
}
