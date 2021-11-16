""" Module to remove non-existing entries
    in the path list
"""

from sys import stdout
from os import pathsep, listdir, environ, fdopen, linesep
from os.path import exists, isdir, realpath
from optparse import OptionParser, OptionValueError
from tempfile import mkstemp
from zipfile import is_zipfile

EXIST_ONLY_DEFAULT = False


def stripPath(path, exist_only=EXIST_ONLY_DEFAULT):
    """ Main function to remove non-existing entries
    in the path list
    """

    # We keep the entry if it exists
    path_check = exists

    if not exist_only:
        # We keep the entry if it is a directory not empty or a zipfile
        path_check = lambda p: exists(p) and ((isdir(p) and listdir(p)) or is_zipfile(p))

    collected = []
    for p in path.split(pathsep):
        if p not in collected:
            try:
                if path_check(realpath(p)):
                    collected.append(p)
            except OSError:
                pass
    return pathsep.join(collected)


def cleanVariable(varname, shell, out, exist_only=EXIST_ONLY_DEFAULT):
    """ Clean up the variable and write out the shell snipet"""
    if varname in environ:
        pth = stripPath(environ[varname], exist_only)
        if shell == "csh" or shell.find("csh") != -1:
            out.write("setenv %s %s" % (varname, pth) + linesep)
        elif shell == "sh" or shell.find("sh") != -1:
            out.write("export %s=%s" % (varname, pth) + linesep)
        elif shell == "bat":
            out.write("set %s=%s" % (varname, pth) + linesep)


def _check_output_options_cb(option, opt_str, value, parser):
    """ Callback for the output options"""
    if opt_str == "--mktemp":
        if parser.values.output != stdout:
            raise OptionValueError(
                "--mktemp cannot be used at the same time as --output")
        else:
            parser.values.mktemp = True
            fd, outname = mkstemp()
            parser.values.output = fdopen(fd, "w")
            print(outname)
    elif opt_str == "--output" or opt_str == "-o":
        if parser.values.mktemp:
            raise OptionValueError(
                "--mktemp cannot be used at the same time as --output")
        else:
            parser.values.output = open(value, "w")


if __name__ == '__main__':

    parser = OptionParser()

    parser.add_option("-e", "--env",
                      action="append",
                      dest="envlist",
                      metavar="PATHVAR",
                      help="add environment variable to be processed")
    parser.add_option("--shell", action="store", dest="shell", type="choice", metavar="SHELL",
                      choices=['csh', 'sh', 'bat'],
                      help="select the type of shell to use")

    parser.set_defaults(output=stdout)
    parser.add_option("-o",
                      "--output",
                      action="callback",
                      metavar="FILE",
                      type="string",
                      callback=_check_output_options_cb,
                      help="(internal) output the command to set up the environment"
                            "of the given file instead of stdout")
    parser.add_option("--mktemp", action="callback",
                      dest="mktemp",
                      callback=_check_output_options_cb,
                      help="(internal) send the output to a temporary file and print"
                           "on stdout the file name (like mktemp)")

    parser.set_defaults(exist_only=EXIST_ONLY_DEFAULT)
    parser.add_option("--exist-only",
                      dest="exist_only",
                      action="store_true",
                      help="Check only the existence of the directories [default: %default]")

    options, args = parser.parse_args()

    if not options.shell and "SHELL" in environ:
        options.shell = environ["SHELL"]

    if options.envlist:
        for v in options.envlist:
            cleanVariable(v, options.shell, options.output, options.exist_only)

    for a in args:
        print(stripPath(a), options.exist_only)
