#!/bin/bash
clear
cmd="${1:-./pxe.sh}"
shift
"${cmd}" "$@"
date
: < ~/.post
exec "$0" "$@"
