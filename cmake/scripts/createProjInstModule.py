import os
import re
from optparse import OptionParser


def main():
    parser = OptionParser(
        usage="ERROR: Usage %prog <project> <location> <used_projects> <outputfile>")
    parser.add_option("-q", "--quiet", action="store_true",
                      help="Do not print messages.")
    parser.set_defaults(install_prefix="")
    parser.add_option("-i", "--install-prefix", action="store",
                     help="set the CMAKE_INSTALL_PREFIX")

    opts, args = parser.parse_args()

    if len(args) > 4:
        parser.error("wrong number of arguments: %s" % ",".join(args))

    if len(args) == 3:
        project, location, outputfile = args
        used_projects = []
    else:
        project, location, used_projects, outputfile = args
        used_projects = used_projects.split(":")

    if not opts.quiet:
        print("Creating %s for %s with %s install location" % (outputfile, project, location))

    outdir = os.path.dirname(outputfile)
    if not os.path.exists(outdir):
        if not opts.quiet:
            print("Creating directory", outdir)
        os.makedirs(outdir)

    # Prepare data to be written
    outputdata = """# Automatically generated file: do not modify!
"""
    for p in used_projects:
        outputdata += """from %(proj)s_INSTALL import %(proj)s_INSTALL_LOCATION
""" % { 'proj': p.upper() }

    outputdata += """
%(proj)s_INSTALL_LOCATION = "%(location)s"
""" % { 'proj': project.upper(), 'location': location }

    used_locations = []
    used_locations.append("%(proj)s_INSTALL_LOCATION" % { 'proj': project.upper()})
    for p in reversed(used_projects):
        used_locations.append("%(proj)s_INSTALL_LOCATION" % { 'proj': p.upper()})

    outputdata += """
CMAKE_INSTALL_PREFIX = "%(prefix)s"
""" % { 'prefix': opts.install_prefix}


    outputdata += """
%(proj)s_SEARCH_DIRS = [%(locations)s]
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
