cmake_minimum_required(VERSION 3.16)
include(${CMAKE_CURRENT_LIST_DIR}/sources.cmake)

if(ESP_PLATFORM)
  idf_component_register(SRCS ${LV_THEME_SHADCN_SOURCES}
                         INCLUDE_DIRS include
                         PRIV_INCLUDE_DIRS src
                         REQUIRES lvgl)
  return()
endif()

project(lvgl_theme_shadcn VERSION 0.1.2 LANGUAGES C)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  set(LV_THEME_SHADCN_TOP_LEVEL ON)
else()
  set(LV_THEME_SHADCN_TOP_LEVEL OFF)
endif()

option(LV_THEME_SHADCN_BUILD_TESTS "Build unit tests" ${LV_THEME_SHADCN_TOP_LEVEL})
option(LV_THEME_SHADCN_BUILD_EXAMPLES "Build demo scenes used by tests and the gallery" ${LV_THEME_SHADCN_TOP_LEVEL})
option(LV_THEME_SHADCN_WERROR "Treat warnings as errors" OFF)
option(LV_THEME_SHADCN_SANITIZE "Build with address and undefined sanitizers" OFF)

if(LV_THEME_SHADCN_SANITIZE)
  add_compile_options(-fsanitize=address,undefined -fno-sanitize-recover=undefined -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
  add_compile_options($<$<COMPILE_LANG_AND_ID:C,Clang,AppleClang>:-fno-sanitize=function>)
  add_compile_options($<$<COMPILE_LANG_AND_ID:C,Clang,AppleClang>:-fsanitize-ignorelist=${CMAKE_CURRENT_LIST_DIR}/tests/sanitize-ignorelist.txt>)
  add_compile_definitions(LV_THEME_SHADCN_SANITIZE_BUILD=1)
endif()

if(LV_THEME_SHADCN_BUILD_TESTS)
  include(${CMAKE_CURRENT_LIST_DIR}/tests/lvgl.cmake)
endif()

add_library(lvgl_theme_shadcn ${LV_THEME_SHADCN_SOURCES})
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
  if(LV_THEME_SHADCN_BUILD_EXAMPLES)
    add_subdirectory(examples/scenes)
  endif()
  add_subdirectory(tests/unit)
  add_subdirectory(tests/screenshots)
endif()
