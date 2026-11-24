/*
 * fileinfo.c -- a tiny "show details of a file on the server" admin tool.
 *
 * CS3SP Secure Programming -- teaching material. INTENTIONALLY VULNERABLE to
 * OS command injection (CWE-78). Never build real tools like this.
 *
 * The idea: lots of real programs get a job done by asking the operating
 * system shell to run a command for them. Here, to show a file's details, the
 * program builds the text "ls -l <whatever the user typed>" and hands it to
 * the shell via system().
 *
 * The bug: the user's input is pasted straight into that shell command. The
 * shell treats characters like ;  &&  |  and backticks as INSTRUCTIONS, not
 * as part of a filename -- so a user can tack on commands of their own.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char cmd[256];

    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    /* THE BUG: user input pasted straight into a shell command line. */
    snprintf(cmd, sizeof(cmd), "ls -l %s", argv[1]);

    printf("Running: %s\n", cmd);
    system(cmd);                 /* hands the whole string to /bin/sh */

    return 0;
}
