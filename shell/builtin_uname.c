/* uname as a gush builtin: rename gu_main and reuse the tool source. */
#define gu_main gu_uname_main
#include "../src/uname.c"
