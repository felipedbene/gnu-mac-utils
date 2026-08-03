/* ls as a gush builtin: rename gu_main and reuse the tool source. */
#define gu_main gu_ls_main
#include "../src/ls.c"
