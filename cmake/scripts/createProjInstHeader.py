#!/usr/bin/env python

import os
import re
from optparse import OptionParser


def main():
    parser = OptionParser(
        usage="ERROR: Usage %prog <project> <location> <outputfile>")
    parser.add_option("-q", "--quiet", action="store_true",
                      help="Do not print messages.")
    opts, args = parser.parse_args()

    if len(args) != 3:
        parser.error("wrong number of arguments")

    project, location, outputfile = args
    if not opts.quiet:
        print("Creating %s for %s with %s install location" % (outputfile, project, location))

    outdir = os.path.dirname(outputfile)
    if not os.path.exists(outdir):
        if not opts.quiet:
            print("Creating directory", outdir)
        os.makedirs(outdir)

    # Prepare data to be written
    outputdata = """#ifndef %(proj)s_INSTALL_H
/* Automatically generated file: do not modify! */
#define %(proj)s_INSTALL_LOCATION %(location)s
#endif
""" % { 'proj': project.upper(), 'location': location }

    # Get the current content of the destination file (if any)
    try:
        f = open(outputfile, "r")
        olddata = f.read()
        f.close()
    except IOError:
        olddata = None

    # Overwrite the file only if there are changes
    if outputdata != olddata:
        open(outputfile, "w").write(outputdata)

if __name__ == "__main__":
    main()
