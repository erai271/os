#!/sh

LIBS="bufio.c lib.c alloc.c syscall.c"
CC="cc1.c type.c parse1.c lex1.c as.c"
GENLEX="genlex.c"
BOOT="pxe.asm"
SSHD="chacha20.c poly1305.c sha256.c sha512.c ed25519.c sshd.c"
KERNEL="kernel.c"
SHELL="echo.c cmp.c rm.c ls.c mv.c mkdir.c cpio.c vi.c sh.c"
BIN="echo cmp rm ls mv mkdir cpio sh vi sshd init cc1 cc2"
ALL="${LIBS} ${CC} ${GENLEX} ${BOOT} ${SSHD} ${KERNEL} ${SHELL} ${BIN}"

./cc1 ${LIBS} ${CC} -o cc2

./cc2 ${LIBS} ${GENLEX} -o genlex
./genlex < cc3.l > lex3.c

./cc2 ${LIBS} echo.c -o echo
./cc2 ${LIBS} cmp.c -o cmp
./cc2 ${LIBS} rm.c -o rm
./cc2 ${LIBS} mv.c -o mv
./cc2 ${LIBS} mkdir.c -o mkdir
./cc2 ${LIBS} ls.c -o ls
./cc2 ${LIBS} cpio.c -o cpio
./cc2 ${LIBS} sh.c -o sh
./cc2 ${LIBS} sshd.c -o sshd
./cc2 ${LIBS} vi.c -o vi

for name in ${ALL}; do echo ${name}; done | ./cpio -o > initramfs

./cc2 kernel.c -o kernel
