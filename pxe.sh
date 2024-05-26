#!/bin/bash

set -ue

exec 3<>/dev/ttyACM0

./bootstrap.sh

nasm -f bin pxe.asm

cp kernel pxe /srv/tftp/

printf r >&3
