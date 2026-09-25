#ifndef ARGS_H
#define ARGS_H

/* ─────────────────────────────────────────────
   args.h  —  CLI argument types and parser
   ───────────────────────────────────────────── */

#define RECALL_MAX_PATH 512

/* period constants */
typedef enum {
    PERIOD_DAY   = 0,
    PERIOD_WEEK  = 1,
    PERIOD_MONTH = 2,
    PERIOD_YEAR  = 3
} Period;

/* all parsed options in one struct */
typedef struct {
    Period  period;             /* which time unit          */
    int     multiplier;         /* how many of that unit    */
    char    outfile[RECALL_MAX_PATH];  /* output filename, or ""   */
    int     make_file;          /* 1 if -mk was given       */
    int     show_help;          /* 1 if --help / -h given   */
    int     only_me;            /* 1 if --me: own commits   */
} RecallArgs;

/* parse argv into a RecallArgs struct.
   returns 0 on success, 1 on error (error already printed) */
int parse_args(int argc, char *argv[], RecallArgs *out);

/* print usage hint to stderr */
void print_usage(void);

#endif /* ARGS_H */
