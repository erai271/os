#!/bin/sh

BOOTSTRAP="cc0.c"
LIBS="bufio.c lib.c alloc.c syscall.c"
SOURCES="cc1.c type.c parse2.c parse3.c peglib.c as.c decl.c node.c cout.c peg.c parsepeg.c"

# Build the bootstrap compiler from c
gcc -std=c99 ${BOOTSTRAP} -o cc0
./cc0 -P P_ cc3.peg -o parse3.c
./cc0 -P PEG_ peg.peg -o parsepeg.c
./cc0 ${LIBS} ${SOURCES} -o cc1

# Double check the bootstrap and self hosting compiler have the same output
./cc1 ${LIBS} ${SOURCES} -o cc2
cmp cc1 cc2 || echo cc mismatch

# Verify the saved bootstrap matches
./cc1 -C ${LIBS} ${SOURCES} -o cc2.c
cmp cc0.c cc2.c || echo bootstrap mismatch
