#!/bin/sh
clear
./pxe.sh
echo status $?
: < ~/.post
exec "$0"
