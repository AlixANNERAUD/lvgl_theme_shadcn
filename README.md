# lvgl_theme_shadcn

[![CI](https://github.com/AlixANNERAUD/lvgl_theme_shadcn/actions/workflows/ci.yml/badge.svg)](https://github.com/AlixANNERAUD/lvgl_theme_shadcn/actions/workflows/ci.yml)
[![Rust](https://github.com/AlixANNERAUD/lvgl_theme_shadcn/actions/workflows/rust.yml/badge.svg)](https://github.com/AlixANNERAUD/lvgl_theme_shadcn/actions/workflows/rust.yml)
[![crates.io](https://img.shields.io/crates/v/lvgl_theme_shadcn.svg)](https://crates.io/crates/lvgl_theme_shadcn)
[![docs.rs](https://img.shields.io/docsrs/lvgl_theme_shadcn)](https://docs.rs/lvgl_theme_shadcn)
[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/AlixANNERAUD/lvgl_theme_shadcn/blob/main/LICENSE)
[![LVGL](https://img.shields.io/badge/LVGL-%E2%89%A5%209.6-brightgreen.svg)](https://lvgl.io)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/alix-anneraud/library/lvgl_theme_shadcn.svg)](https://registry.platformio.org/libraries/alix-anneraud/lvgl_theme_shadcn)

**Make your LVGL 9 interface look like [shadcn/ui](https://ui.shadcn.com) with one function call.**
`lvgl_theme_shadcn` is a C99 theme that restyles every core [LVGL](https://lvgl.io) widget with shadcn's colour
roles, radii and crisp border-based surfaces, built for touch screens and cheap enough for microcontrollers.
Switch between dark and light at runtime, plug in your brand colour, and keep writing plain LVGL code.

<p align="center">
  <img src="https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/showcase-dark.png" alt="Smart home dashboard, dark palette" width="480">
  <img src="https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/showcase-light.png" alt="Smart home dashboard, light palette" width="480">
</p>

## Why lvgl_theme_shadcn?

- **Every widget, every state.** Buttons, switches, checkboxes, sliders, arcs, bars, scales, lines, text areas, dropdowns,
  rollers, spinboxes, tables, lists, tab views, calendars, charts, keyboards, message boxes and more are styled for
  default, pressed, checked, focused, keyboard-focused and disabled states. No more default-theme leftovers. Media widgets (image, canvas, gif, lottie, ffmpeg, gstreamer,
  3dtexture) and qrcode/barcode are intentionally left unstyled.
- **Every shadcn palette, switchable at runtime.** All 9 shadcn/ui base colours times 18 accents (including none), in
  dark and light; `lv_theme_shadcn_set_palette` restyles every existing widget in place, no screen rebuild needed.
- **Import your own shadcn theme.** Paste a theme from the shadcn/ui theme editor or tweakcn, run
  `scripts/import-theme.py`, and get a ready-to-compile C or Rust palette in light and dark.
- **Radius, sizes and typography.** Five radius presets, six button sizes including square icon buttons, and shadcn's
  `h1` to `h4`, `lead`, `muted` and `code` text roles.
- **Badges, alerts, cards and skeletons.** shadcn's component roles are one `lv_theme_shadcn_set_role` call on a plain
  LVGL object; skeletons pulse unless you ask for reduced motion.
- **Touch-first.** Controls are 40 dp tall with 48 dp touch targets, clear pressed feedback and a visible focus ring
  for keypad and encoder navigation. Pointer devices get hover feedback, and a compact flag tightens everything for
  small screens.
- **Cheap to render.** No shadows, blur or gradients: flat fills and 1 dp borders that stay fast on MCUs with small
  draw buffers and RGB565 displays.
- **One-call setup.** `lv_theme_shadcn_init` and you are done. Variants, sizes, surfaces, text roles, component roles
  and error states are a single call per widget when you need them.
- **Brand it in one line.** Derive a full palette from your primary colour with `lv_theme_shadcn_palette_from_primary`,
  or fill in your own palette struct.
- **C and Rust.** Plain C99 with no dependency besides LVGL, plus `no_std` Rust bindings on
  [crates.io](https://crates.io/crates/lvgl_theme_shadcn) that work with any LVGL binding.
- **Ready for your toolchain.** CMake, ESP-IDF, Zephyr, PlatformIO and Arduino manifests are included.

## Gallery

Every image below is a reference from the screenshot test suite, rendered at 480x320 straight from
[`examples/scenes`](https://github.com/AlixANNERAUD/lvgl_theme_shadcn/tree/main/examples/scenes).

| Scene | Dark | Light |
| --- | --- | --- |
| Buttons: variants, icons and states | ![Buttons, dark](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/buttons-dark.png) | ![Buttons, light](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/buttons-light.png) |
| Toggles: switches, checkboxes and LEDs | ![Toggles, dark](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/toggles-dark.png) | ![Toggles, light](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/toggles-light.png) |
| Ranges: sliders, bars, scales and arcs | ![Ranges, dark](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/ranges-dark.png) | ![Ranges, light](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/ranges-light.png) |
| Inputs: text, selects and pickers | ![Inputs, dark](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/inputs-dark.png) | ![Inputs, light](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/inputs-light.png) |
| Collections: tabs, tables, charts, calendar and lists | ![Collections, dark](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/collections-dark.png) | ![Collections, light](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/collections-light.png) |
| Overlays: message box and keyboard | ![Overlays, dark](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/overlays-dark.png) | ![Overlays, light](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/overlays-light.png) |
| Display: typography, badges, alerts, cards and skeletons | ![Display, dark](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/display-dark.png) | ![Display, light](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/display-light.png) |

## Colours

Every shadcn/ui base colour can be combined with every accent, in light or dark mode, with one call. The tables are
generated from the shadcn/ui registry by [`scripts/generate-palettes.py`](https://github.com/AlixANNERAUD/lvgl_theme_shadcn/blob/main/scripts/generate-palettes.py) and live in flash as `const` data.

```c
lv_theme_shadcn_palette_t palette;
lv_theme_shadcn_palette_make(&palette, LV_THEME_SHADCN_BASE_SLATE, LV_THEME_SHADCN_ACCENT_BLUE, true);
lv_theme_shadcn_set_palette(display, &palette);
```

The base (`lv_theme_shadcn_base_t`) sets backgrounds, surfaces, borders and text. The accent
(`lv_theme_shadcn_accent_t`) replaces `primary`, `secondary` and the chart colours; `LV_THEME_SHADCN_ACCENT_NONE`
keeps the base's monochrome primary and charts. `lv_theme_shadcn_base_name` and `lv_theme_shadcn_accent_name` return
the shadcn/ui names, handy for a settings menu. Shadcn's translucent dark `border` and `input` are replaced with opaque
tones of the same base, since LVGL composites them against whatever surface lies beneath.

### Bases, light

| | | |
| --- | --- | --- |
| `neutral` | `stone` | `zinc` |
| ![Neutral, light](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-base-neutral-light.png) | ![Stone, light](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-base-stone-light.png) | ![Zinc, light](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-base-zinc-light.png) |
| `gray` | `slate` | `mauve` |
| ![Gray, light](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-base-gray-light.png) | ![Slate, light](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-base-slate-light.png) | ![Mauve, light](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-base-mauve-light.png) |
| `olive` | `mist` | `taupe` |
| ![Olive, light](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-base-olive-light.png) | ![Mist, light](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-base-mist-light.png) | ![Taupe, light](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-base-taupe-light.png) |

### Accents on zinc, dark

| | | |
| --- | --- | --- |
| `amber` | `blue` | `cyan` |
| ![Amber, dark](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-accent-amber-dark.png) | ![Blue, dark](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-accent-blue-dark.png) | ![Cyan, dark](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-accent-cyan-dark.png) |
| `emerald` | `fuchsia` | `green` |
| ![Emerald, dark](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-accent-emerald-dark.png) | ![Fuchsia, dark](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-accent-fuchsia-dark.png) | ![Green, dark](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-accent-green-dark.png) |
| `indigo` | `lime` | `orange` |
| ![Indigo, dark](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-accent-indigo-dark.png) | ![Lime, dark](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-accent-lime-dark.png) | ![Orange, dark](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-accent-orange-dark.png) |
| `pink` | `purple` | `red` |
| ![Pink, dark](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-accent-pink-dark.png) | ![Purple, dark](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-accent-purple-dark.png) | ![Red, dark](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-accent-red-dark.png) |
| `rose` | `sky` | `teal` |
| ![Rose, dark](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-accent-rose-dark.png) | ![Sky, dark](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-accent-sky-dark.png) | ![Teal, dark](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-accent-teal-dark.png) |
| `violet` | `yellow` |
| ![Violet, dark](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-accent-violet-dark.png) | ![Yellow, dark](https://raw.githubusercontent.com/AlixANNERAUD/lvgl_theme_shadcn/main/tests/screenshots/ref/palette-accent-yellow-dark.png) |

## Quick start

```c
#include "lv_theme_shadcn.h"

static const lv_theme_shadcn_fonts_t fonts = {
    .small = &lv_font_montserrat_12,
    .normal = &lv_font_montserrat_14,
    .large = &lv_font_montserrat_20,
    .h1 = &lv_font_montserrat_32,
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
you reference in your `lv_conf.h`. The optional `mono` font and the heading fonts `h1`, `h2` and `h3` are covered
under [Typography](#typography); initialise the struct with designated initialisers, since new fields are appended to it. Palette and resolution changes restyle existing widgets, except LED colours, which
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
[`examples/esp_idf`](https://github.com/AlixANNERAUD/lvgl_theme_shadcn/tree/main/examples/esp_idf).

### Zephyr

The repository is a Zephyr module. Add it to your `west.yml` (or to `ZEPHYR_EXTRA_MODULES`) and enable it:

```ini
CONFIG_LVGL=y
CONFIG_LV_THEME_SHADCN=y
```

The Zephyr LVGL module must be version 9.6 or newer; the theme header stops the build with an `#error` otherwise.

### PlatformIO

```ini
lib_deps =
    alix-anneraud/lvgl_theme_shadcn
```

LVGL is pulled in as a dependency; provide your `lv_conf.h` as usual. [`examples/platformio`](https://github.com/AlixANNERAUD/lvgl_theme_shadcn/tree/main/examples/platformio) is
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

### Radius

```c
lv_theme_shadcn_set_radius(NULL, LV_THEME_SHADCN_RADIUS_LG);
```

`NONE`, `SM`, `MD`, `DEFAULT` (shadcn/ui's `0.625rem`), `LG` and `XL` rescale every corner in the theme and restyle
existing widgets in place. `lv_theme_shadcn_get_radius` returns the active preset, and the gallery has a radius picker
to try them live.

### Sizes

```c
lv_theme_shadcn_set_size(button, LV_THEME_SHADCN_SIZE_SM);
```

`DEFAULT`, `SM` and `LG` change a button's height, padding and font; `ICON`, `ICON_SM` and `ICON_LG` make it square
at the matching height, for icon-only buttons.

### Typography

```c
static const lv_theme_shadcn_fonts_t fonts = {
    .small = &lv_font_montserrat_12,
    .normal = &lv_font_montserrat_14,
    .large = &lv_font_montserrat_20,
    .mono = &lv_font_unscii_8,
    .h1 = &lv_font_montserrat_32,
    .h2 = &lv_font_montserrat_28,
    .h3 = &lv_font_montserrat_24,
};

lv_theme_shadcn_set_text_role(title, LV_THEME_SHADCN_TEXT_H1);
lv_theme_shadcn_set_text_role(hint, LV_THEME_SHADCN_TEXT_MUTED);
```

Text roles style a label with shadcn's typography: `H1`, `H2`, `H3`, `H4`, `P`, `LEAD`, `LARGE`, `SMALL`, `MUTED` and
`CODE`, plus `DEFAULT` to clear the role. `H1`, `H2` and `H3` use the heading fonts `h1`, `h2` and `h3`, `CODE` uses
`mono`. A missing `mono` falls back to `normal`, and missing headings fall back in turn (`h3` to `large`, `h2` to `h3`,
`h1` to `h2`), so the roles work with just the three base fonts.

Adding `h1`, `h2` and `h3` to `lv_theme_shadcn_fonts_t` breaks positional initialisers such as
`{ &small, &normal, &large, &mono }` when compiling with `-Wmissing-field-initializers`; switch to designated
initialisers as above.

### Components

```c
lv_obj_t * badge = lv_obj_create(row);
lv_theme_shadcn_set_role(badge, LV_THEME_SHADCN_ROLE_BADGE_SECONDARY);
lv_label_set_text(lv_label_create(badge), "New");

lv_obj_t * alert = lv_obj_create(screen);
lv_theme_shadcn_set_role(alert, LV_THEME_SHADCN_ROLE_ALERT_DESTRUCTIVE);
lv_obj_t * alert_title = lv_label_create(alert);
lv_label_set_text(alert_title, "Payment failed");
lv_theme_shadcn_set_text_role(alert_title, LV_THEME_SHADCN_TEXT_LARGE);
lv_label_set_text(lv_label_create(alert), "Your card was declined.");

lv_obj_t * skeleton = lv_obj_create(screen);
lv_theme_shadcn_set_role(skeleton, LV_THEME_SHADCN_ROLE_SKELETON);
lv_obj_set_size(skeleton, 160, 16);
```

Roles turn plain LVGL objects into shadcn components: `BADGE`, `BADGE_SECONDARY`, `BADGE_OUTLINE`,
`BADGE_DESTRUCTIVE`, `ALERT`, `ALERT_DESTRUCTIVE`, `SEPARATOR`, `SEPARATOR_VERTICAL`, `SKELETON` and the card parts
`CARD_HEADER`, `CARD_TITLE`, `CARD_DESCRIPTION`, `CARD_CONTENT` and `CARD_FOOTER`; `NONE` clears the role. Combine card
parts with `lv_theme_shadcn_set_surface(card, LV_THEME_SHADCN_SURFACE_CARD)` on the container. The `display` scene in
the gallery above shows them all. Skeletons pulse to signal loading; pass `LV_THEME_SHADCN_FLAG_REDUCED_MOTION` to
`lv_theme_shadcn_init` to keep them still.

### Compact mode

Pass `LV_THEME_SHADCN_FLAG_COMPACT` to `lv_theme_shadcn_init` to tighten spacing, shrink controls to 32 dp and
touch targets to 40 dp, for dense screens or pointer input.

## Import your shadcn theme

Design a theme in the shadcn/ui theme editor or [tweakcn](https://tweakcn.com), copy its CSS and turn it into a palette:

```sh
python3 scripts/import-theme.py theme.css --name brand -o brand_palette.h
python3 scripts/import-theme.py theme.css --lang rust --name brand -o brand_palette.rs
pbpaste | python3 scripts/import-theme.py --name brand
```

The C output defines `static const lv_theme_shadcn_palette_t brand_light` and `brand_dark`; pass either to
`lv_theme_shadcn_init` or `lv_theme_shadcn_set_palette`. The Rust output defines `brand_light()` and `brand_dark()`,
which return a `Palette`. The script needs only Python 3, reads a file or standard input, and writes to standard output
unless you pass `-o`.

It reads the `:root` and `.dark` variable blocks (also inside `@layer`, `@media` and `@supports`), follows `var()`
references, and accepts `oklch()`, `hsl()`, `rgb()` and hex colours, so both the current Tailwind v4 export and the
older space-separated HSL format work. Missing roles fall back to sensible values with a warning on standard error.

Shadcn's dark `border` and `input` are usually translucent. LVGL has no cheap way to composite them at draw time, so
the importer composites them over the background once; the result can look fainter than the built-in palettes, which
use hand-picked opaque tones.

## Gallery app

[`examples/gallery`](https://github.com/AlixANNERAUD/lvgl_theme_shadcn/tree/main/examples/gallery) is an SDL2 desktop app with one tab per scene and, in the header, base and accent
dropdowns plus a light switch that change the palette live, and a radius picker. It is a standalone CMake project that fetches LVGL 9.6.0:

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

- **Unit tests** in [`tests/unit`](https://github.com/AlixANNERAUD/lvgl_theme_shadcn/tree/main/tests/unit) check palettes, lifecycle and the styles applied to each widget family.
- **Screenshot tests** in [`tests/screenshots`](https://github.com/AlixANNERAUD/lvgl_theme_shadcn/tree/main/tests/screenshots) render every scene in both zinc palettes at 480x320,
  plus the showcase scene for every accent and every base, check that rendering is deterministic, and compare each image against `tests/screenshots/ref`. Renders land in
  `build/screenshots`, with a `-diff.png` highlighting the pixels that changed when a scene fails.

After an intentional visual change, regenerate the references and review them in your diff:

```sh
tests/screenshots/update.sh
```

Point the test build at a local LVGL checkout to build offline, either with the `LV_THEME_SHADCN_LVGL_SOURCE_DIR`
environment variable or with `-DFETCHCONTENT_SOURCE_DIR_LVGL=<path>`. A missing screenshot reference fails the test. CI runs the suite with gcc and
clang, in both RGB565 and XRGB8888 (`-DLV_THEME_SHADCN_TEST_COLOR_FORMAT=...`), and also builds the gallery, the
ESP-IDF example and the PlatformIO example.

## Rust

The [`lvgl_theme_shadcn`](https://crates.io/crates/lvgl_theme_shadcn) crate wraps the theme in a safe, `no_std`,
allocation-free API, on top of the raw [`lvgl_theme_shadcn_sys`](https://crates.io/crates/lvgl_theme_shadcn_sys)
bindings. It does not depend on any LVGL Rust crate, so it plugs into whichever binding your app already uses.

```sh
cargo add lvgl_theme_shadcn
```

The C theme is compiled by the sys crate, which needs the LVGL headers and your `lv_conf.h`. Point it at them in
`.cargo/config.toml`:

```toml
[env]
LVGL_INCLUDE_DIR = { value = "third_party/lvgl", relative = true }
LV_CONF_PATH = { value = "config", relative = true }
```

`LVGL_INCLUDE_DIR` is the LVGL root (the directory containing `lvgl.h`) and `LV_CONF_PATH` the directory containing
`lv_conf.h`; use the same configuration as the LVGL you link against. Linking LVGL itself stays your app's job,
through its LVGL binding or build system.

Handles are built from raw pointers of any type, so the `lv_display_t` and `lv_obj_t` pointers handed out by your
binding (for example `lvgl_rust_sys`) go straight in, and `as_ptr().cast()` takes them back out:

```rust
use lvgl_theme_shadcn::{Accent, Base, Color, Display, Flags, Fonts, Object, Palette, Theme, Variant};

let brand = Palette::zinc_dark().with_primary(Color::hex(0x2563eb), Color::hex(0xffffff));
let theme = unsafe {
    let display = Display::from_ptr(display_ptr).expect("display");
    Theme::init(display, &brand, &Fonts::default(), Flags::NONE).expect("theme")
};

let button = unsafe { Object::from_ptr(button_ptr) }.expect("button");
unsafe { lvgl_theme_shadcn::set_variant(&button, Variant::Destructive) };

theme.set_palette(&Palette::new(Base::Slate, Accent::Blue, false));
assert!(!theme.palette().expect("active theme").is_dark());
```

Dropping the `Theme` restores the display's previous theme. Radius, sizes, text roles and
component roles are available too:

```rust
use lvgl_theme_shadcn::{Radius, Role, Size, TextRole};

theme.set_radius(Radius::Lg);
unsafe {
    lvgl_theme_shadcn::set_size(&button, Size::IconSm);
    lvgl_theme_shadcn::set_text_role(&label, TextRole::H1);
    lvgl_theme_shadcn::set_role(&badge, Role::BadgeSecondary);
}
```

`Fonts` carries `h1`, `h2` and `h3` next to `small`, `normal`, `large` and `mono`, and `Flags::REDUCED_MOTION` keeps
skeletons still. `scripts/import-theme.py --lang rust` generates `Palette` constructors from a shadcn theme CSS. See the
[API documentation](https://docs.rs/lvgl_theme_shadcn) for fonts, surfaces, error states and palette roles.

## License

MIT, see [LICENSE](https://github.com/AlixANNERAUD/lvgl_theme_shadcn/blob/main/LICENSE).
