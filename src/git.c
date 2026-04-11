#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "git.h"

/* ─────────────────────────────────────────────
   git.c  —  repo detection + date helpers
   ───────────────────────────────────────────── */

int is_git_repo(void) {
    #ifdef _WIN32
        return system("\"C:\\Program Files\\Git\\bin\\git.exe\" rev-parse --git-dir > NUL 2>&1") == 0;
    #else
        return system("git rev-parse --git-dir > /dev/null 2>&1") == 0;
    #endif
}

void build_since(char *buf, size_t sz, Period period, int mult) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    switch (period) {
        case PERIOD_DAY:   t->tm_mday -= mult;       break;
        case PERIOD_WEEK:  t->tm_mday -= mult * 7;   break;
        case PERIOD_MONTH: t->tm_mon  -= mult;        break;
        case PERIOD_YEAR:  t->tm_year -= mult;        break;
    }

    mktime(t);   /* normalise — handles day/month roll-over */
    strftime(buf, sz, "%Y-%m-%d", t);
}

void period_label(char *buf, size_t sz, Period period, int mult) {
    const char *names[] = { "Day", "Week", "Month", "Year" };
    if (mult == 1)
        snprintf(buf, sz, "Last %s", names[period]);
    else
        snprintf(buf, sz, "Last %d %ss", mult, names[period]);
}
