#!/bin/bash

set -ue

LIBS="bufio.c lib.c alloc.c syscall.c"
SOURCES="cc1.c type.c parse1.c lex1.c as.c ${LIBS}"
GENLEX_SOURCES="genlex.c ${LIBS}"

if [ -e cc1 ]; then
	./cc1 ${SOURCES} -o cc0
else
	gcc -Wall -Wextra -Wno-unused -pedantic -std=c99 ./cc0.c -o cc0 || { rm -f cc0; echo bootstrap failed; exit 1; }
fi

./cc0 ${SOURCES} -o cc1 || { rm -f cc1; echo cc0 failed; exit 1; }

./cc1 ${SOURCES} -o cc2 || { rm -f cc2; echo cc1 failed; exit 1; }

cmp cc1 cc2 || { echo output mismatch; exit 1; }

./cc1 ${GENLEX_SOURCES} -o genlex || { rm -f genlex; echo cc1 failed; exit 1; }
./genlex < cc3.l > lex3.c || { rm -f lex3.c; echo genlex failed; exit 1; }

./cc1 ${LIBS} echo.c -o echo || { rm -f echo; echo echo failed; exit 1; }
./cc1 ${LIBS} cmp.c -o cmp || { rm -f cmp; echo cmp failed; exit 1; }
./cc1 ${LIBS} rm.c -o rm || { rm -f rm; echo rm failed; exit 1; }
./cc1 ${LIBS} ls.c -o ls || { rm -f ls; echo ls failed; exit 1; }
./cc1 ${LIBS} cpio.c -o cpio || { rm -f cpio; echo cpio failed; exit 1; }
./cc1 ${LIBS} sh.c -o sh || { rm -f sh; echo sh failed; exit 1; }
./cc1 ${LIBS} sshd.c -o sshd || { rm -f sshd; echo sshd failed; exit 1; }
./cc1 ${LIBS} vi.c -o vi || { rm -f vi; echo vi failed; exit 1; }

{
	echo echo
	echo cmp
	echo rm
	echo ls
	echo cpio
	echo sh
	echo sshd
	echo vi
	echo cc1
} | ./cpio > initramfs || { rm -f initramfs; echo initramfs failed; exit 1; }

./cc1 kernel.c -o kernel
