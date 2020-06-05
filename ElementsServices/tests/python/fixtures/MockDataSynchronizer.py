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

from ElementsServices.DataSync.DataSynchronizer import *

from .ConfigFilesFixture import *


class MockDataSynchronizer (DataSynchronizer):

    def __init__(self,
                 connection=theIrodsFrConfig(),
                 dependency=theDependencyConfig()):
        connection_config = ConnectionConfiguration(connection)
        distant_root = connection_config.distant_root
        local_root = connection_config.local_root
        dependency_config = DependencyConfiguration(
            distant_root, local_root, dependency)
        super(MockDataSynchronizer, self).__init__(connection_config, dependency_config)

    def createDownloadCommand(self,
                              distant_file,
                              local_file):
        cmd = "echo " + distant_file + ";" + local_file
        return cmd
