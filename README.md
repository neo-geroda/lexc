# LEX-C
Lightweight Enhanced eXecution Code

## How to Run

1. Open the folder in terminal `../lexc`

2. Compile the file by typing `gcc src/main.c src/parser.c src/lexer.c src/token_stream.c src/token_list.c src/symbol_table.c src/lookupkeywords.c src/parser_tree.c  -I include -g -Wall -o compiler.exe`

3. Run by typing `.\compiler.exe tests/test8.lexc`


<hr>
don't touch - Kim
You can safely reset your local branch to match what’s on GitHub. The exact command depends on **whether you want to keep your local changes or discard them**. I’ll cover both, starting with the most common and safest path.

---

## Case 1: You want to **discard local changes** and go back to GitHub’s version (most common)

This will make your local `main` exactly match `origin/main`.

```bash
git fetch origin
git reset --hard origin/main
```

What this does:

* `git fetch origin` → downloads the latest state from GitHub
* `git reset --hard origin/main` → rewinds your local `main` to that exact commit and **throws away local edits**

⚠️ Warning: this permanently deletes uncommitted changes.

---

## Case 2: You want to **keep a backup of your local changes** (recommended if unsure)

First, stash or save your work, then reset.

### Option A: Stash changes

```bash
git stash
git fetch origin
git reset --hard origin/main
```

You can later recover your edits with:

```bash
git stash pop
```

### Option B: Commit to a temporary branch

```bash
git checkout -b backup-my-changes
git add .
git commit -m "Backup before resetting main"
git checkout main
git fetch origin
git reset --hard origin/main
```

This is the safest approach if the edits might be useful later.

---

## Case 3: You already committed bad changes to `main` (but didn’t push)

Just reset to the remote:

```bash
git fetch origin
git reset --hard origin/main
```

---

## Case 4: You already pushed the bad changes (important)

Do **not** hard reset unless you know what you’re doing. Instead:

```bash
git revert <commit-hash>
```

This creates a new commit that undoes the changes safely.

---

## Quick sanity check (optional)

Before resetting, you can see what’s different:

```bash
git status
git diff origin/main
```

---

### TL;DR (most likely what you want)

```bash
git fetch origin
git reset --hard origin/main
```

If you want, tell me:

* did you **commit** anything?
* did you **push** to GitHub?

I can tailor the safest command exactly to your situation.
