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
        /* rely on PATH — git is not always in C:\Program Files\Git */
        return system("git rev-parse --git-dir > NUL 2>&1") == 0;
    #else
        return system("git rev-parse --git-dir > /dev/null 2>&1") == 0;
    #endif
}

void build_since(char *buf, size_t sz, Period period, int mult) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    int day = t->tm_mday;

    switch (period) {
        case PERIOD_DAY:   t->tm_mday -= mult;       break;
        case PERIOD_WEEK:  t->tm_mday -= mult * 7;   break;
        case PERIOD_MONTH: t->tm_mon  -= mult;        break;
        case PERIOD_YEAR:  t->tm_year -= mult;        break;
    }

    /* clamp month/year jumps to the last day of the target month,
       otherwise e.g. Mar 31 - 1 month = "Feb 31" rolls over to Mar 3 */
    if (period == PERIOD_MONTH || period == PERIOD_YEAR) {
        t->tm_mday = 1;
        t->tm_isdst = -1;
        mktime(t);                       /* normalise month/year     */
        struct tm probe = *t;
        probe.tm_mon += 1;
        probe.tm_mday = 0;               /* day 0 = last of prev mon */
        probe.tm_isdst = -1;
        mktime(&probe);
        t->tm_mday = day < probe.tm_mday ? day : probe.tm_mday;
    }

    t->tm_isdst = -1;
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
