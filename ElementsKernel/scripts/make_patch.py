#!/usr/bin/env python

import os
import sys
import re
from subprocess import Popen, PIPE
from fnmatch import fnmatch
import logging


def command(cmd, *args, **kwargs):
    """
    Simple wrapper to execute a command and return standard output and standard error.
    """
    d = {"stdout": PIPE, "stderr": PIPE}
    d.update(kwargs)
    cmd = [cmd] + list(args)
    logging.debug("Execute command: %r %r", " ".join(cmd), kwargs)
    proc = apply(Popen, (cmd,), d)
    return proc.communicate()

cmt = lambda *args, **kwargs: apply(command, ("cmt",) + args, kwargs)
cvs = lambda *args, **kwargs: apply(command, ("cvs",) + args, kwargs)
svn = lambda *args, **kwargs: apply(command, ("svn",) + args, kwargs)


def broadcast_packages():
    """
    Find the local packages the current one depends on (using 'cmt broadcast').
    Returns a list of pairs ("package name","path to the cmt directory").
    """
    # make cmt print one line per package with python syntax
    if not sys.platform.startswith("win"):
        pkg_dirs = "[\n" + \
            cmt("broadcast", r'echo "(\"<package>\", \"$PWD\"),"')[0] + ']'
    else:
        pkg_dirs = "[\n" + cmt("broadcast",
                               r'echo ("<package>", r"%<package>root%\cmt"),')[0] + ']'
    # Clean up a bit the output (actually needed only on Windows because of
    # the newlines)
    pkg_dirs = "\n".join(
        [l.strip() for l in pkg_dirs.splitlines() if not l.startswith("#")])
    return eval(pkg_dirs)


def matches(filename, patterns):
    """
    Returns True if any of the specified glob patterns (list of strings) matched
    the string 'filename'.
    """
    for p in patterns:
        if fnmatch(filename, p):
            logging.debug("Excluding file: %r", filename)
            return True
    return False


def expand_dirs(files, basepath=""):
    """
    Replace the entries in files that correspond to directories with the list of
    files in those directories.
    """
    if basepath:
        lb = len(basepath) + 1
    else:
        lb = 0
    newlist = []
    for f in files:
        base = os.path.join(basepath, f)
        if os.path.isdir(base):
            for root, ds, fs in os.walk(base):
                for ff in fs:
                    newlist.append(os.path.join(root, ff)[lb:])
        else:
            newlist.append(f)
    return newlist


def revision_diff_cmd(cwd):
    if os.path.isdir(os.path.join(cwd, "CVS")):
        return cvs("diff", "-upN", cwd=cwd)
    else:
        # special treatment to show new files in a way compatible with CVS
        out, err = svn("status", cwd=cwd)

        newfiles = [l
                    for l in out.splitlines()
                    if l.startswith("? ")]
        out, err = svn("diff", cwd=cwd)
        if newfiles:
            out = "\n".join(newfiles) + "\n" + out
        return out, err


def diff_pkg(name, cmtdir, exclusions=[]):
    """
    Return the patch data for a package.
    """
    rootdir = os.path.dirname(cmtdir)
    out, err = revision_diff_cmd(cwd=rootdir)
    # extract new files
    new_files = [l.split(None, 1)[1]
                 for l in out.splitlines()
                 if l.startswith("? ")]
    new_files = expand_dirs(new_files, rootdir)
    new_files = [f
                 for f in new_files
                 if not matches(f, exclusions)]
    # make diff segments for added files
    for f in new_files:
        logging.info("Added file %r", f)
        #out += "diff -u -p -N %s\n" % os.path.basename(f)
        # out += command("diff", "-upN", "/dev/null", f,
        #               cwd = rootdir)[0]
        out += "Index: %s\n" % f
        out += "===================================================================\n"
        out += command("diff", "-upN", "/dev/null", f,
                       cwd=rootdir)[0]
    # extract removed files
    removed_files = [l.split()[-1]
                     for l in err.splitlines()
                     if "cannot find" in l]
    removed_files = [f
                     for f in removed_files
                     if not matches(f, exclusions)]
    # make diff segments for removed files (more tricky)
    for f in removed_files:
        logging.info("Removed file %r", f)
        # retrieve the original content from CVS
        orig = cvs("up", "-p", f,
                   cwd=rootdir)[0]
        out += "diff -u -p -N %s\n" % os.path.basename(f)
        out += "--- %s\t1 Jan 1970 00:00:00 -0000\n" % f
        out += "+++ /dev/null\t1 Jan 1970 00:00:00 -0000\n"
        lines = orig.splitlines()
        out += "@@ -1,%d +0,0 @@\n" % len(lines)
        for l in lines:
            out += '-%s\n' % l
    # Fix the paths to have the package names
    rex = re.compile(r"^(Index: |\? |\+\+\+ |--- (?!/dev/null))", re.MULTILINE)
    out = rex.sub(r"\1%s/" % name, out)
    return out


