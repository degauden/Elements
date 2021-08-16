#
# Copyright (C) 2012-2020 Euclid Science Ground Segment
#
# This library is free software; you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by the Free
# Software Foundation; either version 3.0 of the License, or (at your option)
# any later version.
#
# This library is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
#

"""
:file: ElementsKernel/GetElementsFiles.py

:date: Created on: 10/10/17
:author: Hubert Degaudenzi
"""

import os
import argparse
import ElementsKernel.Logging as log

from ElementsKernel import Path, Exit

DEFAULT_TYPE = "executable"
TYPES = [DEFAULT_TYPE, "library", "python", "configuration", "auxiliary"]


def defineSpecificProgramOptions():
    """
    @brief Allows to define the (command line and configuration file) options
    specific to this program

    @details
        See the Elements documentation for more details.
    @return
        An  ArgumentParser.
    """
    parser = argparse.ArgumentParser()

    parser.add_argument('file_stem', metavar='file-stem',
                        type=str,
                        default="",
                        nargs='?',
                        help='File stem')

    parser.add_argument('-s', '--self',
                        default=False,
                        action="store_true",
                        help='Get only the files from the current project. No recursion.')

    parser.add_argument('-d', '--with-defaults',
                        default=False,
                        action="store_true",
                        help='Add the system internal paths to the environment for the lookup')

    parser.add_argument('-t', '--type',
                        default=DEFAULT_TYPE,
                        choices=TYPES,
                        help='The type of file to search for [default: %s]' % DEFAULT_TYPE)

    return parser


def selfFilter(f_list, only_self):
    """
    @brief function to filter the entries that don't belong to the
    current program.
    """

    this_project_root = os.environ.get("THIS_PROJECT_ROOT", None)

    if this_project_root and only_self:
        f_list = [f for f in f_list if f.startswith(this_project_root)]

    return f_list


def mainMethod(args):
    """
    @brief The "main" method.
    @details This method is the entry point to the program. In this sense, it is
    similar to a main (and it is why it is called mainMethod()).
    """

    logger = log.getLogger('GetElementsFiles')

    exit_code = Exit.Code["OK"]

    stem = args.file_stem
    file_type = args.type

    if stem.find(os.path.sep) != -1 and (not Path.HAS_SUBLEVELS[file_type]):
        logger.error("The search stem cannot contain \"%s\" for the %s type", os.path.sep, file_type)
        exit_code = Exit.Code["NOT_OK"]

    locations = Path.getLocations(file_type, exist_only=True, with_defaults=args.with_defaults)

    if stem:
        found_list = Path.getAllPathFromLocations(stem, locations)
    else:
        logger.info("No stem provided. Listing all files")
        found_list = []
        for l in locations:
            for root, _, files in os.walk(l):
                for f in files:
                    found_list.append(os.path.join(root, f))

    found_list = selfFilter(found_list, args.self)

    for f in found_list:
        print(f)

    return exit_code

