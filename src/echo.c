/* echo [-n] [args...] */
#include "gucompat.h"

#include <string.h>

int gu_main(int argc, char **argv)
{
    int i = 1;
    int newline = 1;

    if (i < argc && strcmp(argv[i], "-n") == 0) {
        newline = 0;
        i++;
    }
    for (; i < argc; i++) {
        fputs(argv[i], gu_out());
        if (i + 1 < argc)
            fputc(' ', gu_out());
    }
    if (newline)
        fputc('\n', gu_out());
    return 0;
}
