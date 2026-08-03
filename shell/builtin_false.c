/* false as a gush builtin: rename gu_main and reuse the tool source. */
#define gu_main gu_false_main
#include "../src/false.c"
