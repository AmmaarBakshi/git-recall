#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#else
#include <unistd.h>
#endif
#include "color.h"

/* ─────────────────────────────────────────────
   color.c  —  color toggle + shared print helpers
   ───────────────────────────────────────────── */

int use_color     = 1;   /* default: colors ON */
int use_color_err = 1;   /* same, for stderr   */

#ifdef _WIN32
/* legacy conhost prints ANSI codes literally unless VT processing
   is switched on; returns 0 if the console refuses              */
static int enable_vt(DWORD which) {
    HANDLE h = GetStdHandle(which);
    DWORD mode;
    if (h == INVALID_HANDLE_VALUE || !GetConsoleMode(h, &mode))
        return 0;
    if (mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING)
        return 1;
    return SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
}
#endif

void init_color(void) {
    const char *nc = getenv("NO_COLOR");
    int no_color = nc && nc[0] != '\0';

    if (no_color || !isatty(fileno(stdout)))
        use_color = 0;
    if (no_color || !isatty(fileno(stderr)))
        use_color_err = 0;

    #ifdef _WIN32
        if (use_color && !enable_vt(STD_OUTPUT_HANDLE))
            use_color = 0;
        if (use_color_err && !enable_vt(STD_ERROR_HANDLE))
            use_color_err = 0;
    #endif
}

const char *C(const char *code) {
    return use_color ? code : "";
}

void print_separator(FILE *out) {
    fprintf(out, "%s%s%s\n",
            C(COL_GRAY),
            "──────────────────────────────────────────────────────",
            C(COL_RESET));
}

void print_error(const char *msg) {
    const char *red   = use_color_err ? COL_RED   : "";
    const char *reset = use_color_err ? COL_RESET : "";
    fprintf(stderr, "%s[git-recall error]%s %s\n", red, reset, msg);
}
