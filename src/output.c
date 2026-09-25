#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "output.h"
#include "color.h"
#include "git.h"

/* ─────────────────────────────────────────────
   output.c  —  git log runner + pretty printer
   ───────────────────────────────────────────── */

#define MAX_CMD  2048
#define MAX_LINE 4096

/* ── open (or create) the output file ── */
FILE *open_output(const RecallArgs *args) {
    if (args->outfile[0] == '\0')
        return stdout;                  /* no file requested → stdout */

    use_color = 0;                      /* strip ANSI from file output */

    FILE *f = fopen(args->outfile, "w");
    if (!f) {
        char errmsg[RECALL_MAX_PATH * 2 + 128];
        if (args->make_file) {
            snprintf(errmsg, sizeof(errmsg),
                     "Cannot create file '%s': %s",
                     args->outfile, strerror(errno));
        } else {
            snprintf(errmsg, sizeof(errmsg),
                     "Cannot open file '%s': %s\n"
                     "  Tip: use '> -mk %s' to create it automatically.",
                     args->outfile, strerror(errno), args->outfile);
        }
        print_error(errmsg);
        return NULL;
    }

    if (args->make_file)
        fprintf(stderr, "[git-recall] Created file: %s\n", args->outfile);

    return f;
}

/* ── fetch git log and print commits ── */
int run_recall(FILE *out, Period period, int mult) {
    char since[64];
    char label[64];
    char cmd[MAX_CMD];
    char line[MAX_LINE];

    build_since(since, sizeof(since), period, mult);
    period_label(label, sizeof(label), period, mult);

    /* ── header ── */
    fprintf(out, "\n");
    print_separator(out);
    fprintf(out, "%s  git recall  —  %s  (since %s)%s\n",
            C(COL_BOLD), label, since, C(COL_RESET));
    print_separator(out);
    fprintf(out, "\n");

    /* ── build git log command ──
       format fields separated by 0x1F (unit separator),
       which cannot appear in a subject — unlike '|':
         %h  = short hash
         %ad = author date (formatted below)
         %an = author name
         %s  = commit subject                */
    snprintf(cmd, sizeof(cmd),
             "git log --all --since=\"%s\" "
             "--pretty=format:\"%%h%%x1f%%ad%%x1f%%an%%x1f%%s\" "
             "--date=format:\"%%Y-%%m-%%d %%H:%%M\" "
             "--no-merges",
             since);

    FILE *pipe = popen(cmd, "r");
    if (!pipe) {
        print_error("Failed to run git log.");
        return 1;
    }

    int  count     = 0;
    char last_date[16] = "";

    while (fgets(line, sizeof(line), pipe)) {

        /* strip trailing newline (and \r on Windows) */
        size_t len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
            line[--len] = '\0';

        /* split on unit separator; subject keeps the rest of the line */
        char *hash    = line;
        char *date    = strchr(hash, '\x1f');   if (!date)    continue; *date++    = '\0';
        char *author  = strchr(date, '\x1f');   if (!author)  continue; *author++  = '\0';
        char *subject = strchr(author, '\x1f'); if (!subject) continue; *subject++ = '\0';

        /* ── date group header (printed once per day) ── */
        char day[16] = "";
        strncpy(day, date, 10);
        day[10] = '\0';

        if (strcmp(day, last_date) != 0) {
            if (count > 0) fprintf(out, "\n");
            fprintf(out, "%s  %s%s\n",
                    C(COL_YELLOW), day, C(COL_RESET));
            strncpy(last_date, day, sizeof(last_date));
        }

        /* ── commit line ── */
        const char *time_part = (strlen(date) > 11) ? date + 11 : "";

        fprintf(out, "  %s%s%s  %s%s%s  %s@ %s  %s%s\n",
                C(COL_GREEN),  hash,      C(COL_RESET),
                C(COL_RESET),  subject,   C(COL_RESET),
                C(COL_GRAY),   time_part, author, C(COL_RESET));

        count++;
    }
    if (pclose(pipe) != 0) {
        /* git already printed its own message to stderr */
        print_error("git log failed.");
        return 1;
    }

    /* ── footer ── */
    fprintf(out, "\n");
    print_separator(out);

    if (count == 0) {
        fprintf(out, "%s  No commits found in this period.%s\n",
                C(COL_MAGENTA), C(COL_RESET));
    } else {
        fprintf(out, "%s  Total commits: %d%s\n",
                C(COL_CYAN), count, C(COL_RESET));
    }

    print_separator(out);
    fprintf(out, "\n");
    return 0;
}
