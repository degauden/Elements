'''
Created on Feb 10 2017

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

import shutil
import os

from ElementsKernel.Path import getLocations, getPath, getTargetPath

def getAuxiliaryLocations(exist_only=False):
    """
    Get all the auxiliary locations -- including the default ones
    """
    return getLocations("auxiliary", exist_only)


def getAuxiliaryPath(file_name, raise_exception=True):
    """
    Get full path to the file name searched in the auxiliary path
    """
    return getPath(file_name, "auxiliary", raise_exception)


def configure(file_name, target_dir, target_name=None, use_stem=False,
              configuration=None, create_missing_dir=False):
    """ Copy/configuration of a file
        @param file_name: the original file name with or without a stem
        @param target_dir: the target directory
        @param target_name: the target name if any
        @param use_stem: choose if the stem (dirname) of the file_name will 
                         be used or only the last component (basename) of 
                         the file_name path.
        @param configuration: dictionary of item to replace in the text
        @param create_missing_dir: create intermediate directories if they
                                          don't exist
        @return: the final target path for the copy/configuration.
    """  
    file_path = getAuxiliaryPath(file_name)
    target_path = getTargetPath(file_name, target_dir, target_name, use_stem)
            
    if create_missing_dir:
        parent_path = os.path.dirname(target_path)
        if not os.path.exists(parent_path):
            os.makedirs(parent_path)

    if not configuration:
        shutil.copy(file_path, target_path)
    else:
        file_txt = open(file_path).read()
        with open(target_path, "w") as tf:
            tf.write(file_txt % configuration)

    return target_path

