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

from ElementsKernel.System import SHLIB_VAR_NAME, DEFAULT_INSTALL_PREFIX
import os
import sys
import re
from distutils.sysconfig import get_python_lib

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

DEFAULT_INSTALL_LOCATIONS = { "executable": [ os.path.join(DEFAULT_INSTALL_PREFIX, "bin")],
                                 "library": [ os.path.join(DEFAULT_INSTALL_PREFIX, "lib64"),
                                              os.path.join(DEFAULT_INSTALL_PREFIX, "lib"),
                                              os.path.join(DEFAULT_INSTALL_PREFIX, "lib32")],
                                  "python": [ get_python_lib(prefix=DEFAULT_INSTALL_PREFIX)],
                           "configuration": [ os.path.join(DEFAULT_INSTALL_PREFIX, "conf") ],
                               "auxiliary": [ os.path.join(DEFAULT_INSTALL_PREFIX, "auxdir"),
                                             os.path.join(DEFAULT_INSTALL_PREFIX, "aux")]}

HAS_SUBLEVELS = { "executable": False,
                     "library": False,
                      "python": True,
               "configuration": True,
                   "auxiliary": True}


def getLocations(file_type="executable", exist_only=False):
    """
    Get the locations of a type of file -- including the default ones
    """

    location_list = getLocationsFromEnv(VARIABLE[file_type], exist_only)

    location_list += DEFAULT_INSTALL_LOCATIONS[file_type]

    if exist_only:
        location_list = [p for p in location_list if os.path.exists(p)]

    return location_list

def getPath(file_name, file_type="executable", raise_exception=True):
    """
    Get full path to the file name searched in the file-type path
    """

    location_list = getLocations(file_type)

    result = getPathFromLocations(file_name, location_list)

    if not result and raise_exception:
        raise Exception("The %s file \"%s\" cannot be found!", file_type, file_name)

    return result


def getLocationsFromEnv(path_variable, exist_only=False):
    """
    Get the list of locations provided by the path
    environment variable.
    """

    env_content = os.environ.get(path_variable, None)

    found_list = []
    if env_content:
        found_list = env_content.split(PATHSEP)

    if exist_only:
        found_list = [p for p in found_list if os.path.exists(p)]

    return found_list

def getPathFromLocations(file_name, locations):
    """
    Get the path to the searched  file name from the
    provided locations. The first found wins.
    """

    for l in locations:
        file_path = os.path.join(l, file_name)
        if os.path.exists(file_path):
            return file_path

    return None

def getAllPathFromLocations(file_name, locations):
    """
    Get all the paths to the searched  file name from the
    provided locations.
    """

    file_list = []

    for l in locations:
        file_path = os.path.join(l, file_name)
        if os.path.exists(file_path):
            file_list.append(file_path)

    return list(set(file_list))


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

def which(program):
    """ Command to assert the existance of an executable
        @param program: program path, absolute or relative
    """
    def is_exe(fpath):
        return os.path.isfile(fpath) and os.access(fpath, os.X_OK)

    fpath, _ = os.path.split(program)
    if fpath:
        if is_exe(program):
            return program
    else:
        for path in os.environ["PATH"].split(os.pathsep):
            path = path.strip('"')
            exe_file = os.path.join(path, program)
            if is_exe(exe_file):
                return exe_file

    return None


def pyVersionWhich(program, program3_prefix=None):
    """ Version of which that returns the right executable
        depending on the calling python version.
        @param program: program path, absolute or relative
        @param program3: program3 prefix,
    """
    executable_name = None
    python_version = ""

    if not program3_prefix:
        program3_prefix = program

    m = re.match(r".*python(.*)$", sys.executable)
    if m:
        python_version = m.group(1)

    if python_version:
        executable_name = program3_prefix + python_version
    else:
        executable_name = program

    return which(executable_name)
