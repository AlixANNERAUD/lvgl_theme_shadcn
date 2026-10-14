#!/bin/sh
set -eu

: "${LVGL_INCLUDE_DIR:?set LVGL_INCLUDE_DIR to the LVGL root directory containing lvgl.h}"
: "${LV_CONF_PATH:?set LV_CONF_PATH to the directory containing lv_conf.h}"

crate_dir=$(cd "$(dirname "$0")" && pwd)
root_dir=$(cd "$crate_dir/../.." && pwd)
output="${1:-$crate_dir/src/bindings.rs}"

bindgen "$root_dir/include/lv_theme_shadcn.h" \
    --output "$output" \
    --use-core \
    --disable-header-comment \
    --no-doc-comments \
    --no-layout-tests \
    --allowlist-function 'lv_theme_shadcn_.*' \
    --allowlist-type 'lv_theme_shadcn_.*' \
    --allowlist-var 'LV_THEME_SHADCN_.*' \
    --newtype-enum 'lv_theme_shadcn_.*_t' \
    --no-recursive-allowlist \
    --with-derive-custom-struct 'lv_theme_shadcn_palette_t=Clone,Copy,Debug,Default,PartialEq,Eq' \
    --raw-line '#[repr(C)]' \
    --raw-line '#[derive(Clone, Copy, Debug, Default, PartialEq, Eq)]' \
    --raw-line 'pub struct lv_color_t {' \
    --raw-line '    pub blue: u8,' \
    --raw-line '    pub green: u8,' \
    --raw-line '    pub red: u8,' \
    --raw-line '}' \
    --raw-line '#[repr(C)]' \
    --raw-line 'pub struct lv_obj_t {' \
    --raw-line '    _unused: [u8; 0],' \
    --raw-line '}' \
    --raw-line '#[repr(C)]' \
    --raw-line 'pub struct lv_display_t {' \
    --raw-line '    _unused: [u8; 0],' \
    --raw-line '}' \
    --raw-line '#[repr(C)]' \
    --raw-line 'pub struct lv_font_t {' \
    --raw-line '    _unused: [u8; 0],' \
    --raw-line '}' \
    --raw-line '#[repr(C)]' \
    --raw-line 'pub struct lv_theme_t {' \
    --raw-line '    _unused: [u8; 0],' \
    --raw-line '}' \
    -- \
    -I"$LVGL_INCLUDE_DIR" \
    -I"$LVGL_INCLUDE_DIR/include" \
    -I"$LV_CONF_PATH" \
    -DLV_CONF_INCLUDE_SIMPLE
