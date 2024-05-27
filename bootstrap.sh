#!/bin/sh

LIBS="bufio.c lib.c alloc.c syscall.c"
SOURCES="cc1.c type.c parse1.c lex1.c as.c"

gcc -Wall -Wextra -Wno-unused -pedantic -std=c99 ./cc0.c -o cc0

./cc0 ${LIBS} ${SOURCES} -o cc1

./cc1 ${LIBS} ${SOURCES} -o cc2

cmp cc1 cc2 || echo mismatch

sh ./build.sh
