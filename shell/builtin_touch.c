/* touch as a gush builtin: rename gu_main and reuse the tool source. */
#define gu_main gu_touch_main
#include "../src/touch.c"
