if(NOT TARGET lv_theme_shadcn_scenes)
  return()
endif()

file(READ ${LV_BUILD_CONF_PATH} lv_theme_shadcn_conf)
if(NOT lv_theme_shadcn_conf MATCHES "#define[ \t]+LV_USE_SNAPSHOT[ \t]+1" OR
   NOT lv_theme_shadcn_conf MATCHES "#define[ \t]+LV_USE_LODEPNG[ \t]+1")
  return()
endif()

add_executable(screenshots screenshots.c)
target_link_libraries(screenshots PRIVATE lv_theme_shadcn_scenes)
if(DEFINED lvgl_SOURCE_DIR)
  target_include_directories(screenshots PRIVATE ${lvgl_SOURCE_DIR})
endif()
set_target_properties(screenshots PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
if(NOT MSVC)
  target_compile_options(screenshots PRIVATE -Wall -Wextra $<$<BOOL:${LV_THEME_SHADCN_WERROR}>:-Werror>)
endif()

add_test(NAME screenshots
  COMMAND screenshots ${CMAKE_CURRENT_SOURCE_DIR}/ref ${CMAKE_BINARY_DIR}/screenshots)
