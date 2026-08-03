/* cat as a gush builtin: rename gu_main and reuse the tool source. */
#define gu_main gu_cat_main
#include "../src/cat.c"
