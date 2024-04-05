#!/bin/bash
# nc -l -k -j -U /tmp/s
./bootstrap.sh && ./cc2 kernel.c -o kernel && qemu-system-x86_64 -kernel kernel -M q35 -m 1g -nographic  -monitor stdio -serial chardev:sock -chardev socket,id=sock,path=/tmp/s
