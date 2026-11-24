# Build the vulnerable tool.
#   make         compile
#   make clean   remove the binary and lab scratch files
#
# -w hides the compiler's warning about our deliberately unsafe use of system().
CC     = gcc
CFLAGS = -O0 -w

fileinfo: fileinfo.c
	$(CC) $(CFLAGS) -o fileinfo fileinfo.c

clean:
	rm -f fileinfo fileinfo_safe secret.txt

.PHONY: clean
