'''
Created on Apr 29, 2016

@author: hubert


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

from ElementsKernel.System import SHLIB_VAR_NAME
import os

Type = ["executable", "library", "python", "configuration", "auxiliary"]


PATHSEP = os.pathsep

VARIABLE = {"executable": "PATH",
            "library": SHLIB_VAR_NAME,
            "python":  "PYTHONPATH",
            "configuration": "ELEMENTS_CONF_PATH",
            "auxiliary": "ELEMENTS_AUX_PATH"}

SUFFIXES = {"executable": ["scripts", "bin"],
            "library": ["lib"],
            "python": ["python"],
            "configuration": ["conf"],
            "auxiliary": ["auxdir", "aux"]}


def getLocationsFromEnv(path_variable, exist_only=False):
    
    env_content = os.environ.get(path_variable, None)
    
    found_list = []
    if (env_content) :
        found_list = env_content.split(PATHSEP)
        
    if exist_only:
        found_list = [ p for p in found_list if os.path.exists(p)]
        
    return found_list

def getPathFromLocations(file_name, locations):
    
    for l in locations:
        file_path = os.path.join(l, file_name)
        if os.path.exists(file_path):
            return file_path
        
    return None

def getPathFromEnvVariable(file_name, path_variable):
    """
    Look for the first path valid in the <path_variable> environment
    variable where is located the <file_name> file. It returns the
    filename with the path or an empty string if not found. We assume that the
    file_name also contains any sub directory under the <path_variable>
    environment variable entry.
    @param file_name: file name to look for can be "Some.txt" or "/" path
    component like "SomeFolder/Some.txt"
    @param path_variable: name of the environment variable to look into
    @return: full path to the first match entry.
    """

    location_list = getLocationsFromEnv(path_variable)
    
    return getPathFromLocations(file_name, location_list)


def joinPath(path_list):
    """ stupid wrapper to look like the C++ call """
    return os.pathsep.join(path_list)


def multiPathAppend(initial_locations, suffixes):
    """ Function to append all the suffixes to
    all the initial location
    @param initial_locations: as quoted the initial paths to be 
    appended to
    @param suffixes: the extensions to be appended.
    """
    result = []

    for l in initial_locations:
        result += [os.path.join(l, s) for s in suffixes]

    return result

