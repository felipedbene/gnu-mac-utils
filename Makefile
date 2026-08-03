# Host (POSIX) build of retro68-coreutils, used for development and the
# automated tests. The Mac OS build goes through CMake + the Retro68
# toolchain — see README.md.

CC ?= cc
CFLAGS ?= -O2 -std=gnu99 -Wall -Wextra

TOOLS = true false echo cat head tail wc sort uniq basename dirname \
        pwd ls mkdir rmdir rm mv cp touch date sleep uname

LIBSRC_CORE = lib/gufile.c lib/util.c lib/compat_posix.c lib/compat_mac.c
LIBSRC = $(LIBSRC_CORE) lib/tool_main.c
SHELL_SRC = shell/gush.c $(TOOLS:%=shell/builtin_%.c)

all: $(TOOLS:%=bin/%) bin/gush

bin/%: src/%.c $(LIBSRC) include/gucompat.h | bin
	$(CC) $(CFLAGS) -DGU_HOST -DGU_TOOLNAME=\"$*\" -Iinclude \
	    src/$*.c $(LIBSRC) -o $@

# The shell links every tool in as a builtin (via the shell/builtin_*.c
# wrappers) and provides its own main(), so tool_main.c is excluded.
bin/gush: $(SHELL_SRC) $(TOOLS:%=src/%.c) $(LIBSRC_CORE) include/gucompat.h | bin
	$(CC) $(CFLAGS) -DGU_HOST -Iinclude \
	    $(SHELL_SRC) $(LIBSRC_CORE) -o $@

bin:
	mkdir -p bin

test: all
	tests/run_tests.sh

clean:
	rm -rf bin

.PHONY: all test clean
