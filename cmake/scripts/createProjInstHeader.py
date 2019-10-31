import os
import re
from optparse import OptionParser


def main():
    parser = OptionParser(
        usage="ERROR: Usage %prog <project> <location> <used_projects> <outputfile>")
    parser.add_option("-q", "--quiet", action="store_true",
                      help="Do not print messages.")

    parser.set_defaults(so_version=False)
    parser.add_option("-V", "--so-version", action="store_true",
                      help="Use So Version")
    
    opts, args = parser.parse_args()

    if len(args) > 4:
        parser.error("wrong number of arguments: %s" % ",".join(args))

    if len(args) == 3:
        project, location, outputfile = args
        used_projects = []
    else:
        project, location, used_projects, outputfile = args
        used_projects = used_projects.split(":")

    so_value = "false"
    if (opts.so_version):
        so_value = "true"

    if not opts.quiet:
        print("Creating %s for %s with %s install location" % (outputfile, project, location))
        

    outdir = os.path.dirname(outputfile)
    if not os.path.exists(outdir):
        if not opts.quiet:
            print("Creating directory", outdir)
        os.makedirs(outdir)

    # Prepare data to be written
    outputdata = """#ifndef %(proj)s_INSTALL_H
#define %(proj)s_INSTALL_H
/* Automatically generated file: do not modify! */

#include <string>       // for string
#include <vector>       // for vector
""" % { 'proj': project.upper() }

    for p in used_projects:
        outputdata += """#include "%(proj)s_INSTALL.h"
""" % { 'proj': p.upper() }

    outputdata += """
const std::string %(proj)s_INSTALL_LOCATION_STRING {"%(location)s"};
""" % { 'proj': project.upper(), 'location': location }

    outputdata += """
const bool %(proj)s_USE_SOVERSION {%(so_value)s};
""" % { 'proj': project.upper(), 'so_value': so_value }


    used_locations = []
    used_locations.append("%(proj)s_INSTALL_LOCATION_STRING" % { 'proj': project.upper()})
    for p in reversed(used_projects):
        used_locations.append("%(proj)s_INSTALL_LOCATION_STRING" % { 'proj': p.upper()})

    outputdata += """
const std::vector<std::string> %(proj)s_SEARCH_DIRS {%(locations)s};

#endif
""" % { 'proj': project.upper(), 'locations': ", ".join(used_locations) }


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
