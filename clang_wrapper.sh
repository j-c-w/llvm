#!/bin/bash

# Try running idl clang, then fallback to normal clang.

execdir=$(dirname $0)

idl_clang="$execdir/idlclang"

exec $idl_clang "$@" || (exec $idl_clang "$@" --fno-idl)
