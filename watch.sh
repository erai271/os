#!/bin/sh
clear
time sh -e ./build.sh
echo status $?
: < ~/.post
exec "$0"
