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

import os
import shlex
import subprocess

from ElementsKernel.Configuration import getConfigurationPath


def dataSyncConfFilePath (filename):
    """Get the path of a configuration file for the data synchronization tool.
    """
    return getConfigurationPath(filename)


def runCommandAndCaptureOutErr (cmd):
    """Execute a command and return its output and error messages.
    """
    p = subprocess.Popen(
        shlex.split(cmd),
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE)
    out, err = p.communicate()
    return out.decode("utf-8"), err.decode("utf-8")


def localDirExists (localDir):
    """Check whether a local directory exists.
    """
    if not localDir:
        return True
    if os.path.isdir(localDir):
        return True
    return False


def createLocalDirOf (localFile):
    """Create the parent directory for a local file.
    """
    dir_name = os.path.dirname(localFile)
    if not localDirExists(dir_name):
        os.makedirs(dir_name)


def environmentVariable (name):
    """Get the value of an environment variable or '' if it does not exist.
    """
    return os.environ.get(name, "")

def localWorkspacePrefix ():
    """Get the prefix of the local workspace.
    """
    codeenPrefix = 'WORKSPACE'
    return environmentVariable(codeenPrefix)


def concatenatePaths (chunks):
    """Concatenate path chunks into a single path.
    """
    meaningfulChunks = [x for x in chunks if x]
    joined = '/'.join(meaningfulChunks)
    return os.path.normpath(joined)
