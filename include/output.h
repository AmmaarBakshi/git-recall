#ifndef OUTPUT_H
#define OUTPUT_H

/* ─────────────────────────────────────────────
   output.h  —  git log fetching and pretty-
                printing to a FILE stream
   ───────────────────────────────────────────── */

#include <stdio.h>
#include "args.h"

/* run git log for the given period/multiplier
   and pretty-print results to `out`.
   returns 0 on success, 1 on error             */
int run_recall(FILE *out, Period period, int mult);

/* open the output file described in args.
   returns the FILE* to write to (may be stdout),
   or NULL on error (error already printed)      */
FILE *open_output(const RecallArgs *args);

#endif /* OUTPUT_H */
