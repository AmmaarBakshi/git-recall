#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#else
#include <unistd.h>
#endif
#include "color.h"

/* ─────────────────────────────────────────────
   color.c  —  color toggle + shared print helpers
   ───────────────────────────────────────────── */

int use_color = 1;   /* default: colors ON */

void init_color(void) {
    const char *nc = getenv("NO_COLOR");
    if ((nc && nc[0] != '\0') || !isatty(fileno(stdout)))
        use_color = 0;
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
    fprintf(stderr, "%s[git-recall error]%s %s\n",
            C(COL_RED), C(COL_RESET), msg);
}
