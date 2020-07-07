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

"""Useful/conventional constants."""

__all__ = ['ALL', 'VERBOSE', 'DEBUG', 'INFO', 'WARNING', 'ERROR', 'FATAL',
           'TRUE', 'FALSE']

### message levels -----------------------------------------------------------
ALL = 0
VERBOSE = 1
DEBUG = 2
INFO = 3
WARNING = 4
ERROR = 5
FATAL = 6

TRUE = True
FALSE = False

# ## message to notify folks that they really have to fix their codes:
error_explanation = """
     ==> After this line, a hack will be executed, attempting to rectify the
     ==> problem just reported. Since this hack may fail, and since it is a
     ==> rather temporary measure hack, this is an ERROR, not a WARNING."""
