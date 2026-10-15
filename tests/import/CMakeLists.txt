find_package(Python3 COMPONENTS Interpreter)
if(NOT Python3_FOUND)
  return()
endif()

set(import_script ${CMAKE_CURRENT_LIST_DIR}/../../scripts/import-theme.py)
set(import_inputs
  neutral:neutral-v4.css
  slate_v3:shadcn-v3-hsl.css
  tweak:tweakcn-export.css)

set(import_outputs)
foreach(entry ${import_inputs})
  string(REPLACE ":" ";" parts ${entry})
  list(GET parts 0 theme)
  list(GET parts 1 fixture)
  set(output ${CMAKE_CURRENT_BINARY_DIR}/${theme}.inc)
  add_custom_command(
    OUTPUT ${output}
    COMMAND ${Python3_EXECUTABLE} ${import_script} ${CMAKE_CURRENT_LIST_DIR}/${fixture} --name ${theme} -o ${output}
    DEPENDS ${import_script} ${CMAKE_CURRENT_LIST_DIR}/../../scripts/shadcn_colors.py ${CMAKE_CURRENT_LIST_DIR}/${fixture}
    VERBATIM)
  list(APPEND import_outputs ${output})
endforeach()

add_executable(test_import_c check.c ${import_outputs})
target_include_directories(test_import_c PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(test_import_c PRIVATE lvgl_theme_shadcn)
set_target_properties(test_import_c PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
if(NOT MSVC)
  target_compile_options(test_import_c PRIVATE -Wall -Wextra $<$<BOOL:${LV_THEME_SHADCN_WERROR}>:-Werror>)
endif()

add_test(NAME test_import_c COMMAND test_import_c)
add_test(NAME test_import_script COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/run.py)
