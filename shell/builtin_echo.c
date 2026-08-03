/* echo as a gush builtin: rename gu_main and reuse the tool source. */
#define gu_main gu_echo_main
#include "../src/echo.c"
