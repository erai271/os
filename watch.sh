#!/bin/sh
clear
timeout 1 time sh -e ./build.sh
echo status $?
: < ~/.post
exec "$0"
