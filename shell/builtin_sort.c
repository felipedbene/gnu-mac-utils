/* sort as a gush builtin: rename gu_main and reuse the tool source. */
#define gu_main gu_sort_main
#include "../src/sort.c"
