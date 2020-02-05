#!/bin/bash

if [[ $# -ne 1 ]]; then
  echo "Usage: $0 <clang location>"
  exit 1
fi

for file in ./*; do
  clang -x ir $file
done
