# 📜 git-recall

**git-recall** is your personal standup assistant. Built in **pure C** with zero dependencies, it provides a lightning-fast summary of what you and your team have accomplished over any timeframe.

[**Installation**](#-install) • [**Usage**](#-usage) • [**Troubleshooting**](#-windows-encoding-fix) • [**License**](#-license)

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
| `git recall` | Defaults to the **last 7 days** |
| `git recall --day` | See today's progress |
| `git recall --week` | Summary of the last week |
| `git recall --month` | Summary of the last month |
| `git recall --year` | The "Year in Review" view |

### Lookback Multipliers

Need to see the last 3 days or 2 months? Just add the number:

```bash
git recall --day -3    # Last 3 days
git recall --month -2  # Last 2 months
```

### Only Your Commits

Add `--me` to filter by your `git config user.email` — perfect for standups:

```bash
git recall --week --me
```

### Help & Colors

`git recall --help` prints all options. Colors are turned off automatically when the output is piped (e.g. `git recall | less`) or when the `NO_COLOR` environment variable is set.

### Exporting Reports

```bash
# Append/Write to an existing file
git recall --month > recall.txt

# Force create a new file and write
git recall --month > -mk recall.txt
```

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

If you see garbled characters like `ΓöÇ` instead of smooth lines `─`, your PowerShell session isn't using UTF-8.

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
| `Cannot open file 'x'` | Check folder permissions or if the file is locked by another app. |

-----

## 🛡 License

Distributed under the **MIT License**. See `LICENSE` for more information.

-----

*Created by [AmmaarBakshi](https://www.google.com/search?q=https://github.com/AmmaarBakshi)*