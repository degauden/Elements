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
    outputdata = """/* Automatically generated file: do not modify! */
/**
 * @file BoostTestMain.cpp
 *
 * @date Feb 8, 2017
 * @author Hubert Degaudenzi
 *
 * @copyright 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 *
 */
#pragma GCC visibility push(default)
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE %(pack)s Test Suite
#include <boost/test/unit_test.hpp>
#pragma GCC visibility pop
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
