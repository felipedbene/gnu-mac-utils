/* pwd as a gush builtin: rename gu_main and reuse the tool source. */
#define gu_main gu_pwd_main
#include "../src/pwd.c"
