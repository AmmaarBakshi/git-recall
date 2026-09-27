#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#endif
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

#ifdef _WIN32
/* SetConsoleOutputCP changes the code page of the whole console,
   not just this process — put the user's setting back on exit   */
static UINT saved_cp;
static void restore_cp(void) { SetConsoleOutputCP(saved_cp); }
#endif

int main(int argc, char *argv[]) {

    #ifdef _WIN32
        saved_cp = GetConsoleOutputCP();
        if (saved_cp != 0 && saved_cp != CP_UTF8) {
            SetConsoleOutputCP(CP_UTF8);
            atexit(restore_cp);
        }
    #endif

    init_color();

    /* 1. parse command-line arguments (before the repo check,
          so --help works anywhere)                             */
    RecallArgs args;
    if (parse_args(argc, argv, &args) != 0)
        return 1;

    if (args.show_help) {
        print_usage();
        return 0;
    }

    /* 2. make sure we are inside a git repository */
    if (!is_git_repo()) {
        print_error("Not a git repository (or any of the parent directories).");
        print_error("Run 'git init' to create one, or cd into an existing repo.");
        return 1;
    }

    /* 3. open output destination (stdout or a file) */
    FILE *out = open_output(&args);
    if (!out)
        return 1;

    /* 4. fetch git log and print */
    int ret = run_recall(out, args.period, args.multiplier, args.only_me);

    /* 5. flush/close and check for write errors (disk full, closed
          pipe, ...) — only claim success if everything landed      */
    int write_err = ferror(out);
    if (out != stdout)
        write_err |= fclose(out) != 0;
    else
        write_err |= fflush(stdout) != 0;

    if (write_err) {
        print_error(out != stdout ? "Failed to write the output file."
                                  : "Failed to write output.");
        return 1;
    }

    if (out != stdout && ret == 0)
        fprintf(stderr, "[git-recall] Output written to: %s\n", args.outfile);

    return ret;
}
