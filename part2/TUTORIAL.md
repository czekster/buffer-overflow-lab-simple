# Lab Part 2: Command Injection & Least Privilege

**CS3SP — Secure Programming**

Part 1 showed a bug *inside* a program's memory. Part 2 shows a bug at the
*boundary* between a program and the operating system: when a program asks the
OS shell to run a command and lets the user's input become part of that
command, the user can smuggle in commands of their own. This is **OS command
injection (CWE-78)** — number 5 on the industry's most-dangerous list, and the
same family as the ShellShock bug from the lecture.

**Time:** ~30–40 minutes. **Difficulty:** gentle–medium.

---

## 1. The program

Open `fileinfo.c`. It's a small admin tool: give it a filename and it shows
that file's details by asking the shell to run `ls -l` on it:

```c
snprintf(cmd, sizeof(cmd), "ls -l %s", argv[1]);  // build the command line
system(cmd);                                       // hand it to /bin/sh
```

The problem: whatever you type is pasted straight into a line that the **shell**
interprets. And to a shell, characters like `;`, `&&`, `|` and backticks don't
mean "part of a filename" — they mean "here comes another command."

---

## 2. Build and run it normally

```bash
make
./fileinfo /etc/hostname
```

You'll see the normal `ls -l` output for that file. So far it behaves like a
harmless tool.

---

## 3. Inject your own command

A filename can't contain a semicolon in any sensible use — but the shell will
happily act on one. Try:

```bash
./fileinfo "; id"
```

Look past the `ls` output: you'll also see the result of **`id`** — a command
*you* were never meant to be able to run. The `;` ended the `ls` command and
started yours. Now try a few more:

```bash
./fileinfo "; cat /etc/passwd"      # read a system file
./fileinfo "nothing && whoami"      # chain with &&
./fileinfo '`id`'                   # backticks = "run this" (as in ShellShock)
```

Each one runs a command of your choosing. That's the whole vulnerability: the
program handed your text to a shell, and the shell did what you said.

> Make a secret and steal it — the "flag" moment:
> ```bash
> echo "SECRET-KEY-42" > secret.txt && chmod 600 secret.txt
> ./fileinfo "; cat secret.txt"
> ```

---

## 4. Why it's dangerous — the OS security model

Run this one and read the output carefully:

```bash
./fileinfo "; id"
```

Whatever `id` prints is **who your injected command runs as** — because it
inherits the privileges of the program that launched it. This is the key idea
from the lecture's *confining applications* section:

- If `id` shows **`uid=0(root)`**, your injected command is running as root — it
  can read, change or destroy almost anything on the system. If this tool were a
  web service running as root (a common misconfiguration), an attacker now owns
  the machine. **This is exactly why the lecture says servers should run as
  low-privilege users.**
- If `id` shows a normal user, the damage is **confined** to what that user can
  touch. The operating system's **file permissions** are the fence: a file the
  user can't read, your injected `cat` can't read either.

Quick experiment (if you're *not* root):
```bash
chmod 000 secret.txt         # remove all permissions
./fileinfo "; cat secret.txt"   # now the injected cat is denied
chmod 600 secret.txt
```
The injection still *runs* — but the OS stops it from reading the file. The bug
let the attacker in; least privilege decided how far they got. (If you *are*
root, this won't stop you — root bypasses permissions, which is the whole reason
running as root is dangerous.)

---

## 5. Fix #1 — don't involve the shell

The real fix is to never build a shell command out of user input. Run the
program directly and pass the input as a plain argument, so no shell is there to
interpret `;` or backticks. Create `fileinfo_safe.c`:

```c
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2) { printf("Usage: %s <filename>\n", argv[0]); return 1; }
    char *args[] = { "ls", "-l", argv[1], NULL };   // one explicit argument
    execvp("ls", args);                             // no shell involved
    perror("execvp");
    return 1;
}
```
```bash
gcc -o fileinfo_safe fileinfo_safe.c
./fileinfo_safe "; id"
```
Now `; id` is handed to `ls` as a *filename* — `ls: cannot access '; id'`. The
injection is inert, because nothing ever parsed it as a command.

(A second-best fix is to **validate** the input — e.g. reject anything that
isn't a plain filename — but avoiding the shell entirely is the robust choice.)

---

## 6. Fix #2 — least privilege (defence in depth)

Even good code has bugs. The OS is your backstop: run programs, and especially
network services, as a **low-privilege user** that can touch only what it needs.
Then a vulnerability you missed is confined instead of catastrophic. Fixing the
code (Fix #1) and confining the process (Fix #2) are two different layers — you
want both.

---

## 7. Submission

Submit a short write-up (half to one page) with:

1. A screenshot of `./fileinfo "; id"` running your injected command.
2. One or two sentences: what character made the injection possible, and why the
   shell acted on it.
3. The output of `; id` on your setup — are you root or a normal user? One
   sentence on what that means for how much damage the injection could do.
4. Your `fileinfo_safe.c` and confirmation that `"; id"` no longer injects.
5. One sentence explaining why "run as a low-privilege user" is still worth doing
   even after you've fixed the code.
