This is a solid tool. To make the README truly "pro-tier," we should focus on **visual hierarchy**, **feature highlights**, and **clearer technical specifications**. Since it's built in pure C, that’s a huge selling point (speed and portability) that we should emphasize.

Here is a revamped version of your README.

-----

# 📜 git-recall

[](https://github.com/microsoft/winget-pkgs)
[](https://launchpad.net/~ammaar-apt/+archive/ubuntu/git-recall)
[](https://www.google.com/search?q=)
[](https://www.google.com/search?q=LICENSE)

**git-recall** is your personal standup assistant. Built in **pure C** with zero dependencies, it provides a lightning-fast summary of what you and your team have accomplished over any timeframe.

[**Installation**](https://www.google.com/search?q=%23-install) • [**Usage**](https://www.google.com/search?q=%23-usage) • [**Troubleshooting**](https://www.google.com/search?q=%23-windows-encoding-fix) • [**Contributing**](https://www.google.com/search?q=%23license)

-----

## ✨ Features

  * **Zero Dependencies:** No Python, Node.js, or heavy runtimes. Just `git` and a C compiler.
  * **Flexible Scopes:** Quickly pivot between daily, weekly, monthly, or yearly summaries.
  * **Multipliers:** Look back $N$ number of days or weeks with simple flags.
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

The basic syntax is `git recall [range] [multiplier]`.

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

> [\!TIP]
> For the best experience, use **Windows Terminal**. It handles Unicode natively without any extra configuration.

-----

## ⚠️ Error Reference

| Message | Solution |
| :--- | :--- |
| `Not a git repository...` | Run the command inside a folder initialized with `git init`. |
| `Unknown option: --xyz` | Check `git recall --help` for valid flags. |
| `Expected filename after '>'` | Ensure you provide a path (e.g., `> report.txt`). |
| `Cannot open file 'x'` | Check folder permissions or if the file is locked by another app. |

-----

## 🛡 License

Distributed under the **MIT License**. See `LICENSE` for more information.

-----

*Created by [AmmaarBakshi](https://www.google.com/search?q=https://github.com/AmmaarBakshi)*