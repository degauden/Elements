import os
from optparse import OptionParser


def main():
    parser = OptionParser(
        usage="ERROR: Usage %prog <module> <outputfile>")
    parser.add_option("-q", "--quiet", action="store_true",
                      help="Do not print messages.")
    opts, args = parser.parse_args()

    if len(args) != 2:
        parser.error("wrong number of arguments")

    module, outputfile = args
    if not opts.quiet:
        print("Creating %s for %s" % (outputfile, module))

    outdir = os.path.dirname(outputfile)
    if not os.path.exists(outdir):
        if not opts.quiet:
            print("Creating directory", outdir)
        os.makedirs(outdir)

    # Prepare data to be written
    outputdata = """#ifndef _THIS_MODULE_H_
#define _THIS_MODULE_H_
/* Automatically generated file: do not modify! */
#include <cstdint>                  // for uing_least64_t
#include <string>                   // for string
#include <vector>                   // for vector
#include "%(Mod)sVersion.h"
constexpr std::uint_least64_t THIS_MODULE_MAJOR_VERSION = %(mod)s_MAJOR_VERSION;
constexpr std::uint_least64_t THIS_MODULE_MINOR_VERSION = %(mod)s_MINOR_VERSION;
constexpr std::uint_least64_t THIS_MODULE_PATCH_VERSION = %(mod)s_PATCH_VERSION;
constexpr std::uint_least64_t THIS_MODULE_VERSION = %(mod)s_VERSION;
const std::string THIS_MODULE_VERSION_STRING {%(mod)s_VERSION_STRING};
const std::string THIS_MODULE_NAME_STRING {"%(Mod)s"};
#endif
""" % {'mod': module.upper(), 'Mod': module}

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
