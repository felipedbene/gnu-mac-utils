/* tail as a gush builtin: rename gu_main and reuse the tool source. */
#define gu_main gu_tail_main
#include "../src/tail.c"
