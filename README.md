# lvgl_theme_shadcn

[![CI](https://github.com/AlixANNERAUD/lvgl_theme_shadcn/actions/workflows/ci.yml/badge.svg)](https://github.com/AlixANNERAUD/lvgl_theme_shadcn/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![LVGL](https://img.shields.io/badge/LVGL-%E2%89%A5%209.6-brightgreen.svg)](https://lvgl.io)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/alixanneraud/library/lvgl_theme_shadcn.svg)](https://registry.platformio.org/libraries/alixanneraud/lvgl_theme_shadcn)

**Make your LVGL 9 interface look like [shadcn/ui](https://ui.shadcn.com) with one function call.**
`lvgl_theme_shadcn` is a C99 theme that restyles every core [LVGL](https://lvgl.io) widget with shadcn's colour
roles, radii and crisp border-based surfaces, built for touch screens and cheap enough for microcontrollers.
Switch between dark and light at runtime, plug in your brand colour, and keep writing plain LVGL code.

<p align="center">
  <img src="tests/screenshots/ref/showcase-dark.png" alt="Smart home dashboard, dark palette" width="480">
  <img src="tests/screenshots/ref/showcase-light.png" alt="Smart home dashboard, light palette" width="480">
</p>

## Why lvgl_theme_shadcn?

- **Every widget, every state.** Buttons, switches, checkboxes, sliders, arcs, bars, scales, text areas, dropdowns,
  rollers, spinboxes, tables, lists, tab views, calendars, charts, keyboards, message boxes and more are styled for
  default, pressed, checked, focused, keyboard-focused and disabled states. No more default-theme leftovers.
- **Dark and light, switchable at runtime.** Ships the shadcn zinc dark and light palettes; `lv_theme_shadcn_set_palette`
  restyles every existing widget in place, no screen rebuild needed.
- **Touch-first.** Controls are 40 dp tall with 48 dp touch targets, clear pressed feedback and a visible focus ring
  for keypad and encoder navigation. A compact flag tightens everything for small screens.
- **Cheap to render.** No shadows, blur or gradients: flat fills and 1 dp borders that stay fast on MCUs with small
  draw buffers and RGB565 displays.
- **One-call setup.** `lv_theme_shadcn_init` and you are done. Variants, surfaces and error states are a single call
  per widget when you need them.
- **Brand it in one line.** Derive a full palette from your primary colour with `lv_theme_shadcn_palette_from_primary`,
  or fill in your own palette struct.
- **C today, Rust next.** Plain C99 with no dependency besides LVGL; Rust bindings are on the way.
- **Ready for your toolchain.** CMake, ESP-IDF, Zephyr, PlatformIO and Arduino manifests are included.

## Gallery

Every image below is a reference from the screenshot test suite, rendered at 480x320 straight from
[`examples/scenes`](examples/scenes).

| Scene | Dark | Light |
| --- | --- | --- |
| Buttons: variants, icons and states | ![Buttons, dark](tests/screenshots/ref/buttons-dark.png) | ![Buttons, light](tests/screenshots/ref/buttons-light.png) |
| Toggles: switches, checkboxes and LEDs | ![Toggles, dark](tests/screenshots/ref/toggles-dark.png) | ![Toggles, light](tests/screenshots/ref/toggles-light.png) |
| Ranges: sliders, bars, scales and arcs | ![Ranges, dark](tests/screenshots/ref/ranges-dark.png) | ![Ranges, light](tests/screenshots/ref/ranges-light.png) |
| Inputs: text, selects and pickers | ![Inputs, dark](tests/screenshots/ref/inputs-dark.png) | ![Inputs, light](tests/screenshots/ref/inputs-light.png) |
| Collections: tabs, tables, charts, calendar and lists | ![Collections, dark](tests/screenshots/ref/collections-dark.png) | ![Collections, light](tests/screenshots/ref/collections-light.png) |
| Overlays: message box and keyboard | ![Overlays, dark](tests/screenshots/ref/overlays-dark.png) | ![Overlays, light](tests/screenshots/ref/overlays-light.png) |

## Quick start

```c
#include "lv_theme_shadcn.h"

static const lv_theme_shadcn_fonts_t fonts = {
    .small = &lv_font_montserrat_12,
    .normal = &lv_font_montserrat_14,
    .large = &lv_font_montserrat_20,
};

void ui_init(lv_display_t * display)
{
    lv_theme_shadcn_init(display, lv_theme_shadcn_palette_zinc_dark(), &fonts, LV_THEME_SHADCN_FLAG_NONE);

    lv_obj_t * save = lv_button_create(lv_screen_active());
    lv_label_set_text(lv_label_create(save), "Save");

    lv_obj_t * cancel = lv_button_create(lv_screen_active());
    lv_label_set_text(lv_label_create(cancel), "Cancel");
    lv_theme_shadcn_set_variant(cancel, LV_THEME_SHADCN_VARIANT_OUTLINE);
}

void ui_set_light(lv_display_t * display, bool light)
{
    lv_theme_shadcn_set_palette(display, light ? lv_theme_shadcn_palette_zinc_light()
                                               : lv_theme_shadcn_palette_zinc_dark());
}
```

Call `lv_theme_shadcn_init` before creating widgets; passing `NULL` as the display uses the default display. Any
font left `NULL` falls back to `normal`, and `normal` falls back to `LV_FONT_DEFAULT`, so enable the Montserrat sizes
you reference in your `lv_conf.h`. Palette and resolution changes restyle existing widgets, except LED colours, which
are object properties set at creation. `lv_theme_shadcn_deinit` restores the previous theme.

## Installing

LVGL 9.6 or newer is required. The header includes `lvgl.h` (or `lvgl/lvgl.h` when that is the one on the include
path); define `LV_THEME_SHADCN_LVGL_H_INCLUDE` to point it elsewhere, for example `-DLV_THEME_SHADCN_LVGL_H_INCLUDE='"my_lvgl.h"'`.

### CMake

```cmake
add_subdirectory(lvgl_theme_shadcn)
target_link_libraries(app PRIVATE lvgl_theme_shadcn)
```

The `lvgl` target must exist before `add_subdirectory`. As a subproject the library builds no tests or examples and
fetches nothing.

### ESP-IDF

Add the dependency to your component's `idf_component.yml`; the theme is then available to any component that
requires it:

```yaml
dependencies:
  lvgl/lvgl: ">=9.6.0"
  lvgl_theme_shadcn:
    git: https://github.com/AlixANNERAUD/lvgl_theme_shadcn.git
```

Enable the fonts you use with `CONFIG_LV_FONT_MONTSERRAT_12=y` and friends. A complete project lives in
[`examples/esp_idf`](examples/esp_idf).

### Zephyr

The repository is a Zephyr module. Add it to your `west.yml` (or to `ZEPHYR_EXTRA_MODULES`) and enable it:

```ini
CONFIG_LVGL=y
CONFIG_LV_THEME_SHADCN=y
```

### PlatformIO

```ini
lib_deps =
    alixanneraud/lvgl_theme_shadcn
```

LVGL is pulled in as a dependency; provide your `lv_conf.h` as usual. [`examples/platformio`](examples/platformio) is
a native project that renders a themed button headlessly.

### Arduino

Download the repository as a ZIP and install it with **Sketch > Include Library > Add .ZIP Library**, next to the
`lvgl` library and its `lv_conf.h`. Then `#include <lv_theme_shadcn.h>` in your sketch.

## Customising

### Brand colour

```c
static lv_theme_shadcn_palette_t brand;
lv_theme_shadcn_palette_from_primary(&brand, lv_theme_shadcn_palette_zinc_light(),
                                     lv_color_hex(0x2563eb), lv_color_hex(0xffffff));
lv_theme_shadcn_set_palette(NULL, &brand);
```

`palette_from_primary` copies the base palette, swaps the primary pair and derives the focus ring from it. For full
control fill an `lv_theme_shadcn_palette_t` yourself: it holds shadcn's colour roles (`background`, `card`,
`popover`, `primary`, `secondary`, `muted`, `accent`, `destructive`, each with its foreground, plus `border`, `input`,
`ring` and five `chart` colours). The theme copies the palette, so it does not need to outlive the call, and
`lv_theme_shadcn_get_palette` returns the active one.

### Variants

```c
lv_theme_shadcn_set_variant(button, LV_THEME_SHADCN_VARIANT_DESTRUCTIVE);
```

`DEFAULT`, `SECONDARY`, `OUTLINE`, `GHOST` and `DESTRUCTIVE`, each with its own pressed state.

### Surfaces

```c
lv_theme_shadcn_set_surface(panel, LV_THEME_SHADCN_SURFACE_CARD);
```

`BACKGROUND`, `CARD`, `POPOVER` and `MUTED` set a container's fill and text colour to the matching palette role.

### Errors

```c
lv_theme_shadcn_set_error(textarea, true);
```

Draws the border and text in the destructive colour, including while focused or checked; pass `false` to clear it.

### Compact mode

Pass `LV_THEME_SHADCN_FLAG_COMPACT` to `lv_theme_shadcn_init` to tighten spacing, shrink controls to 32 dp and
touch targets to 40 dp, for dense screens or pointer input.

## Gallery app

[`examples/gallery`](examples/gallery) is an SDL2 desktop app with one tab per scene and a light switch in the header
that flips the palette live. It is a standalone CMake project that fetches LVGL 9.6.0:

```sh
cmake -S examples/gallery -B build-gallery
cmake --build build-gallery --parallel
./build-gallery/gallery
```

## Testing

```sh
cmake -S . -B build
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

Building the repository at the top level fetches LVGL 9.6.0 and runs two kinds of tests:

- **Unit tests** in [`tests/unit`](tests/unit) check palettes, lifecycle and the styles applied to each widget family.
- **Screenshot tests** in [`tests/screenshots`](tests/screenshots) render every scene in both palettes at 480x320,
  check that rendering is deterministic, and compare each image against `tests/screenshots/ref`. Renders land in
  `build/screenshots`, with a `-diff.png` highlighting the pixels that changed when a scene fails.

After an intentional visual change, regenerate the references and review them in your diff:

```sh
tests/screenshots/update.sh
```

Set `LV_THEME_SHADCN_LVGL_SOURCE_DIR` to a local LVGL checkout to build offline. CI runs the suite with gcc and
clang, in both RGB565 and XRGB8888 (`-DLV_THEME_SHADCN_TEST_COLOR_FORMAT=...`), and also builds the gallery, the
ESP-IDF example and the PlatformIO example.

## Rust

Rust bindings: coming soon.

## License

MIT, see [LICENSE](LICENSE).
