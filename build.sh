#!/bin/sh

LIBS="bufio.c lib.c alloc.c syscall.c"
CRYPTO="ed25519.c sha512.c sha256.c chacha20.c poly1305.c"
CC="cc1.c type.c parse1.c lex1.c as.c decl.c node.c"
PEG="peg.c peglib.c parsepeg.c"
BOOT="pxe.asm"
SSHD="chacha20.c poly1305.c sha256.c sha512.c ed25519.c sshd.c"
KERNEL="kernel.c"
SHELL="echo.c cmp.c rm.c ls.c cat.c xxd.c mv.c mkdir.c cpio.c sh.c"
BIN="echo cmp rm ls cat xxd mv mkdir cpio sh sshd init cc1 cc2 build.sh peg.peg cc3.peg"
ALL="${LIBS} ${CC} ${PEG} ${BOOT} ${SSHD} ${KERNEL} ${SHELL} ${BIN}"

./cc1 ${LIBS} echo.c -o echo
./cc1 ${LIBS} cmp.c -o cmp
./cc1 ${LIBS} rm.c -o rm
./cc1 ${LIBS} mv.c -o mv
./cc1 ${LIBS} mkdir.c -o mkdir
./cc1 ${LIBS} ls.c -o ls
./cc1 ${LIBS} cat.c -o cat
./cc1 ${LIBS} xxd.c -o xxd
./cc1 ${LIBS} cpio.c -o cpio
./cc1 ${LIBS} sh.c -o sh
./cc1 ${LIBS} ${CRYPTO} sshd.c -o sshd

for name in ${ALL}; do echo ${name}; done | ./cpio -o > initramfs

./cc1 kernel.c -o kernel
