cmake_minimum_required(VERSION 3.25)
project(lvgl_theme_shadcn_rust_ci LANGUAGES C)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

get_filename_component(LV_THEME_SHADCN_ROOT ${CMAKE_CURRENT_LIST_DIR}/../.. ABSOLUTE)
include(${LV_THEME_SHADCN_ROOT}/tests/lvgl.cmake)

add_library(lvgl_theme_shadcn_layout_probe STATIC layout_probe.c)
target_include_directories(lvgl_theme_shadcn_layout_probe PRIVATE ${LV_THEME_SHADCN_ROOT}/include)
target_link_libraries(lvgl_theme_shadcn_layout_probe PRIVATE lvgl)
set_target_properties(lvgl_theme_shadcn_layout_probe PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
