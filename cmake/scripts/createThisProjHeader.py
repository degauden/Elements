import os
import re
from optparse import OptionParser


def main():
    parser = OptionParser(
        usage="ERROR: Usage %prog <project> <outputfile>")
    parser.add_option("-q", "--quiet", action="store_true",
                      help="Do not print messages.")
    parser.set_defaults(install_prefix="")
    parser.add_option("-i", "--install-prefix", action="store",
                     help="set the CMAKE_INSTALL_PREFIX")
    opts, args = parser.parse_args()

    if len(args) != 2:
        parser.error("wrong number of arguments")

    project, outputfile = args
    if not opts.quiet:
        print("Creating %s for %s %s" % (outputfile, project, version))

    outdir = os.path.dirname(outputfile)
    if not os.path.exists(outdir):
        if not opts.quiet:
            print("Creating directory", outdir)
        os.makedirs(outdir)

    # Prepare data to be written
    outputdata = """#ifndef _THIS_PROJECT_H_
#define _THIS_PROJECT_H_
/* Automatically generated file: do not modify! */
#include <cstdint>                  // for uing_least64_t
#include <string>                   // for string
#include <vector>                   // for vector
#include "%(proj)s_VERSION.h"
#include "%(proj)s_INSTALL.h"
const std::string THIS_PROJECT_ORIGINAL_VERSION = %(proj)s_ORIGINAL_VERSION;
constexpr std::uint_least64_t THIS_PROJECT_MAJOR_VERSION = %(proj)s_MAJOR_VERSION;
constexpr std::uint_least64_t THIS_PROJECT_MINOR_VERSION = %(proj)s_MINOR_VERSION;
constexpr std::uint_least64_t THIS_PROJECT_PATCH_VERSION = %(proj)s_PATCH_VERSION;
constexpr std::uint_least64_t THIS_PROJECT_VERSION = %(proj)s_VERSION;
const std::string THIS_PROJECT_VERSION_STRING {%(proj)s_VERSION_STRING};
const std::string THIS_PROJECT_NAME_STRING {"%(Proj)s"};
const std::string THIS_PROJECT_INSTALL_LOCATION_STRING {%(proj)s_INSTALL_LOCATION_STRING};
const std::vector<std::string> THIS_PROJECT_SEARCH_DIRS {%(proj)s_SEARCH_DIRS};
const std::string CMAKE_INSTALL_PREFIX_STRING {"%(prefix)s"};
#endif
""" % { 'proj': project.upper(), 'Proj': project, 'prefix': opts.install_prefix}

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
