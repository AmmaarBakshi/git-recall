# 📜 git-recall

**git-recall** is your personal standup assistant. Built in **pure C** with zero dependencies, it provides a lightning-fast summary of what you and your team have accomplished over any timeframe.

[**Installation**](#-install) • [**Usage**](#-usage) • [**Troubleshooting**](#-windows-encoding-fix) • [**Errors**](#️-error-reference) • [**License**](#-license)

-----

## ✨ Features

  * **Zero Dependencies:** No Python, Node.js, or heavy runtimes. Just `git` and a C compiler.
  * **Flexible Scopes:** Quickly pivot between daily, weekly, monthly, or yearly summaries.
  * **Multipliers:** Look back any number of days, weeks, months, or years with simple flags.
  * **Just Your Work:** `--me` shows only your own commits.
  * **Native File Export:** Built-in support for logging summaries to text files.
  * **Blazing Fast:** Written in C for near-instant execution even in massive monorepos.

-----

## 🚀 Install

### Windows (Winget)

```powershell
winget install Ammaar.git-recall
```

### Ubuntu / Debian (PPA)

```bash
sudo add-apt-repository ppa:ammaar-apt/git-recall
sudo apt update && sudo apt install git-recall
```

### Build from Source

Perfect for macOS or other Linux distros.

```bash
git clone https://github.com/AmmaarBakshi/git-recall
cd git-recall
make
sudo make install
```

-----

## 🛠 Usage

The basic syntax is `git recall [range] [multiplier] [--me]`.

### Quick Scopes

| Command | Description |
| :--- | :--- |
| `git recall` | Defaults to the **last 7 days** (same as `--week`) |
| `git recall --day` | Yesterday and today — handy for a morning standup |
| `git recall --week` | Summary of the last week |
| `git recall --month` | Summary of the last month |
| `git recall --year` | The "Year in Review" view |

Every range starts at **midnight** of its first day, so nothing from that morning is missed. Commits from all branches are included (merge commits are skipped), grouped by day with the newest first, and shown in **your local time** — even when teammates commit from other timezones.

### Lookback Multipliers

Need to see the last 3 days or 2 months? Just add the number:

```bash
git recall --day -3    # Last 3 days
git recall --month -2  # Last 2 months
```

Months and years are calendar-aware: on March 31, `--month` starts from February 28 (or 29), not March 3. Ranges that would reach before 1970 start at `1970-01-01`.

### Only Your Commits

Add `--me` to filter by your `git config user.email` — perfect for standups:

```bash
git recall --week --me
```

### Help & Colors

`git recall --help` prints all options. Colors are turned off automatically when the output is piped (e.g. `git recall | less`) or when the `NO_COLOR` environment variable is set.

### Exporting Reports

```bash
# Write to a file (creates it, or overwrites it if it exists)
git recall --month > recall.txt

# Let git-recall create the file itself and confirm where it wrote
git recall --month '>' -mk recall.txt
```

Note the quotes in the second example: an unquoted `>` is taken by your shell (bash, zsh, PowerShell, cmd), which would write to a file literally named `-mk`. Quoting it passes `>` to git-recall instead. Files are always written as plain text without color codes, and git-recall exits with an error if the write fails (for example, a full disk).

-----

## 🖥 Example Output

```text
──────────────────────────────────────────────────────
  git recall  —  Last Week  (since 2026-04-04)
──────────────────────────────────────────────────────
  2026-04-11
  448e66e  git-recall 0.1.0 : the base version  @ 19:41  AmmaarBakshi
──────────────────────────────────────────────────────
  Total commits: 1
──────────────────────────────────────────────────────
```

-----

## 🔧 Windows Encoding Fix

Since 1.6.0, git-recall switches the console to UTF-8 while it runs and **restores your original code page** when it exits. It also turns on ANSI color support in the classic Windows console, so you shouldn't see raw codes like `←[1;32m` any more. If a console can't show colors, git-recall prints plain text instead.

If you still see garbled characters like `ΓöÇ` instead of smooth lines `─`, it's usually when the output goes through PowerShell, e.g. `git recall | Out-File log.txt` or `$x = git recall`. That means PowerShell itself isn't reading the output as UTF-8.

**The Permanent Fix:**

1.  Open your profile: `notepad $PROFILE`
2.  Paste: `[Console]::OutputEncoding = [System.Text.Encoding]::UTF8`
3.  Restart PowerShell.

> [!TIP]
> For the best experience, use **Windows Terminal**. It handles Unicode natively without any extra configuration.

-----

## ⚠️ Error Reference

| Message | Solution |
| :--- | :--- |
| `Not a git repository...` | Run the command inside a folder initialized with `git init`. |
| `Unknown option: --xyz` | Check `git recall --help` for valid flags. |
| `Invalid multiplier '-x'` | Use a whole number from `-1` to `-10000`. |
| `--me needs git user.email` | Run `git config --global user.email you@example.com`. |
| `Expected filename after '>'` | Ensure you provide a path (e.g., `> report.txt`). |
| `Expected filename after '-mk'` | Put a path after `-mk` (e.g., `'>' -mk report.txt`). |
| `Cannot open file 'x'` | Check folder permissions or if the file is locked by another app. |
| `Unknown option: report.txt` | Your shell took an unquoted `>`. Quote it: `'>' -mk report.txt`. |
| `git log failed.` | git printed the real reason just above — often a corrupt or unreadable repository. |
| `Failed to write output.` / `Failed to write the output file.` | The disk is full, or the pipe/terminal was closed before git-recall finished. |

-----

## 🛡 License

Distributed under the **MIT License**. See `LICENSE` for more information.

-----

*Created by [AmmaarBakshi](https://github.com/AmmaarBakshi)* • [Releases](https://github.com/AmmaarBakshi/git-recall/releases) • [Report an issue](https://github.com/AmmaarBakshi/git-recall/issues)