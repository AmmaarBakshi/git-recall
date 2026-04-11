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

## Error Messages

| Situation | Message |
|---|---|
| Not in a git repo | `Not a git repository. Run 'git init' first.` |
| Unknown flag | `Unknown option: --xyz` |
| Missing filename after > | `Expected filename after '>'` |
| Cannot open/create file | `Cannot open file 'x': <reason>` |

---

## Uninstall

```bash
sudo make uninstall
```
