# Buffer Overflow — a 5-minute demo

**CS3SP — Secure Programming**

A tiny C "login" program with a classic buffer overflow. Type a password that's
too long and it overwrites the variable next to it in memory, unlocking access
you were never meant to have. No debugger, no special tools — just compile and
run.

> ⚠️ Change the badge URL below to your own `owner/repo` before publishing.

[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://codespaces.new/czekster/buffer-overflow-lab-simple)

## Start

**GitHub Codespaces (recommended — works on university PCs):**
Click the badge → **Create codespace**. A terminal opens with `gcc` already
installed — nothing to set up.

**Local:** any Linux / macOS / WSL with `gcc`.

Then:
```bash
make        # compile login.c
./login     # run it
```
and open **`TUTORIAL.md`** — the walkthrough.

## Files

| File | What it is |
|---|---|
| `login.c` | The deliberately vulnerable program |
| `Makefile` | Compiles it with the teaching flags |
| `TUTORIAL.md` | **Start here** — the student walkthrough |

## For instructors

- Runs on the default Codespaces image (gcc included) — no Dockerfile or
  devcontainer needed.
- The secret password is `s3cr3t` and the overflow bypass triggers at 9+
  characters; change either in `login.c` to suit your marking.
- This lab is its own repo on purpose, so students on earlier labs never see it.
