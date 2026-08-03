# gnu-mac-utils

A clean-room rewrite of a core subset of the GNU coreutils for **classic
Mac OS (System 7 through Mac OS 9)**, built with the
[Retro68](https://github.com/autc04/Retro68) cross-compiler toolchain.

This is not GPL code from GNU coreutils — every tool is written from
scratch in portable C against a small compatibility layer, and is
licensed under this repository's MIT license. "GNU-style" here means the
familiar names, flags and behaviors, not the original sources.

Each utility builds as a standalone classic Mac console application
(68K or PowerPC). Double-click one and it opens a console window; since
classic Mac OS has no shell, a tool started without arguments prompts
for a command line first (quotes group words, so
`cat "Macintosh HD:My Notes"` works).

## The tools

| Tool | Flags | Notes |
|------|-------|-------|
| `true`, `false` | | exit status 0 / 1 |
| `echo` | `-n` | |
| `cat` | `-n` | line-oriented; CR, LF and CRLF inputs all handled |
| `head`, `tail` | `-n N`, legacy `-N` | |
| `wc` | `-l -w -c` | CR, LF and CRLF each count as one line ending |
| `sort` | `-n -r -u` | in-memory; bounded by the app's memory partition |
| `uniq` | `-c -d -u` | |
| `basename`, `dirname` | | HFS `:` separator on Mac, `/` on the host build |
| `pwd` | | prints the default directory as a full HFS path |
| `ls` | `-a -l` | `-a` shows Finder-invisible files; `-l` shows type/creator codes and **both fork sizes** |
| `mkdir` | `-p` | |
| `rmdir` | | |
| `rm` | `-r -f` | |
| `mv` | | same-volume moves are catalog operations; cross-volume file moves copy + delete |
| `cp` | `-r` | copies the data fork, the **resource fork** and the Finder info, so apps and documents survive intact |
| `touch` | | new files are created as `TEXT`/`ttxt` (SimpleText) documents |
| `date` | | |
| `sleep` | | blocks in `Delay()`; note cooperative multitasking |
| `uname` | | reports the Mac OS version via `Gestalt` and the CPU (68K / PowerPC) |

## gush — the shell

Classic Mac OS has no terminal, so the project ships one. `gush` is a
console application with **every utility linked in as a builtin**, a
persistent prompt showing the current HFS directory, and `cd`:

```
Macintosh HD:Stuff % sort "My Disk:words" | uniq -c > counts
Macintosh HD:Stuff % cd "Macintosh HD:Documents"
Macintosh HD:Documents % ls -l
```

Pipes and `<` / `>` / `>>` redirection are emulated with temporary
files between builtins (the OS has no processes to connect), which is
indistinguishable in practice for these tools. Syntax rules: separate
`|`, `<`, `>`, `>>` with spaces, and use double quotes around paths
containing spaces. `help` lists the builtins, `exit` quits.

`gush -c "command line"` runs a single command line and exits — that's
how the test suite drives it.

## Mac-isms you should expect

- **Paths are HFS paths.** `Macintosh HD:Documents:notes` is absolute
  (the first segment is a volume name); `:sub:file` and `file` are
  relative to the folder the tool was launched from. There is no `.` or
  `..` — the parent of `:` is spelled `::`... which we don't support;
  use full paths to go up.
- **Line endings are CR** in classic Mac text files. All text tools
  accept CR, LF and CRLF transparently.
- **Files have two forks** plus Finder metadata. `cp` and same-volume
  `mv` preserve all of it; `ls -l` shows data and resource fork sizes
  and the type/creator codes.
- **No environment, no pipes, no shell.** Each tool is its own
  application. stdin/stdout are the console window.
- A tool holds its console window open at exit
  (`press Return to quit`) so output doesn't vanish.

## Building for Mac OS (the point of all this)

1. Install [Retro68](https://github.com/autc04/Retro68) (see its README;
   `./build-toolchain.bash` produces a `Retro68-build/toolchain`
   directory).

2. Point `RETRO68` at the toolchain and build:

   ```sh
   export RETRO68=/path/to/Retro68-build/toolchain

   # 68K (System 7+, any Mac)
   cmake -S . -B build-m68k \
     -DCMAKE_TOOLCHAIN_FILE=$RETRO68/m68k-apple-macos/cmake/retro68.toolchain.cmake
   cmake --build build-m68k

   # PowerPC (recommended for Mac OS 8/9)
   cmake -S . -B build-ppc \
     -DCMAKE_TOOLCHAIN_FILE=$RETRO68/powerpc-apple-macos/cmake/retroppc.toolchain.cmake
   cmake --build build-ppc
   ```

   Or run both in one go with `./scripts/build-retro68.sh`.

   > On very old Retro68 checkouts the `CONSOLE` keyword of
   > `add_application` may be missing; update Retro68, or add
   > `target_link_libraries(<tool> RetroConsole)` per tool.

3. Each build directory then contains, per tool, a `.bin` (MacBinary),
   a `.dsk` disk image and an `.APPL` file. Get them onto the Mac side
   however you like:
   - **Emulators** (Basilisk II, SheepShaver, QEMU `qemu-system-ppc`
     running Mac OS 9): mount the `.dsk`, or drop the `.bin` into a
     shared folder and decode it with StuffIt Expander.
   - **Real hardware**: `LaunchAPPL` (ships with Retro68) can send apps
     over serial/TCP, or transfer the `.bin` via floppy/CF/network.

Double-click a tool, type arguments at the prompt, read the output,
press Return to close.

## Building and testing on a modern machine

The same sources compile against a POSIX implementation of the compat
layer, which is how the logic is developed and CI-tested:

```sh
make        # host binaries in bin/
make test   # runs tests/run_tests.sh (65 checks)
```

## CI

Every push runs two pipelines (OpenTTD-style):

- **host-build-and-test** — POSIX build plus the full test suite.
- **mac-cross** — a real cross-compile of every tool for 68K and
  PowerPC inside the official Retro68 toolchain container
  (`ghcr.io/autc04/retro68`). The resulting `.bin` / `.dsk` / `.APPL`
  files are downloadable as per-target workflow artifacts, so each
  commit yields binaries you can drop straight into an emulator.

Pushing a `v*` tag additionally packages both targets into zips and
attaches them to a GitHub Release.

## How it's put together

```
include/gucompat.h    the portability API every tool is written against
lib/compat_mac.c      classic Mac OS implementation (HFS File Manager:
                      FSSpec, PBGetCatInfo, PBCatMove, forks, Gestalt...)
lib/compat_posix.c    POSIX implementation for development and tests
lib/gufile.c          buffered read-only file handles + CR/LF/CRLF-aware
                      line reader (shared by both platforms)
lib/util.c            tiny getopt, HFS/POSIX path helpers, diagnostics
lib/tool_main.c       shared main(): argument prompt + exit pause on Mac
src/<tool>.c          one file per utility, platform-neutral
shell/gush.c          the shell: prompt, parser, pipe/redirect plumbing
shell/builtin_*.c     2-line wrappers linking each tool into gush
```

The rule of the codebase: **utilities never touch platform APIs.**
Anything platform-specific goes through `gucompat.h`, which keeps every
tool testable on the host and keeps the Mac quirks (forks, HFS paths,
the 1904 epoch, invisible files) in exactly one place.

## Status / roadmap

Implemented: the 22 tools above, the gush shell (pipes, redirection,
cd), host test suite, dual-target build.

Not yet: `grep`/`sed`-class text tools, `df`/`du` (PBHGetVInfo),
`find`, MPW tool variants, globbing, and shell niceties like history
and `;` command lists.