def main():
    from optparse import OptionParser
    parser = OptionParser(description="Produce a patch file from a CMT project. "
                          "The patch contains the changes with respect "
                          "to the CVS repository, including new files "
                          "that are present only locally. Run the script "
                          "from the cmt directory of a package.")
    parser.add_option("-x", "--exclude", action="append", type="string",
                      metavar="PATTERN", dest="exclusions",
                      help="Pattern to exclude new files from the patch")
    parser.add_option("-o", "--output", action="store", type="string",
                      help="Name of the file to send the output to. Standard "
                           "output is used if not specified")
    parser.add_option("-v", "--verbose", action="store_true",
                      help="Print some progress information on standard error")
    parser.add_option("--debug", action="store_true",
                      help="Print debug information on standard error")
    parser.set_defaults(exclusions=[])

    opts, args = parser.parse_args()

    if opts.debug:
        logging.basicConfig(level=logging.DEBUG)
    elif opts.verbose:
        logging.basicConfig(level=logging.INFO)

    # default exclusions
    opts.exclusions += ["*.py[co]",
                        "*.patch",
                        "cmt/cleanup.*",
                        "cmt/setup.*",
                        "cmt/*.make",
                        "cmt/Makefile",
                        "cmt/*.nmake",
                        "cmt/*.nmakesav",
                        "cmt/NMake",
                        "cmt/install*.history",
                        "cmt/build.*.log",
                        "cmt/version.cmt",
                        "genConf",
                        "slc3_ia32_gcc323*",
                        "slc4_ia32_gcc34*",
                        "slc4_amd64_gcc34*",
                        "slc4_amd64_gcc43*",
                        "win32_vc71*",
                        "i686-slc3-gcc323*",
                        "i686-slc4-gcc34*",
                        "i686-slc4-gcc41*",
                        "x86_64-slc4-gcc34*",
                        "x86_64-slc4-gcc41*",
                        "i686-slc5-gcc43*",
                        "x86_64-slc5-gcc43*",
                        "x86_64-slc5-icc*",
                        ]
    if "CMTCONFIG" in os.environ:
        opts.exclusions.append(os.environ["CMTCONFIG"])

    # check if we are in the cmt directory before broadcasting
    if not (os.path.basename(os.getcwd()) == "cmt" and os.path.exists("requirements")):
        logging.error(
            "This script must be executed from the cmt directory of a package.")
        return 1

    pkgs = broadcast_packages()
    num_pkgs = len(pkgs)
    count = 0

    patch = ""
    for name, path in pkgs:
        count += 1
        logging.info("Processing %s from %s (%d/%d)",
                     name, os.path.dirname(path), count, num_pkgs)
        patch += diff_pkg(name, path, opts.exclusions)

    if sys.platform.startswith("win"):
        # fix newlines chars
        patch = patch.replace("\r", "")

    if opts.output:
        logging.info("Writing patch file %r", opts.output)
        open(opts.output, "w").write(patch)
    else:
        sys.stdout.write(patch)
    return 0

if __name__ == "__main__":
    sys.exit(main())
