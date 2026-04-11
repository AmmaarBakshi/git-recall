#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "args.h"
#include "color.h"

/* ─────────────────────────────────────────────
   args.c  —  CLI argument parser for git-recall
   ───────────────────────────────────────────── */

void print_usage(void) {
    fprintf(stderr, "Usage: git recall [--day|--week|--month|--year] [-N] [> [-mk] file]\n");
    fprintf(stderr, "  Examples:\n");
    fprintf(stderr, "    git recall                   # last week (default)\n");
    fprintf(stderr, "    git recall --month           # last month\n");
    fprintf(stderr, "    git recall --month -2        # last 2 months\n");
    fprintf(stderr, "    git recall --year > log.txt  # write to file\n");
    fprintf(stderr, "    git recall --day > -mk out.txt  # create file & write\n");
}

int parse_args(int argc, char *argv[], RecallArgs *a) {
    /* set defaults */
    a->period     = PERIOD_WEEK;
    a->multiplier = 1;
    a->outfile[0] = '\0';
    a->make_file  = 0;

    for (int i = 1; i < argc; i++) {

        /* ── period flags ── */
        if (strcmp(argv[i], "--day")   == 0) { a->period = PERIOD_DAY;   continue; }
        if (strcmp(argv[i], "--week")  == 0) { a->period = PERIOD_WEEK;  continue; }
        if (strcmp(argv[i], "--month") == 0) { a->period = PERIOD_MONTH; continue; }
        if (strcmp(argv[i], "--year")  == 0) { a->period = PERIOD_YEAR;  continue; }

        /* ── multiplier  -N  (e.g. -2, -3) ── */
        if (argv[i][0] == '-' && argv[i][1] >= '1' && argv[i][1] <= '9') {
            a->multiplier = atoi(argv[i] + 1);
            if (a->multiplier <= 0) {
                print_error("Multiplier must be a positive number (e.g. -2).");
                return 1;
            }
            continue;
        }

        /* ── redirect operator > ── */
        if (strcmp(argv[i], ">") == 0) {
            if (i + 1 >= argc) {
                print_error("Expected filename after '>'.");
                print_usage();
                return 1;
            }

            /* optional -mk flag */
            if (strcmp(argv[i + 1], "-mk") == 0) {
                a->make_file = 1;
                i++;
                if (i + 1 >= argc) {
                    print_error("Expected filename after '-mk'.");
                    print_usage();
                    return 1;
                }
            }

            strncpy(a->outfile, argv[i + 1], MAX_PATH - 1);
            a->outfile[MAX_PATH - 1] = '\0';
            i++;
            continue;
        }

        /* ── unknown flag ── */
        char errmsg[128];
        snprintf(errmsg, sizeof(errmsg), "Unknown option: %s", argv[i]);
        print_error(errmsg);
        print_usage();
        return 1;
    }

    return 0;
}
