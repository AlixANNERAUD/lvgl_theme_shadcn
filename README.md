# lvgl_theme_shadcn

A touch-first [LVGL](https://lvgl.io) theme inspired by [shadcn/ui](https://ui.shadcn.com), for C (Rust bindings planned).

- Shadcn colour roles, shapes and border-based surfaces; Material Design 3 touch targets, state feedback and focus model.
- Every core LVGL widget styled in every interaction state.
- Built-in zinc dark and light palettes, switchable at runtime.
- Cheap to render on microcontrollers: no shadows, blur or gradients.
- Requires LVGL 9.6 or newer.

## Usage (C)

```c
#include "lv_theme_shadcn.h"

lv_theme_shadcn_fonts_t fonts = { &lv_font_montserrat_12, &lv_font_montserrat_14, &lv_font_montserrat_20, NULL };
lv_theme_shadcn_init(display, lv_theme_shadcn_palette_zinc_dark(), &fonts, LV_THEME_SHADCN_FLAG_NONE);

lv_obj_t * cancel = lv_button_create(lv_screen_active());
lv_theme_shadcn_set_variant(cancel, LV_THEME_SHADCN_VARIANT_OUTLINE);

lv_theme_shadcn_set_palette(display, lv_theme_shadcn_palette_zinc_light());
```

Call `lv_theme_shadcn_init` before creating widgets. Palette and resolution changes restyle existing widgets, except LED colours, which are object properties set at creation.

## Building with CMake

```cmake
add_subdirectory(lvgl_theme_shadcn)
target_link_libraries(app PRIVATE lvgl_theme_shadcn)
```

The `lvgl` target must exist before `add_subdirectory`. Tests: `cmake -S . -B build && cmake --build build && ctest --test-dir build`.

## License

MIT
