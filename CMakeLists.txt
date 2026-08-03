# retro68-coreutils
#
# Cross build (the real target — classic Mac OS via Retro68):
#   cmake -S . -B build-m68k \
#     -DCMAKE_TOOLCHAIN_FILE=$RETRO68/m68k-apple-macos/cmake/retro68.toolchain.cmake
#   cmake --build build-m68k
#
#   cmake -S . -B build-ppc \
#     -DCMAKE_TOOLCHAIN_FILE=$RETRO68/powerpc-apple-macos/cmake/retroppc.toolchain.cmake
#   cmake --build build-ppc
#
# Building without a toolchain file produces host (POSIX) binaries for
# development; `make test` at the top of this directory does the same
# thing without CMake.

cmake_minimum_required(VERSION 3.9)
# CXX must be enabled even though all sources are C: add_application's
# CONSOLE option links RetroConsole, which is C++, and sets the target's
# LINKER_LANGUAGE to CXX.
project(retro68_coreutils C CXX)

set(GU_TOOLS
    true false echo cat head tail wc sort uniq basename dirname
    pwd ls mkdir rmdir rm mv cp touch date sleep uname)

set(GU_LIB_CORE
    lib/gufile.c
    lib/util.c
    lib/compat_mac.c
    lib/compat_posix.c)

set(GU_LIB_SOURCES
    ${GU_LIB_CORE}
    lib/tool_main.c)

# gush links every tool in as a builtin and has its own main().
set(GU_SHELL_SOURCES shell/gush.c ${GU_LIB_CORE})
foreach(tool IN LISTS GU_TOOLS)
    list(APPEND GU_SHELL_SOURCES shell/builtin_${tool}.c)
endforeach()

# Retro68's toolchain files use different system names per target:
# m68k = "Retro68", PowerPC = "RetroPPC", Carbon = "RetroCarbon".
if(CMAKE_SYSTEM_NAME MATCHES "Retro68|RetroPPC|RetroCarbon")
    foreach(tool IN LISTS GU_TOOLS)
        # CONSOLE links Retro68's console runtime so stdio gets a window.
        add_application(${tool} CONSOLE src/${tool}.c ${GU_LIB_SOURCES})
        target_include_directories(${tool} PRIVATE include)
        target_compile_definitions(${tool} PRIVATE
            GU_MACOS GU_TOOLNAME="${tool}")
    endforeach()
    add_application(gush CONSOLE ${GU_SHELL_SOURCES})
    target_include_directories(gush PRIVATE include)
    target_compile_definitions(gush PRIVATE GU_MACOS)
else()
    foreach(tool IN LISTS GU_TOOLS)
        add_executable(${tool} src/${tool}.c ${GU_LIB_SOURCES})
        target_include_directories(${tool} PRIVATE include)
        target_compile_definitions(${tool} PRIVATE
            GU_HOST GU_TOOLNAME="${tool}")
    endforeach()
    add_executable(gush ${GU_SHELL_SOURCES})
    target_include_directories(gush PRIVATE include)
    target_compile_definitions(gush PRIVATE GU_HOST)
endif()
