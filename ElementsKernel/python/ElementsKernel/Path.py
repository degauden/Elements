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


from ElementsKernel import Logging
import os

Type = ["executable", "library", "python", "configuration", "auxiliary"]

SUFFIXES = { "executable": ["scripts", "bin"],
             "library": ["lib"],
             "python": ["python"],
             "configuration": ["conf"],
             "auxiliary": ["auxdir", "aux"] }

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

    full_path = ""

    log = Logging.getLogger(None)

    found = False
    file_name = file_name.replace("/", os.sep)

    for elt in os.environ[path_variable].split(os.pathsep):
        full_path = os.path.join(elt, file_name)
        # look for the first valid path
        if os.path.exists(full_path):
            found = True
            log.debug("File %s found at the %s location of %s", file_name, elt, path_variable)
            break

    if not found:
        full_path = ''
        log.error("File %s not found in %s", file_name, path_variable)


    return full_path
