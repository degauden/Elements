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


""" This module irons out some of the system dependent constants

:file: ElementsKernel/Project.py
:author: Hubert Degaudenzi

:date: 28.07.2016

"""

import platform

if platform.system() == "Darwin":
    SHLIB_VAR_NAME = "DYLD_LIBRARY_PATH"
else:
    SHLIB_VAR_NAME = "LD_LIBRARY_PATH"

LIB_PREFIX = "lib"


if platform.system() == "Darwin":
    LIB_EXTENSION = "dylib"
else:
    LIB_EXTENSION = "so"

LIB_SUFFIX = "." + LIB_EXTENSION

SHLIB_SUFFIX = LIB_SUFFIX

DEFAULT_INSTALL_PREFIX = "/usr"

