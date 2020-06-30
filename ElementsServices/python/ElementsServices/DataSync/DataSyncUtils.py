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


def localDirExists (local_dir):
    """Check whether a local directory exists.
    """
    if not local_dir:
        return True
    if os.path.isdir(local_dir):
        return True
    return False


def createLocalDirOf (local_file):
    """Create the parent directory for a local file.
    """
    dir_name = os.path.dirname(local_file)
    if not localDirExists(dir_name):
        os.makedirs(dir_name)


def environmentVariable (name):
    """Get the value of an environment variable or '' if it does not exist.
    """
    return os.environ.get(name, "")

def localWorkspacePrefix ():
    """Get the prefix of the local workspace.
    """
    codeen_prefix = 'WORKSPACE'
    return environmentVariable(codeen_prefix)


def concatenatePaths (chunks):
    """Concatenate path chunks into a single path.
    """
    meaningful_chunks = [x for x in chunks if x]
    joined = '/'.join(meaningful_chunks)
    return os.path.normpath(joined)
