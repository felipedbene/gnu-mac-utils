/* basename as a gush builtin: rename gu_main and reuse the tool source. */
#define gu_main gu_basename_main
#include "../src/basename.c"
