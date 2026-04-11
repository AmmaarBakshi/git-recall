#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include "args.h"
#include "color.h"
#include "git.h"
#include "output.h"

/* ─────────────────────────────────────────────
   main.c  —  git-recall entry point

   Project layout:
     include/
       color.h   —  ANSI color codes + toggle
       args.h    —  CLI argument types + parser
       git.h     —  repo detection + date utils
       output.h  —  log printer + file opener
     src/
       color.c
       args.c
       git.c
       output.c
       main.c
   ───────────────────────────────────────────── */

int main(int argc, char *argv[]) {

    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    /* 1. make sure we are inside a git repository */
    if (!is_git_repo()) {
        print_error("Not a git repository (or any of the parent directories).");
        print_error("Run 'git init' to create one, or cd into an existing repo.");
        return 1;
    }

    /* 2. parse command-line arguments */
    RecallArgs args;
    if (parse_args(argc, argv, &args) != 0)
        return 1;

    /* 3. open output destination (stdout or a file) */
    FILE *out = open_output(&args);
    if (!out)
        return 1;

    /* 4. fetch git log and print */
    int ret = run_recall(out, args.period, args.multiplier);

    /* 5. close file and notify user if we wrote to one */
    if (out != stdout) {
        fclose(out);
        fprintf(stderr, "[git-recall] Output written to: %s\n", args.outfile);
    }

    return ret;
}
