"""
Simple script to remove lines from a file.
"""
import os
import sys
import re


def main():
    if len(sys.argv) != 3:
        print("Usage: %s <filename> <>" + os.linesep + \
              "\tRemoves from <filename> the lines matching <reg.exp.>",
              os.path.basename(sys.argv[0]))

        sys.exit(1)

    filename, pattern = sys.argv[1:]
    if not os.path.isfile(filename):
        print("Error: cannot find file '%s'", filename)
        sys.exit(1)

    try:
        regexp = re.compile(pattern)
    except re.error as v:
        print("Error: invalid regular expression %r (%s)", pattern, v)
        sys.exit(1)

    # read the file in memory skipping the matched lines
    lines = [l
             for l in open(filename)]
    orig_size = len(lines)
    lines = [l
             for l in lines
             if not regexp.search(l)]
    final_size = len(lines)
    # rename the original to make a backup copy
    if os.path.exists(filename + "~"):
        os.remove(filename + "~")
    os.rename(filename, filename + "~")
    # write out the file
    open(filename, "w").writelines(lines)
    print("Removed %d lines out of %d in file %s, matching pattern %r",
          orig_size - final_size, orig_size, filename, pattern)

if __name__ == '__main__':
    main()
