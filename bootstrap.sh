#!/bin/sh

LIBS="bufio.c lib.c alloc.c syscall.c"
SOURCES="cc1.c type.c parse1.c lex1.c as.c $LIBS"
GENLEX_SOURCES="genlex.c $LIBS"

./cc1 $SOURCES -o cc0 || gcc -Wall -Wextra -Wno-unused -pedantic -std=c99 ./cc0.c -o cc0 || exit 1

./cc0 $SOURCES -o cc1 || exit 1

./cc1 $SOURCES -o cc2 || exit 1

cmp cc1 cc2 || echo mismatch

./cc1 $GENLEX_SOURCES -o genlex || exit 1
./genlex < cc3.l > lex3.c || exit 1

./cc1 $LIBS echo.c -o echo || exit 1
./cc1 $LIBS cmp.c -o cmp || exit 1
./cc1 $LIBS rm.c -o rm || exit 1
./cc1 $LIBS ls.c -o ls || exit 1
./cc1 $LIBS cpio.c -o cpio || exit 1
./cc1 $LIBS sh.c -o sh || exit 1
./cc1 $LIBS sshd.c -o sshd || exit 1
./cc1 $LIBS vi.c -o vi || exit 1

{
	echo pxe.asm
	echo chacha20.c
	echo poly1305.c
	echo sha256.c
	echo sha512.c
	echo ed25519.c
	echo bootstrap.sh
	echo bufio.c
	echo lib.c
	echo alloc.c
	echo syscall.c
	echo cc1.c
	echo type.c
	echo parse1.c
	echo lex1.c
	echo as.c
	echo genlex.c
	echo kernel.c
	echo echo.c
	echo echo
	echo cmp.c
	echo cmp
	echo rm.c
	echo rm
	echo ls.c
	echo ls
	echo cpio.c
	echo cpio
	echo sh.c
	echo sh
	echo vi.c
	echo vi
	echo sshd.c
	echo sshd
	echo init
	echo cc1
} | ./cpio -o > initramfs || exit 1

./cc1 kernel.c -o kernel || exit 1
