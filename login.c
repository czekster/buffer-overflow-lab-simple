/*
 * login.c -- a tiny, deliberately insecure "login".
 *
 * CS3SP Secure Programming -- teaching material. This code is INTENTIONALLY
 * broken to demonstrate a buffer overflow. Never write software like this.
 *
 * The idea:
 *   - There is a secret password ("s3cr3t").
 *   - There is a flag, 'authorised', that is 0 (locked) until you prove the
 *     password.
 *   - 'password' and 'authorised' sit next to each other in memory.
 *
 * The bug: scanf("%s", ...) does NOT limit how much it reads. If you type more
 * characters than 'password' can hold, the extra characters run off the end of
 * the buffer and overwrite 'authorised' -- flipping it from 0 to non-zero and
 * granting access WITHOUT the real password.
 */
#include <stdio.h>
#include <string.h>

int main(void)
{
    /* Both fields are in a struct so that 'authorised' is guaranteed to sit
       immediately AFTER 'password' in memory -- that is what makes the
       overflow land on it reliably. */
    struct {
        char password[8];
        int  authorised;
    } user;

    user.authorised = 0;                 /* locked by default */

    printf("Enter the password: ");
    scanf("%s", user.password);          /* <-- THE BUG: no length limit */

    if (strcmp(user.password, "s3cr3t") == 0)
        user.authorised = 1;             /* the only legitimate way in */

    if (user.authorised != 0)
        printf("ACCESS GRANTED\n");
    else
        printf("Access denied.\n");

    return 0;
}
