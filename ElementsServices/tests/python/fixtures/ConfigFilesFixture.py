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

import os.path

from ElementsServices.DataSync.DataSyncUtils import *
from ElementsKernel.Temporary import TempDir, TempEnv

class WorkspaceFixture(object):
    def __init__(self):
        m_top_dir = TempDir("DataSync_test")
        m_env = TempEnv()
        m_env["WORKSPACE"] = os.path.join(m_top_dir.path(), "workspace")

def theDependencyConfig():
    return "ElementsServices/testdata/test_file_list.txt"


def theLocalFiles():
    return ["file1.txt", "file2.txt", "dir/file3.txt", "dir/file4.txt", "file5.txt"]


def theInvalidDependencyConfig():
    return "ElementsServices/testdata/invalid_test_file_list.txt"


def theLocalWorkspace():
    return "/tmp"


def absPath(rel_path):
    return concatenatePaths([localWorkspacePrefix(), theLocalWorkspace(), rel_path])

def thePrefixedLocalWorkspace():
    return concatenatePaths([localWorkspacePrefix(), theLocalWorkspace()])


def theIrodsFrConfig():
    return "ElementsServices/testdata/sync_fr_irods.conf"


def theWebdavFrConfig():
    return "ElementsServices/testdata/sync_fr_webdav.conf"


def theWebdavEsConfig():
    return "ElementsServices/testdata/sync_es_webdav.conf"


def theNoOverwriteConfig():
    return "ElementsServices/testdata/sync_fr_webdav_nooverwrite.conf"


def aBadConnectionConfig():
    return "ElementsServices/testdata/sync_wrong.conf"
