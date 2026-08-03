/* uniq as a gush builtin: rename gu_main and reuse the tool source. */
#define gu_main gu_uniq_main
#include "../src/uniq.c"
