/* mkdir as a gush builtin: rename gu_main and reuse the tool source. */
#define gu_main gu_mkdir_main
#include "../src/mkdir.c"
