#!/bin/bash

set -ue

uptodate() {
	target=$1
	shift
	if ! [ -f "${target}" ]; then
		return 1
	fi
	while [ $# -gt 0 ]; do
		if ! [ -f "$1" ] || [ "$1" -nt "${target}" ]; then
			return 1
		fi
		shift
	done
	return 0
}

# Environmental variables used to compile the bootstrap
: ${CC:=gcc}
: ${CFLAGS:=-Wall -Wextra -Wno-unused -pedantic -std=c99}
LIBS="bufio.c lib.c alloc.c syscall.c"
SOURCES="cc1.c type.c parse1.c lex1.c as.c ${LIBS}"
GENLEX_SOURCES="genlex.c ${LIBS}"

# First compile the bootstrap
uptodate cc0 cc0.c || ${CC} ${CFLAGS} ./cc0.c -o cc0 || { rm -f cc0; echo "bootstrap failed" >&2; exit 1; }

# Then use the bootstrap to compile the compiler
uptodate cc1 ${SOURCES} || ./cc0 ${SOURCES} -o cc1 || { rm -f cc1; echo "cc0 failed" >&2; exit 1; }

# Then compile the compiler with itself
uptodate cc2 cc1 ${SOURCES} && false || ./cc1 ${SOURCES} -o cc2 || { rm -f cc2; echo "cc1 failed" >&2; exit 1; }

# And check our work
diff <(xxd cc1) <(xxd cc2) || :
cmp cc1 cc2 || { echo "output mismatch"; exit 1; }

uptodate genlex cc1 ${GENLEX_SOURCES} || ./cc1 ${GENLEX_SOURCES} -o genlex || { rm -f genlex; echo "cc1 failed" >&2; exit 1; }
uptodate lex3.c genlex cc3.l || ./genlex < cc3.l > lex3.c || { rm -f lex3.c; echo "genlex failed" >&2; exit 1; }
