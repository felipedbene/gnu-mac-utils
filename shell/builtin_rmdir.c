/* rmdir as a gush builtin: rename gu_main and reuse the tool source. */
#define gu_main gu_rmdir_main
#include "../src/rmdir.c"
