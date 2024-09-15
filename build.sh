#!/sh

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

./cc1 ${LIBS} ${CC} -o cc2

./cc1 ${LIBS} ${PEG} -o peg
./peg -o parsepeg2.c peg.peg
cmp parsepeg.c parsepeg2.c || echo peg mismatch

./peg -o parse3.c cc3.peg
./cc2 -o cc3 bufio.c lib.c alloc.c syscall.c peglib.c parse2.c parse3.c as.c type.c decl.c node.c cc1.c

./cc3 ${LIBS} echo.c -o echo
./cc3 ${LIBS} cmp.c -o cmp
./cc3 ${LIBS} rm.c -o rm
./cc3 ${LIBS} mv.c -o mv
./cc3 ${LIBS} mkdir.c -o mkdir
./cc3 ${LIBS} ls.c -o ls
./cc3 ${LIBS} cat.c -o cat
./cc3 ${LIBS} xxd.c -o xxd
./cc3 ${LIBS} cpio.c -o cpio
./cc3 ${LIBS} sh.c -o sh
./cc3 ${LIBS} ${CRYPTO} sshd.c -o sshd

for name in ${ALL}; do echo ${name}; done | ./cpio -o > initramfs

./cc3 kernel.c -o kernel
