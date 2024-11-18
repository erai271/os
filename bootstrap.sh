#!/bin/sh

BOOTSTRAP="cc0.c"
LIBS="bufio.om lib.om alloc.om syscall.om"
SOURCES="cc1.om type.om parse2.om parse3.om peglib.om as.om decl.om node.om cout.om peg.om parsepeg.om"

# Build the bootstrap compiler from c
gcc -std=c99 ${BOOTSTRAP} -o cc0
./cc0 -P P_ cc3.peg -o parse3.om
./cc0 -P PEG_ peg.peg -o parsepeg.om
./cc0 ${LIBS} ${SOURCES} -o cc1

# Double check the bootstrap and self hosting compiler have the same output
./cc1 ${LIBS} ${SOURCES} -o cc2
cmp cc1 cc2 || echo cc mismatch

# Verify the saved bootstrap matches
./cc1 -C ${LIBS} ${SOURCES} -o cc2.c
cmp cc0.c cc2.c || echo bootstrap mismatch
