import argparse
import sys
import os

visited_files = []

def preprocess(file, dir=None):
    global visited_files
    # Don't include a file more than once.
    if dir and file:
        file = dir + os.path.sep + file
    if file in visited_files:
        return
    visited_files.append(file)

    f = None
    try:
        if file is None:
            f = sys.stdin
        else:
            f = open(file)

        # Preprocess everything else also:
        for line in f.readlines():
            if line.startswith('Include '):
                include_file = line.split(' ')[1].strip()
                preprocess(include_file, dir=dir)
            else:
              print(line)
    finally:
        if file is not None:
            if f:
                f.close()


if __name__ == "__main__":
    parser = argparse.ArgumentParser("Preprocessor for IDL")

    parser.add_argument("filename", type=argparse.FileType('r'), nargs='?', default=None)
    parser.add_argument("-d", dest='dir', help="Include directory to find files in", default=None)
    args = parser.parse_args()

    preprocess(args.filename, args.dir)
