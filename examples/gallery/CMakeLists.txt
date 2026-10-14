cmake_minimum_required(VERSION 3.25)
project(lvgl_theme_shadcn_gallery LANGUAGES C)
include(FetchContent)

set(LV_BUILD_CONF_PATH ${CMAKE_CURRENT_LIST_DIR}/lv_conf.h CACHE PATH "" FORCE)
set(CONFIG_LV_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CONFIG_LV_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
set(CONFIG_LV_USE_THORVG OFF CACHE BOOL "" FORCE)
set(CONFIG_LV_USE_THORVG_INTERNAL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(lvgl
  GIT_REPOSITORY https://github.com/lvgl/lvgl.git
  GIT_TAG v9.6.0
  GIT_SHALLOW TRUE
  SYSTEM)
FetchContent_MakeAvailable(lvgl)

find_package(SDL2 REQUIRED)
target_link_libraries(lvgl PUBLIC SDL2::SDL2)

set(LV_THEME_SHADCN_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(LV_THEME_SHADCN_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../.. theme)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../scenes scenes)

add_executable(gallery main.c)
target_link_libraries(gallery PRIVATE lv_theme_shadcn_scenes lvgl_theme_shadcn lvgl SDL2::SDL2)
set_target_properties(gallery PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
if(NOT MSVC)
  target_compile_options(gallery PRIVATE -Wall -Wextra -Werror -Wno-deprecated-declarations)
endif()
