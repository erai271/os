#!/bin/bash
exec 3<>/dev/ttyACM0
./bootstrap.sh && ./cc2 kernel.c -o kernel && nasm -f bin pxe.asm && cp kernel pxe /srv/tftp/ && printf r >&3
