/* mv as a gush builtin: rename gu_main and reuse the tool source. */
#define gu_main gu_mv_main
#include "../src/mv.c"
