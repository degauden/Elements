'''
Created on Feb 10, 2017

@author: Hubert Degaudenzi


@copyright: 2012-2020 Euclid Science Ground Segment

This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation; either version 3.0 of the License, or (at your option)
any later version.

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

'''

from ElementsKernel.Path import getLocations, getPath

def getConfigurationLocations(exist_only=False):
    """
    Get all the configuration locations -- including the default ones
    """
    return getLocations("configuration", exist_only)


def getConfigurationPath(file_name, raise_exception=True):
    """
    Get full path to the file name searched in the Configuration path
    """
    return getPath(file_name, "configuration", raise_exception)


