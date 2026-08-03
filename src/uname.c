/* uname [-a] — print system information (via Gestalt on classic Mac OS) */
#include "gucompat.h"

int gu_main(int argc, char **argv)
{
    char buf[128];

    (void)argc;
    (void)argv;
    gu_uname(buf, sizeof(buf));
    fprintf(gu_out(), "%s\n", buf);
    return 0;
}
