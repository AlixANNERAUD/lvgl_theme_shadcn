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
    lv_draw_buf_t * decoded = NULL;
    if(ok) ok = lodepng_decode32((unsigned char **)&decoded, width, height, png, (size_t)size) == 0;
    free(png);
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
        &lv_font_montserrat_12, &lv_font_montserrat_14, &lv_font_montserrat_20, NULL
    };
    lv_theme_shadcn_init(display, lv_theme_shadcn_palette_zinc_dark(), &fonts, LV_THEME_SHADCN_FLAG_NONE);

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
    for(int p = 0; p < 2; p++) {
        lv_theme_shadcn_set_palette(display, palettes[p]);
        for(uint32_t s = 0; s < count; s++) {
            char name[96];
            char path[512];
            snprintf(name, sizeof(name), "%s-%s", scenes[s].name, palette_names[p]);

            unsigned char * actual = render(&scenes[s], &snapshot_a) ? to_rgba(&snapshot_a) : NULL;
            if(actual == NULL) {
                printf("FAIL %s (render error)\n", name);
                failed++;
                continue;
            }

            snprintf(path, sizeof(path), "%s/%s.png", out_dir, name);
            if(!write_png(path, actual)) {
                printf("FAIL %s (cannot write %s)\n", name, path);
                failed++;
            } else if(update) {
                snprintf(path, sizeof(path), "%s/%s.png", ref_dir, name);
                if(write_png(path, actual)) printf("UPDATED %s\n", name);
                else {
                    printf("FAIL %s (cannot write %s)\n", name, path);
                    failed++;
                }
            } else {
                snprintf(path, sizeof(path), "%s/%s.png", ref_dir, name);
                unsigned rw = 0;
                unsigned rh = 0;
                lv_draw_buf_t * ref = read_png(path, &rw, &rh);
                if(ref == NULL) {
                    printf("MISSING %s (0 px)\n", name);
                    missing++;
                } else if(rw != WIDTH || rh != HEIGHT) {
                    printf("FAIL %s (size %ux%u)\n", name, rw, rh);
                    failed++;
                } else {
                    unsigned char * diff = malloc((size_t)WIDTH * HEIGHT * 4);
                    unsigned n = diff ? count_diff(actual, (const unsigned char *)ref->data, diff) : (unsigned)WIDTH * HEIGHT;
                    if((uint64_t)n * 1000 > (uint64_t)WIDTH * HEIGHT * MAX_DIFF_PERMILLE) {
                        printf("FAIL %s (%u px)\n", name, n);
                        failed++;
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
    }

    if(failed) return 1;
    if(missing) {
        printf("run tests/screenshots/update.sh to create references\n");
        return 77;
    }
    return 0;
}
