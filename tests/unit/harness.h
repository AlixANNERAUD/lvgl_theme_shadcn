#ifndef HARNESS_H
#define HARNESS_H

#include <stdio.h>
#include "lv_theme_shadcn.h"

extern int harness_failures;

void harness_begin(const lv_theme_shadcn_palette_t * palette);
void harness_end(void);
void harness_settle(void);
lv_display_t * harness_display(void);
const lv_theme_shadcn_fonts_t * harness_fonts(void);
lv_display_t * harness_create_display(void);
int harness_result(void);

#define CHECK(cond) do { if(!(cond)) { printf("  %s:%d: CHECK(%s) failed\n", __FILE__, __LINE__, #cond); harness_failures++; } } while(0)
#define CHECK_INT(actual, expected) do { long a_ = (long)(actual); long e_ = (long)(expected); if(a_ != e_) { printf("  %s:%d: %s == %ld, expected %ld\n", __FILE__, __LINE__, #actual, a_, e_); harness_failures++; } } while(0)
#define CHECK_COLOR(actual, expected) do { lv_color_t a_ = (actual); lv_color_t e_ = (expected); if(!lv_color_eq(a_, e_)) { printf("  %s:%d: %s == #%06x, expected #%06x\n", __FILE__, __LINE__, #actual, (unsigned)(lv_color_to_u32(a_) & 0xffffff), (unsigned)(lv_color_to_u32(e_) & 0xffffff)); harness_failures++; } } while(0)
#define RUN(test) do { printf("- %s\n", #test); harness_begin(lv_theme_shadcn_palette_zinc_dark()); test(); harness_end(); } while(0)
#define RUN_RAW(test) do { printf("- %s\n", #test); test(); } while(0)

#endif
