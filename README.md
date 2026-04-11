# git-recall

> Your personal git standup tool. See what you (and your team) did — by day, week, month, or year.

Built in pure C. No dependencies. Just git.

---

## Install

```bash
git clone https://github.com/yourname/git-recall
cd git-recall
make
sudo make install
```

---

## Usage

```bash
git recall                    # default: last 1 week
git recall --day              # last 1 day
git recall --week             # last 1 week
git recall --month            # last 1 month
git recall --year             # last 1 year
```

### Multiplier flag `-N`

```bash
git recall --day   -3         # last 3 days
git recall --week  -2         # last 2 weeks
git recall --month -2         # last 2 months
git recall --year  -2         # last 2 years
```

### Output to file

```bash
git recall --month > recall.txt          # write to existing file
git recall --month > -mk recall.txt      # create file and write
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

On Windows, PowerShell may display box-drawing characters (`─`, `—`) as garbled text like `ΓöÇ`. This is a UTF-8 encoding issue, not a bug in git-recall.

### Quick Fix (current session only)

Run this once before using git-recall:

```powershell
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
./git-recall.exe --week
```

### Permanent Fix (applies to every session)

Add the encoding line to your PowerShell profile so it loads automatically:

```powershell
# Step 1 — open your PowerShell profile in Notepad
notepad $PROFILE

# Step 2 — add this line to the file and save
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
```

Restart PowerShell and git-recall will render correctly from now on.

### Best Fix — Use Windows Terminal

If you haven't already, install **Windows Terminal** from the Microsoft Store. It has full Unicode support out of the box and requires no configuration — git-recall will render perfectly with no extra steps.

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

```bash
sudo make uninstall
```

---

## License

MIT