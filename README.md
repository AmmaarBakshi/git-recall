# git-recall

[![winget](https://img.shields.io/badge/winget-AMMAAR--IC.git--recall-blue?logo=windows)](https://github.com/microsoft/winget-pkgs)
[![APT PPA](https://img.shields.io/badge/PPA-ammaar--apt%2Fgit--recall-orange?logo=ubuntu)](https://launchpad.net/~ammaar-apt/+archive/ubuntu/git-recall)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Language: C](https://img.shields.io/badge/language-C-blue.svg)]()
[![GitHub release](https://img.shields.io/github/v/release/AMMAAR-IC/git-recall)](https://github.com/AMMAAR-IC/git-recall/releases)
[![GitHub stars](https://img.shields.io/github/stars/AMMAAR-IC/git-recall)](https://github.com/AMMAAR-IC/git-recall/stargazers)

> Your personal git standup tool. See what you (and your team) did — by day, week, month, or year.

Built in pure C. No dependencies. Just git.

---

## Install

### Windows — winget
```powershell
winget install AMMAAR-IC.git-recall
```

### Ubuntu / Debian — apt
```bash
sudo add-apt-repository ppa:ammaar-apt/git-recall
sudo apt update
sudo apt install git-recall
```

### Build from Source
```bash
git clone https://github.com/AMMAAR-IC/git-recall
cd git-recall
make
sudo make install
```

---

## Usage

```bash
git-recall                    # default: last 1 week
git-recall --day              # last 1 day
git-recall --week             # last 1 week
git-recall --month            # last 1 month
git-recall --year             # last 1 year
```

### Multiplier flag `-N`

```bash
git-recall --day   -3         # last 3 days
git-recall --week  -2         # last 2 weeks
git-recall --month -2         # last 2 months
git-recall --year  -2         # last 2 years
```

### Output to file

```bash
git-recall --month > recall.txt          # write to existing file
git-recall --month > -mk recall.txt      # create file and write
```

---

## Example Output

```
──────────────────────────────────────────────────────
  git recall  —  Last Week  (since 2026-04-04)
──────────────────────────────────────────────────────
  2026-04-11
  448e66e  git-recall 0.1.0 : the base version  @ 19:41  AMMAAR-IC
──────────────────────────────────────────────────────
  Total commits: 1
──────────────────────────────────────────────────────
```

---

## Windows — Fix Garbled Characters in PowerShell

On Windows, PowerShell may display box-drawing characters (`─`, `—`) as garbled text like `ΓöÇ`.
This is a UTF-8 encoding issue, not a bug in git-recall.

### Quick Fix (current session only)

```powershell
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
git-recall --week
```

### Permanent Fix

```powershell
# open your PowerShell profile
notepad $PROFILE

# add this line and save
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
```

Restart PowerShell and git-recall will render correctly from now on.

### Best Fix — Use Windows Terminal

Install **Windows Terminal** from the Microsoft Store — full Unicode support out of the box, no configuration needed.

---

## Error Messages

| Situation | Message |
|---|---|
| Not in a git repo | `Not a git repository. Run 'git init' first.` |
| Unknown flag | `Unknown option: --xyz` |
| Missing filename after `>` | `Expected filename after '>'` |
| Cannot open/create file | `Cannot open file 'x': <reason>` |

---

## Uninstall

### Windows
```powershell
winget uninstall Ammaar.git-recall
```

### Ubuntu / Debian
```bash
sudo apt remove git-recall
```

### From Source
```bash
sudo make uninstall
```

---

## License

MIT — see [LICENSE](LICENSE)