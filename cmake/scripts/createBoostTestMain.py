""" script module for the creation of the c++ boost test main function """

import os
from optparse import OptionParser


def main():
    """ main function of this script module """
    parser = OptionParser(
        usage="ERROR: Usage %prog <package> <outputfile>")
    parser.add_option("-q", "--quiet", action="store_true",
                      help="Do not print messages.")
    opts, args = parser.parse_args()

    if len(args) != 2:
        parser.error("wrong number of arguments")

    package, outputfile = args
    if not opts.quiet:
        print("Creating %s for %s", outputfile, package)

    outdir = os.path.dirname(outputfile)
    if not os.path.exists(outdir):
        if not opts.quiet:
            print("Creating directory", outdir)
        os.makedirs(outdir)

    # Prepare data to be written
    outputdata = """ /* Automatically generated file: do not modify! */
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE %(pack)s Test Suite
#include <boost/test/unit_test.hpp>
""" % { 'pack': package }

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
