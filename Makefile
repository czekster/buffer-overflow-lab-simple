# Build the vulnerable program.
#   make         compile
#   make clean   remove the binary
#
# -fno-stack-protector : turn off the stack canary, so the raw overflow shows
#                        through cleanly instead of being caught.
# -O0                  : no optimisation -- keeps the memory layout simple.
# -w                   : hide the compiler's warning about our deliberate bug.
CC     = gcc
CFLAGS = -fno-stack-protector -O0 -w

login: login.c
	$(CC) $(CFLAGS) -o login login.c

clean:
	rm -f login

.PHONY: clean
