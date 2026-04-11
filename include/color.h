#ifndef COLOR_H
#define COLOR_H

/* ─────────────────────────────────────────────
   color.h  —  ANSI terminal color definitions
               and the global color toggle
   ───────────────────────────────────────────── */

#define COL_RESET   "\033[0m"
#define COL_BOLD    "\033[1m"
#define COL_RED     "\033[1;31m"
#define COL_GREEN   "\033[1;32m"
#define COL_YELLOW  "\033[1;33m"
#define COL_CYAN    "\033[1;36m"
#define COL_GRAY    "\033[0;37m"
#define COL_BLUE    "\033[1;34m"
#define COL_MAGENTA "\033[1;35m"

/* global flag — set to 0 when writing to a file
   so no ANSI escape codes pollute plain text     */
extern int use_color;

/* returns the color code if colors are on,
   or an empty string if they are off            */
const char *C(const char *code);

/* helpers used across multiple modules */
void print_separator(FILE *out);
void print_error(const char *msg);

#endif /* COLOR_H */
