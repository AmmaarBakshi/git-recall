#ifndef GIT_H
#define GIT_H

/* ─────────────────────────────────────────────
   git.h  —  git repository checks and
             date/label utilities
   ───────────────────────────────────────────── */

#include "args.h"

/* returns 1 if current directory is inside a git repo,
   0 otherwise                                          */
int is_git_repo(void);

/* fills buf with a YYYY-MM-DD date string representing
   (now - mult * period).  e.g. period=MONTH, mult=2
   → two months ago                                     */
void build_since(char *buf, size_t sz, Period period, int mult);

/* fills buf with a human label like "Last 2 Months"    */
void period_label(char *buf, size_t sz, Period period, int mult);

#endif /* GIT_H */
