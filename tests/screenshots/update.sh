#!/bin/sh
set -eu
repo=$(cd "$(dirname "$0")/../.." && pwd)
build="$repo/build-screenshots"
extra=""
if [ -n "${LV_THEME_SHADCN_LVGL_SOURCE_DIR:-}" ]; then
  extra="-DFETCHCONTENT_SOURCE_DIR_LVGL=$LV_THEME_SHADCN_LVGL_SOURCE_DIR"
fi
cmake -S "$repo" -B "$build" -DLV_THEME_SHADCN_TEST_COLOR_FORMAT=LV_COLOR_FORMAT_XRGB8888 ${extra:+"$extra"}
cmake --build "$build" --target screenshots --parallel
"$build/tests/screenshots/screenshots" "$repo/tests/screenshots/ref" "$build/screenshots" --update
