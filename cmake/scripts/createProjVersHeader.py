import os
import re
from optparse import OptionParser

lhcb_ver_style = "v(?P<maj_ver>[0-9]+)r(?P<min_ver>[0-9]+)(?:p(?P<pat_ver>[0-9]+))?"
atlas_ver_style = "[A-Za-z]+\-(?P<maj_ver>[0-9]+)\-(?P<min_ver>[0-9]+)(?:\-(?P<pat_ver>[0-9]+))?"
plain_ver_style = "(?P<maj_ver>[0-9]+)\.(?P<min_ver>[0-9]+)(?:\.(?P<pat_ver>[0-9]+))?"


def main():
    parser = OptionParser(
        usage="ERROR: Usage %prog <project> <version> <vcs_version> <outputfile>")
    parser.add_option("-q", "--quiet", action="store_true",
                      help="Do not print messages.")
    opts, args = parser.parse_args()

    if len(args) != 4:
        parser.error("wrong number of arguments: %d: %s" % (len(args), " ".join(args)))

    project, version, vcs_version, outputfile = args
    if not opts.quiet:
        print("Creating %s for %s %s (VCS: %s)" % (outputfile, project, version, vcs_version))

    if version.startswith('HEAD'):
        # special handling
        majver, minver, patver = 999, 999, 0
    else:
        for style in [lhcb_ver_style, atlas_ver_style, plain_ver_style]:
            m = re.match(style, version)
            if m:
                break
        majver = int(m.groupdict()['maj_ver'])
        minver = int(m.groupdict()['min_ver'])
        patver = int(m.groupdict()['pat_ver'] or 0)

    outdir = os.path.dirname(outputfile)
    if not os.path.exists(outdir):
        if not opts.quiet:
            print("Creating directory", outdir)
        os.makedirs(outdir)

    # Prepare data to be written
    outputdata = """#ifndef %(proj)s_VERSION_H
#define %(proj)s_VERSION_H
/* Automatically generated file: do not modify! */
#include <cstdint>
#include <string>
#include "ElementsKernel/Version.h"
const std::string %(proj)s_ORIGINAL_VERSION {"%(version)s"};
const std::string %(proj)s_VCS_VERSION {"%(vcs_version)s"};
constexpr std::uint_least64_t %(proj)s_MAJOR_VERSION = %(maj)d;
constexpr std::uint_least64_t %(proj)s_MINOR_VERSION = %(min)d;
constexpr std::uint_least64_t %(proj)s_PATCH_VERSION = %(pat)d;
constexpr std::uint_least64_t %(proj)s_VERSION = CALC_PROJECT_VERSION(%(proj)s_MAJOR_VERSION,%(proj)s_MINOR_VERSION,%(proj)s_PATCH_VERSION);
const std::string %(proj)s_VERSION_STRING {Elements::getVersionString(%(proj)s_MAJOR_VERSION,%(proj)s_MINOR_VERSION,%(proj)s_PATCH_VERSION)};
#endif
""" % { 'proj': project.upper(), 'version': version, 'vcs_version':vcs_version ,'min': minver, 'maj': majver, 'pat': patver }

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
