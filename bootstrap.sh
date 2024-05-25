#!/bin/bash

set -ue

# Environmental variables used to compile the bootstrap
: ${CC:=gcc}
: ${CFLAGS:=-Wall -Wextra -Wno-unused -pedantic -std=c99}
LIBS="bufio.c lib.c alloc.c syscall.c"
SOURCES="cc1.c type.c parse1.c lex1.c as.c ${LIBS}"
GENLEX_SOURCES="genlex.c ${LIBS}"

# First compile the bootstrap
${CC} ${CFLAGS} ./cc0.c -o cc0 || { rm -f cc0; echo "bootstrap failed" >&2; exit 1; }

# Then use the bootstrap to compile the compiler
./cc0 ${SOURCES} -o cc1 || { rm -f cc1; echo "cc0 failed" >&2; exit 1; }

# Then compile the compiler with itself
./cc1 ${SOURCES} -o cc2 || { rm -f cc2; echo "cc1 failed" >&2; exit 1; }

# And check our work
cmp cc1 cc2 || { echo "output mismatch"; exit 1; }

./cc1 ${GENLEX_SOURCES} -o genlex || { rm -f genlex; echo "cc1 failed" >&2; exit 1; }
./genlex < cc3.l > lex3.c || { rm -f lex3.c; echo "genlex failed" >&2; exit 1; }
