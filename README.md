# Compiled-app security lab (two parts)

**CS3SP — Secure Programming**

Two short, self-contained C labs on the security of compiled applications. No
debugger, no special tools — just compile and run.

- **Part 1 — Buffer overflow** (repo root): a toy "login" where typing an
  over-long password overflows into the neighbouring variable and bypasses the
  check.
- **Part 2 — Command injection & least privilege** (`part2/`): an admin tool
  that shells out with your input, letting you smuggle in your own commands —
  then how OS permissions and least privilege contain the damage.

> ⚠️ Change the badge URL below to your own `owner/repo` before publishing.

[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://codespaces.new/czekster/buffer-overflow-lab-simple)

## Start

**GitHub Codespaces (recommended — works on university PCs):**
Click the badge → **Create codespace**. A terminal opens with `gcc` already
installed — nothing to set up.

**Docker Desktop (local):** from the repo folder —
```powershell
docker compose run --rm lab
# then, inside the container:
make && ./login          # Part 1
cd part2 && make && ./fileinfo /etc/hostname   # Part 2
```

**Plain local:** any Linux / macOS / WSL with `gcc`.

- **Part 1:** `make && ./login` — then follow `TUTORIAL.md`.
- **Part 2:** `cd part2 && make` — then follow `part2/TUTORIAL.md`.

## Files

| File | What it is |
|---|---|
| `login.c` | Part 1 — the deliberately vulnerable program |
| `TUTORIAL.md` | Part 1 walkthrough — **start here** |
| `part2/fileinfo.c` | Part 2 — the command-injection tool |
| `part2/TUTORIAL.md` | Part 2 walkthrough |
| `Makefile` / `part2/Makefile` | Compile each part with the teaching flags |
| `docker-compose.yml` | Optional — for running the labs via Docker Desktop |

## For instructors

- Runs on the default Codespaces image (gcc included) — no Dockerfile or
  devcontainer needed.
- Part 1: password is `s3cr3t`, overflow bypass triggers at 9+ chars.
- Part 2: command injection via `system()`; the fix is `execvp` (no shell) plus
  least privilege. Tie-in to the lecture's ShellShock / CWE-78 / confining-apps
  material.
- This lab is its own repo on purpose, so students on earlier labs never see it.
