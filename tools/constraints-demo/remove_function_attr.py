#!/usr/bin/python
import sys

blacklist = ["noinline"]

for line in sys.stdin:
    filter_line = " ".join([word for word in line.split(' ') if word not in blacklist])
    sys.stdout.write(filter_line)
