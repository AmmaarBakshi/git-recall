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

/* ── read `git config user.email` into buf.
      returns 0 on success, 1 if unset or unsafe ── */
static int get_user_email(char *buf, size_t sz) {
    FILE *p = popen("git config user.email", "r");
    if (!p) return 1;
    if (!fgets(buf, (int)sz, p)) buf[0] = '\0';
    pclose(p);

    buf[strcspn(buf, "\r\n")] = '\0';
    /* refuse characters that could break out of the quoted shell arg */
    if (buf[0] == '\0' || strpbrk(buf, "\"`$%!\\") != NULL)
        return 1;
    return 0;
}

/* ── replace terminal control characters in place, so a commit
      subject can't smuggle escape sequences into the terminal.
      covers C0 (0x00-0x1F), DEL, and UTF-8 encoded C1 (U+0080-U+009F),
      e.g. U+009B which some terminals treat as CSI ── */
static void sanitize(char *s) {
    unsigned char *p = (unsigned char *)s;
    for (; *p; p++) {
        if (*p < 0x20 || *p == 0x7f) {
            *p = '?';
        } else if (*p == 0xc2 && p[1] >= 0x80 && p[1] <= 0x9f) {
            p[0] = '?';
            p[1] = '?';
            p++;
        }
    }
}

/* ── one parsed git log line; all fields point into hash's buffer ── */
typedef struct {
    char   *hash;
    char   *date;      /* "YYYY-MM-DD HH:MM" — sorts as a string */
    char   *author;
    char   *subject;
    size_t  seq;       /* position in git's output, for stable ties */
} Commit;

/* newest date first; equal dates keep git's order */
static int cmp_commit(const void *pa, const void *pb) {
    const Commit *a = pa, *b = pb;
    int d = strcmp(b->date, a->date);
    if (d != 0) return d;
    return (a->seq > b->seq) - (a->seq < b->seq);
}

static void free_commits(Commit *commits, size_t n) {
    for (size_t i = 0; i < n; i++)
        free(commits[i].hash);
    free(commits);
}

/* ── fetch git log and print commits ── */
int run_recall(FILE *out, Period period, int mult, int only_me) {
    char since[64];
    char label[64];
    char cmd[MAX_CMD];
    char line[MAX_LINE];

    char author_opt[320] = "";

    build_since(since, sizeof(since), period, mult);
    period_label(label, sizeof(label), period, mult);

    if (only_me) {
        char email[256];
        if (get_user_email(email, sizeof(email)) != 0) {
            print_error("--me needs git user.email to be set "
                        "(git config user.email you@example.com).");
            return 1;
        }
        snprintf(author_opt, sizeof(author_opt), "--author=\"<%s>\" ", email);
        strncat(label, " (mine)", sizeof(label) - strlen(label) - 1);
    }

    /* ── header ── */
    fprintf(out, "\n");
    print_separator(out);
    fprintf(out, "%s  git recall  —  %s  (since %s)%s\n",
            C(COL_BOLD), label, since, C(COL_RESET));
    print_separator(out);
    fprintf(out, "\n");

    /* ── build git log command ──
       --since gets an explicit midnight: a bare date makes git use
       the current time of day, dropping the start of the first day.
       --no-show-signature: with log.showSignature=true git would run
       gpg for every commit, which is slow and output we don't use.
       format fields separated by 0x1F (unit separator),
       which cannot appear in a subject — unlike '|':
         %h  = short hash
         %ad = author date, in local time so
                commits from other timezones sort
                and group consistently
         %an = author name
         %s  = commit subject                */
    snprintf(cmd, sizeof(cmd),
             "git log --all --since=\"%s 00:00:00\" %s"
             "--pretty=format:\"%%h%%x1f%%ad%%x1f%%an%%x1f%%s\" "
             "--date=format-local:\"%%Y-%%m-%%d %%H:%%M\" "
             "--no-merges --no-show-signature",
             since, author_opt);

    FILE *pipe = popen(cmd, "r");
    if (!pipe) {
        print_error("Failed to run git log.");
        return 1;
    }

    /* ── collect commits first, then sort and print ── */
    Commit *commits = NULL;
    size_t  n = 0, cap = 0;
    int     oom = 0;

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

        sanitize(author);
        sanitize(subject);

        if (n == cap) {
            size_t ncap = cap ? cap * 2 : 64;
            Commit *grown = realloc(commits, ncap * sizeof(*commits));
            if (!grown) { oom = 1; break; }
            commits = grown;
            cap = ncap;
        }

        /* one allocation holds all four fields back to back */
        char *buf = malloc(len + 1);
        if (!buf) { oom = 1; break; }
        memcpy(buf, line, len + 1);

        Commit *c  = &commits[n];
        c->hash    = buf;
        c->date    = buf + (date    - line);
        c->author  = buf + (author  - line);
        c->subject = buf + (subject - line);
        c->seq     = n;
        n++;
    }

    if (oom) {
        /* drain so git doesn't die on a broken pipe */
        while (fgets(line, sizeof(line), pipe)) {}
        pclose(pipe);
        free_commits(commits, n);
        print_error("Out of memory.");
        return 1;
    }

    if (pclose(pipe) != 0) {
        /* git already printed its own message to stderr */
        free_commits(commits, n);
        print_error("git log failed.");
        return 1;
    }

    /* git lists commits in graph order, but we group by author date;
       after a rebase or cherry-pick those disagree and the same day
       header would appear twice. sort newest-first by author date. */
    if (n > 1)
        qsort(commits, n, sizeof(*commits), cmp_commit);

    char last_date[16] = "";

    for (size_t i = 0; i < n; i++) {
        const Commit *c = &commits[i];

        /* ── date group header (printed once per day) ── */
        char day[16] = "";
        strncpy(day, c->date, 10);
        day[10] = '\0';

        if (strcmp(day, last_date) != 0) {
            if (i > 0) fprintf(out, "\n");
            fprintf(out, "%s  %s%s\n",
                    C(COL_YELLOW), day, C(COL_RESET));
            strncpy(last_date, day, sizeof(last_date));
        }

        /* ── commit line ── */
        const char *time_part = (strlen(c->date) > 11) ? c->date + 11 : "";

        fprintf(out, "  %s%s%s  %s%s%s  %s@ %s  %s%s\n",
                C(COL_GREEN),  c->hash,    C(COL_RESET),
                C(COL_RESET),  c->subject, C(COL_RESET),
                C(COL_GRAY),   time_part,  c->author, C(COL_RESET));
    }

    int count = (int)n;
    free_commits(commits, n);

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
