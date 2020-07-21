import os
import re
from optparse import OptionParser

def main():
    parser = OptionParser(
        usage="ERROR: Usage %prog <project> <outputfile>")
    parser.add_option("-q", "--quiet", action="store_true",
                      help="Do not print messages.")
    opts, args = parser.parse_args()

    if len(args) != 2:
        parser.error("wrong number of arguments: %d: %s" % (len(args), " ".join(args)))

    project, outputfile = args
    if not opts.quiet:
        print("Creating %s for %s" % (outputfile, project))

    outdir = os.path.dirname(outputfile)
    if not os.path.exists(outdir):
        if not opts.quiet:
            print("Creating directory", outdir)
        os.makedirs(outdir)

    # Prepare data to be written
    outputdata = """
#ifndef _%(proj)s_EXPORT_H_
#define _%(proj)s_EXPORT_H_

// ---------------------------------- Symbol visibility macros (begin)
#if defined(%(proj)s_HIDE_SYMBOLS)
/// These macros will allow selection on exported symbols
/// taken from http://gcc.gnu.org/wiki/Visibility
#if defined(__GNUC__) && not defined(__CINT__)
#  define %(proj)s_HASCLASSVISIBILITY
#endif

#if defined(%(proj)s_HASCLASSVISIBILITY)
#  define %(proj)s_IMPORT __attribute__((visibility("default")))
#  define %(proj)s_EXPORT __attribute__((visibility("default")))
#  define %(proj)s_LOCAL  __attribute__((visibility("hidden")))
#else
#  define %(proj)s_IMPORT
#  define %(proj)s_EXPORT
#  define %(proj)s_LOCAL
#endif

/// Define %(proj)s_API for DLL builds
#ifdef ELEMENTS_LINKER_LIBRARY
#define %(proj)s_API %(proj)s_EXPORT
#else
#define %(proj)s_API %(proj)s_IMPORT
#endif
#else
/// Dummy definitions for the backward compatibility mode.
#define %(proj)s_API
#define %(proj)s_IMPORT
#define %(proj)s_EXPORT
#define %(proj)s_LOCAL
#endif  // defined(%(proj)s_HIDE_SYMBOLS)

// ---------------------------------- Symbol visibility macros (end)

#endif  // _%(proj)s_EXPORT_H_

""" % { 'proj': project.upper() }

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
