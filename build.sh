#!/bin/sh

LIBS="peglib.om bufio.om lib.om alloc.om syscall.om"
CRYPTO="ed25519.om sha512.om sha256.om chacha20.om poly1305.om"
CC="cc1.om type.om as.om decl.om node.om"
PEG="peg.om parsepeg.om"
SSHD="chacha20.om poly1305.om sha256.om sha512.om ed25519.om sshd.om"
KERNEL="kernel.om"
SHELL="echo.om cmp.om rm.om ls.om cat.om xxd.om mv.om mkdir.om cpio.om sh.om"
BIN="echo cmp rm ls cat xxd mv mkdir cpio sh sshd init cc1 build.sh peg.peg cc3.peg"
ALL="${LIBS} ${CC} ${PEG} ${SSHD} ${KERNEL} ${SHELL} ${BIN}"

./cc1 ${LIBS} echo.om -o echo
./cc1 ${LIBS} cmp.om -o cmp
./cc1 ${LIBS} rm.om -o rm
./cc1 ${LIBS} mv.om -o mv
./cc1 ${LIBS} mkdir.om -o mkdir
./cc1 ${LIBS} ls.om -o ls
./cc1 ${LIBS} cat.om -o cat
./cc1 ${LIBS} xxd.om -o xxd
./cc1 ${LIBS} cpio.om -o cpio
./cc1 ${LIBS} sh.om -o sh
./cc1 ${LIBS} ${CRYPTO} sshd.om -o sshd

for name in ${ALL}; do echo ${name}; done | ./cpio -o > initramfs

./cc1 kernel.om -o kernel
