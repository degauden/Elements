# file ZipPythonDir.py
# Script to generate a zip file that can replace a directory in the python
# path.

import os
import sys
import zipfile
import logging
import stat
import time
import re
import codecs
from StringIO import StringIO

# Add to the path the entry needed to import the locker module.
import locker

# Class for generic exception coming from the zipdir() function


class ZipdirError(RuntimeError):
    pass

# Collect the changes to be applied to the zip file.
#
#  @param directory: directory to be packed in the zip file
#  @param infolist: list of ZipInfo objects already contained in the zip archive
#
#  @return: tuple of (added, modified, untouched, removed) entries in the directory with respect to the zip file
#


def _zipChanges(directory, infolist):
    # gets the dates of the files in the zip archive
    infos = {}
    for i in infolist:
        fn = i.filename
        if fn.endswith(".pyc"):
            fn = fn[:-1]
        infos[fn] = i.date_time

    # gets the changes
    added = []
    modified = []
    untouched = []
    removed = []
    all_files = set()

    log = logging.getLogger("zipdir")
    dirlen = len(directory) + 1
    for root, dirs, files in os.walk(directory):
        if "lib-dynload" in dirs:
            # exclude the directory containing binary modules
            dirs.remove("lib-dynload")
        arcdir = root[dirlen:]
        for f in files:
            ext = os.path.splitext(f)[1]
            if ext == ".py":  # extensions that can enter the zip file
                filename = os.path.join(arcdir, f)
                all_files.add(filename)
                if filename not in infos:
                    action = "A"
                    added.append(filename)
                else:
                    filetime = time.localtime(
                        os.stat(os.path.join(directory, filename))[stat.ST_MTIME])[:6]
                    if filetime > infos[filename]:
                        action = "M"
                        modified.append(filename)
                    else:
                        action = "U"
                        untouched.append(filename)
                if action in ['U']:
                    log.debug(" %s -> %s", action, filename)
                else:
                    log.info(" %s -> %s", action, filename)
            # cases that can be ignored
            elif ext not in [".pyc", ".pyo", ".stamp", ".cmtref"] and not f.startswith('.__afs'):
                raise ZipdirError(
                    "Cannot add '%s' to the zip file, only '.py' are allowed." % os.path.join(arcdir, f))
    # check for removed files
    for filename in infos:
        if filename not in all_files:
            removed.append(filename)
            log.info(" %s -> %s", "R", filename)
    return added, modified, untouched, removed


def checkEncoding(file_obj):
    '''
    Check that a file honors the declared encoding (default ASCII for Python 2
    and UTF-8 for Python 3).

    Raises a UnicodeDecodeError in case of decoding problems and LookupError if
    the specified codec does not exists.

    See http://www.python.org/dev/peps/pep-0263/
    '''
    from itertools import islice

    # default encoding
    if sys.version_info[0] <= 2:
        enc = 'ascii'
    else:
        enc = 'utf-8'

    # find the encoding of the file, if specified (in the first two lines)
    enc_exp = re.compile(r"coding[:=]\s*([-\w.]+)")
    for l in islice(file_obj, 2):
        m = enc_exp.search(l)
        if m:
            enc = m.group(1)
            break

    if hasattr(file_obj, 'name'):
        logging.getLogger('checkEncoding').debug('checking encoding %s on %s',
                                                 enc, file_obj.name)
    else:
        logging.getLogger('checkEncoding').debug('checking encoding %s on file object',
                                                 enc)
    # try to read the file with the declared encoding
    file_obj.seek(0)
    codecs.getreader(enc)(file_obj).read()


# Make a zip file out of a directory containing python modules
def zipdir(directory, no_pyc=False):
    log = logging.getLogger("zipdir")
    if not os.path.isdir(directory):
        raise OSError(20, "Not a directory", directory)
    msg = "Zipping directory '%s'"
    if no_pyc:
        msg += " (without pre-compilation)"
    log.info(msg, directory)
    filename = os.path.realpath(directory + ".zip")

    # Open the file in read an update mode
    if os.path.exists(filename):
        zip_file = open(filename, "r+b")
    else:
        # If the file does not exist, we need to create it.
        # "append mode" ensures that, in case of two processes trying to
        # create the file, they do not truncate each other file
        zip_file = open(filename, "ab")

    locker.lock(zip_file)
    try:
        if zipfile.is_zipfile(filename):
            infolist = zipfile.ZipFile(filename).infolist()
        else:
            infolist = []
        (added, modified, untouched, removed) = _zipChanges(
            directory, infolist)
        if added or modified or removed:
            temp_buf = StringIO()
            z = zipfile.PyZipFile(temp_buf, "w", zipfile.ZIP_DEFLATED)
            for f in added + modified + untouched:
                src = os.path.join(directory, f)
                checkEncoding(open(src, 'rb'))
                if no_pyc:
                    log.debug("adding '%s'", f)
                    z.write(src, f)
                else:
                    # Remove the .pyc file to always force a re-compilation
                    if os.path.exists(src + 'c'):
                        log.debug("removing old .pyc for '%s'", f)
                        os.remove(src + 'c')
                    log.debug("adding '%s'", f)
                    z.writepy(src, os.path.dirname(f))
            z.close()
            zip_file.seek(0)
            zip_file.write(temp_buf.getvalue())
            zip_file.truncate()
            log.info("File '%s' closed", filename)
        else:
            log.info("Nothing to do on '%s'", filename)
    except UnicodeDecodeError as x:
        log.error("Wrong encoding in file '%s':", src)
        log.error("    %s", x)
        log.error("Probably you forgot the line '# -*- coding: utf-8 -*-'")
        sys.exit(1)
    finally:
        locker.unlock(zip_file)
        zip_file.close()

# Main function of the script.
#  Parse arguments and call zipdir() for each directory passed as argument


def main(argv=None):
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] directory1 [directory2 ...]")
    parser.add_option("--no-pyc", action="store_true",
                      help="copy the .py files without pre-compiling them")
    parser.add_option("--quiet", action="store_true",
                      help="do not print info messages")
    parser.add_option("--debug", action="store_true",
                      help="print debug messages (has priority over --quiet)")

    if argv is None:
        argv = sys.argv
    opts, args = parser.parse_args(argv[1:])

    if not args:
        parser.error("Specify at least one directory to zip")

    # Initialize the logging module
    level = logging.INFO
    if opts.quiet:
        level = logging.WARNING
    if opts.debug:
        level = logging.DEBUG
    logging.basicConfig(level=level)

    if "ELEMENTS_BUILD_LOCK" in os.environ:
        _scoped_lock = locker.LockFile(
            os.environ["ELEMENTS_BUILD_LOCK"], temporary=True)
    # zip all the directories passed as arguments
    for d in args:
        zipdir(d, opts.no_pyc)


if __name__ == '__main__':
    main()
