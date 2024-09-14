#!/bin/sh
clear
sh -e ./build.sh
echo status $?
: < ~/.post
exec "$0"
