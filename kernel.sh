#!/bin/bash
truncate --size 4096 disk
./bootstrap.sh && ./cc2 kernel.c -o kernel && qemu-system-x86_64 -kernel kernel -M q35 -m 1g -nographic  -monitor stdio -serial chardev:sock -chardev socket,id=sock,path=/tmp/s -drive file=disk,if=none,id=disk,format=raw -device nvme,serial=12345678,drive=disk -device e1000,netdev=net -netdev tap,id=net,ifname=tap0,script=no,downscript=no
