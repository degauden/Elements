#!/usr/bin/env python

import os
from optparse import OptionParser


def main():
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
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "ElementsKernel/Unused.h"
#include "ElementsKernel/Export.h"

ELEMENTS_API int main(ELEMENTS_UNUSED int argc, ELEMENTS_UNUSED char **argv)
{
  CppUnit::TextUi::TestRunner runner;
  CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
  runner.addTest( registry.makeTest() );
  bool wasSuccessful = runner.run( "", false );
  return (wasSuccessful? 0 : 1 );
}

"""

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
