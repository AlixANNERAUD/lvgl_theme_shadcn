cmake_minimum_required(VERSION 3.16)
project(lvgl_theme_shadcn VERSION 0.1.0 LANGUAGES C)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  set(LV_THEME_SHADCN_TOP_LEVEL ON)
else()
  set(LV_THEME_SHADCN_TOP_LEVEL OFF)
endif()

option(LV_THEME_SHADCN_BUILD_TESTS "Build unit tests" ${LV_THEME_SHADCN_TOP_LEVEL})
option(LV_THEME_SHADCN_WERROR "Treat warnings as errors" OFF)

if(LV_THEME_SHADCN_BUILD_TESTS)
  include(${CMAKE_CURRENT_LIST_DIR}/tests/lvgl.cmake)
endif()

add_library(lvgl_theme_shadcn
  src/lv_theme_shadcn.c
  src/lv_theme_shadcn_apply.c
  src/lv_theme_shadcn_palette.c
  src/lv_theme_shadcn_styles.c
  src/widgets/lv_theme_shadcn_actions.c
  src/widgets/lv_theme_shadcn_inputs.c
  src/widgets/lv_theme_shadcn_ranges.c
  src/widgets/lv_theme_shadcn_surfaces.c
  src/widgets/lv_theme_shadcn_toggles.c)
add_library(lvgl_theme_shadcn::lvgl_theme_shadcn ALIAS lvgl_theme_shadcn)
target_include_directories(lvgl_theme_shadcn PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(lvgl_theme_shadcn PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)
target_link_libraries(lvgl_theme_shadcn PUBLIC lvgl)
set_target_properties(lvgl_theme_shadcn PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
if(NOT MSVC)
  target_compile_options(lvgl_theme_shadcn PRIVATE -Wall -Wextra $<$<BOOL:${LV_THEME_SHADCN_WERROR}>:-Werror>)
endif()

if(LV_THEME_SHADCN_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests/unit)
endif()
