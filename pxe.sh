#!/bin/sh
./bootstrap.sh || exit 1
nasm -f bin pxe.asm || exit 1
cp kernel pxe /srv/tftp/ || exit 1
printf r > /dev/ttyACM0 || exit 1
