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
File: python/ElementsKernel/GetElementsFiles.py

Created on: 10/10/17
Author: hubert
"""

from __future__ import division, print_function
import sys
if sys.version_info[0] < 3:
    from future_builtins import *

import os
import argparse
import ElementsKernel.Logging as log

from ElementsKernel import Path


DEFAULT_TYPE = "executable"
TYPES = [DEFAULT_TYPE, "library", "python", "configuration", "library"]

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

    parser.add_argument('-t', '--type',
                        default=DEFAULT_TYPE,
                        choices=TYPES,
                        help='The type of file to search for [default: %s]' % DEFAULT_TYPE)


    return parser


def mainMethod(args):
    """
    @brief The "main" method.
    @details
        This method is the entry point to the program. In this sense, it is
        similar to a main (and it is why it is called mainMethod()).
    """

    logger = log.getLogger('GetElementsFiles')

    stem = args.file_stem
    file_type = args.type

    if stem.find(os.path.sep) != -1 and (not Path.HAS_SUBLEVELS[file_type]):
        logger.error("The search stem cannot contain \"%s\" for the %s type", os.path.sep, file_type)
        return 1

    locations = Path.getLocations(file_type, exist_only=True)

    if stem:
        found_list = Path.getAllPathFromLocations(stem, locations)
    else:
        logger.error("You must provide a term to look for.")
        return 1

    this_project_root = os.environ.get("THIS_PROJECT_ROOT", None)

    if this_project_root and args.self:
        found_list = [f for f in found_list if f.startswith(this_project_root)]


    for f in found_list:
        print(f)



