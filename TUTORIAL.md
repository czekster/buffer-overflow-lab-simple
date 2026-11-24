# Lab: Buffer Overflow — the 5-minute version

**CS3SP — Secure Programming**

In this short lab you'll see *why* buffer overflows are dangerous, using nothing
but a C compiler. You'll break into a program by typing a password that's too
long — no debugger, no hacking tools.

**Time:** ~20 minutes. **Difficulty:** gentle.

---

## 1. The program

Open `login.c`. It's a toy login:

```c
struct {
    char password[8];     // room for 8 characters
    int  authorised;      // 0 = locked, non-zero = unlocked
} user;

user.authorised = 0;
scanf("%s", user.password);                 // read what the user types
if (strcmp(user.password, "s3cr3t") == 0)
    user.authorised = 1;                    // correct password unlocks

if (user.authorised) /* ACCESS GRANTED */ else /* Access denied */
```

The only intended way in is to type the password `s3cr3t`.

Two facts to keep in mind:
- `password` holds **8 characters**.
- `password` and `authorised` sit **next to each other** in memory, in that
  order.

---

## 2. Build and run it normally

```bash
make
./login
```

Try it a few times:

| You type | Result | Why |
|---|---|---|
| `hello` | Access denied | wrong password |
| `s3cr3t` | ACCESS GRANTED | correct password |

So far, so normal.

---

## 3. Break in without the password

Now type **nine or more** characters — anything, as long as it's longer than
the buffer:

```bash
./login
Enter the password: AAAAAAAAA
ACCESS GRANTED
```

You got in, and you never typed `s3cr3t`. **Why?**

`scanf("%s", ...)` doesn't check length. You gave it 9 characters, but
`password` only has room for 8. The 9th character didn't vanish — it was written
into the memory **right after** `password`, which is exactly where `authorised`
lives. Writing a non-zero character there flips `authorised` from 0 to non-zero,
and the final check reads it as "unlocked".

```
  memory, low -> high
  +--------------------------+------------+
  |      password[8]         | authorised |
  +--------------------------+------------+
   ^ you typed 8 chars here   ^ the 9th (and beyond) spill into here
```

That's a **buffer overflow**: writing past the end of a buffer corrupts
whatever is next to it. Here it corrupted a security decision.

---

## 4. Push it further (optional)

Type something really long:

```bash
python3 -c "print('A'*200)" | ./login
```

Now it doesn't just mis-behave — it **crashes** with a *segmentation fault*.
You overwrote so much memory that you destroyed data the program depends on to
keep running (including the hidden "where do I return to" bookkeeping). A
segfault is Linux stopping a program that touched memory it shouldn't. In real
software, that same corruption is the door attackers use to take control.

---

## 5. How to fix it

The bug is the unbounded read. Limit it to the buffer size:

```c
scanf("%7s", user.password);   // read at most 7 chars (+ the terminating 0)
```

or use a function that takes a size, like `fgets(user.password,
sizeof(user.password), stdin)`.

Recompile and try the 9-character attack again — it no longer overflows, and
`authorised` stays locked. The rule: **never read input without bounding it to
the size of the buffer that holds it.**

---

*This code is deliberately insecure and for learning only.*
