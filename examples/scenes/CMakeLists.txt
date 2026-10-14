add_library(lv_theme_shadcn_scenes STATIC lv_theme_shadcn_scenes.c)
target_include_directories(lv_theme_shadcn_scenes PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(lv_theme_shadcn_scenes PUBLIC lvgl_theme_shadcn)
set_target_properties(lv_theme_shadcn_scenes PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
if(NOT MSVC)
  target_compile_options(lv_theme_shadcn_scenes PRIVATE -Wall -Wextra -Wno-deprecated-declarations
    $<$<BOOL:${LV_THEME_SHADCN_WERROR}>:-Werror>)
endif()
