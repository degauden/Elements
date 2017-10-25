""" merge_files <fragment> file into a 'per-project' <merged> file """

import os
import sys
from datetime import datetime
import locker


def mergeFiles(frag_file_names, merged_file_name, comment_char,
               do_merge, ignore_missing):
    """ main function of the merge script """

    start_mark = "%s --Beg " % comment_char
    time_mark = "%s --Date inserted: %s" % (comment_char, datetime.now())
    end_mark = "%s --End " % comment_char
    name_offset = len(start_mark)

    basenames = map(os.path.basename, frag_file_names)

    is_new_file = not os.path.exists(merged_file_name)

    # create an empty file if it does not exist
    # "append mode" ensures that, in case of two processes trying to
    # create the file, they do not truncate each other file
    if is_new_file:
        # check if the destination directory exists
        path_to_file = os.path.split(merged_file_name)[0]
        if path_to_file and not os.path.isdir(path_to_file):
            # if doesn't exist, create it
            os.makedirs(path_to_file)
        open(merged_file_name, 'a')

    merged_file = open(merged_file_name, 'r+')

    # locking file, gaining exclusive access to it
    _ = locker.lock(merged_file)
    try:

        new_lines = []
        skip_block = ""
        for line in merged_file.readlines():
            if line.startswith(start_mark) and line[name_offset:].strip() in basenames:
                skip_block = end_mark + line[name_offset:].strip()
                # remove all the empty lines occurring before the start mark
                while (len(new_lines) > 0) and (new_lines[-1].strip() == ''):
                    new_lines.pop()
            if not skip_block:
                new_lines.append(line)
            if line.startswith(skip_block):
                skip_block = ""
        if skip_block:
            print("WARNING: missing end mark ('%s')", skip_block)

        if do_merge:
            for f in frag_file_names:
                if ignore_missing and not os.path.exists(f):
                    print("WARNING: '%s' does not exist, I'm ignoring it", f)
                    continue
                # I do not want to add 2 empty lines at the beginning of a file
                if new_lines:
                    new_lines.append(os.linesep + os.linesep)
                bf = os.path.basename(f)
                new_lines.append(start_mark + bf + os.linesep)
                new_lines.append(time_mark + os.linesep)
                file_data = open(f, 'r').read()
                new_lines.append(file_data)
                if file_data and file_data[-1] != os.linesep:
                    new_lines.append(os.linesep)
                new_lines.append(end_mark + bf + os.linesep)

        merged_file.seek(0)
        merged_file.truncate(0)
        merged_file.writelines(new_lines)

    finally:
        # unlock file
        locker.unlock(merged_file)

    return 0

if __name__ == "__main__":

    from optparse import OptionParser
    parser = OptionParser(usage="usage: %prog [options]")
    parser.add_option(
        "-i",
        "--input-file",
        action="append",
        dest="frag_file_names",
        default=[],
        help="The path and name of the file one wants to merge into the 'master' one"
    )
    parser.add_option(
        "-m",
        "--merged-file",
        dest="merged_file_name",
        default=None,
        help="The path and name of the 'master' file which will hold the content of all the other fragment files"
    )
    parser.add_option(
        "-c",
        "--comment-char",
        dest="comment_char",
        default="#",
        help="The type of the commenting character for the type of files at hand"
             "(this is an attempt at handling the largest possible use cases)"
    )
    parser.add_option(
        "--do-merge",
        dest="do_merge",
        action="store_true",
        default=True,
        help="Switch to actually carry on with the merging procedure"
    )
    parser.add_option(
        "--un-merge",
        dest="un_merge",
        action="store_true",
        default=False,
        help="Switch to remove our fragment file from the 'master' file"
    )
    parser.add_option(
        "--stamp-dir",
        dest="stamp_dir",
        action="store",
        default=None,
        help="Create the stamp file in the specified directory. If not specified"
        + " the directory of the source file is used."
    )
    parser.add_option(
        "--no-stamp",
        action="store_true",
        help="Do no create stamp files."
    )
    parser.add_option(
        "--ignore-missing",
        dest="ignore_missing",
        action="store_true",
        help="Print a warning if a fragment file is missing, but do not fail."
    )

    (options, args) = parser.parse_args()

    # ensure consistency...
    options.do_merge = not options.un_merge

    # allow command line syntax as
    # merge_files.py [options] <fragment file1> [<fragment file2>...] <merged
    # file>
    if args:
        options.merged_file_name = args[-1]
        options.frag_file_names += args[:-1]

    sc = 1
    if not options.frag_file_names or \
       not options.merged_file_name:
        str(parser.print_help() or "")
        print("*** ERROR ***", sys.argv)
        sys.exit(sc)

    if options.stamp_dir is None:
        stampFileName = lambda x: x + ".stamp"
    else:
        stampFileName = lambda x: os.path.join(options.stamp_dir,
                                               os.path.basename(x)
                                               + ".stamp")
    # Configure Python logging
    import logging
    logging.basicConfig(level=logging.INFO)

    if "ELEMENTS_BUILD_LOCK" in os.environ:
        globalLock = locker.LockFile(
            os.environ["ELEMENTS_BUILD_LOCK"], temporary=True)
    else:
        globalLock = None

    if True:  # try:
        sc = mergeFiles(options.frag_file_names, options.merged_file_name,
                        options.comment_char,
                        do_merge=options.do_merge,
                        ignore_missing=options.ignore_missing)
        if not options.no_stamp:
            for stamp in map(stampFileName, options.frag_file_names):
                open(stamp, 'w')

    del globalLock

    sys.exit(sc)
