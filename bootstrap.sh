#!/bin/sh

LIBS="bufio.c lib.c alloc.c syscall.c"
PEG="peg.c peglib.c parsepeg.c"
SOURCES="cc1.c type.c parse2.c parse3.c peglib.c as.c decl.c node.c cout.c"

# Build the bootstrap compiler from c
gcc -std=c99 ./cc0.c -o cc0

# Bootstrap the parser tool
./cc0 ${LIBS} ${PEG} -o peg0

# Bootstrap the self hosted compiler
./peg0 cc3.peg -o parse3.c
./cc0 ${LIBS} ${SOURCES} -o cc1

# Double check the bootstrap and self hosting compiler have the same output
./cc1 ${LIBS} ${SOURCES} -o cc2
cmp cc1 cc2 || echo cc mismatch

# Also check the peg tool compiles the same
./cc1 ${LIBS} ${PEG} -o peg
cmp peg0 peg || echo peg mismatch

# Verify that the saved bootstrap parsepeg.c came from peg.peg
./peg -P PEG_ peg.peg -o parsepeg2.c
cmp parsepeg.c parsepeg2.c || echo parsepeg mismatch

./cc1 -C ${LIBS} ${SOURCES} -o cc2.c
cmp cc0.c cc2.c || echo bootstrap mismatch
