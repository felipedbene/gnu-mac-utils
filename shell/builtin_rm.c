/* rm as a gush builtin: rename gu_main and reuse the tool source. */
#define gu_main gu_rm_main
#include "../src/rm.c"
