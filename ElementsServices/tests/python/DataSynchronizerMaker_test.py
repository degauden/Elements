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


import py.test

from ElementsServices.DataSync import \
    DataHost, ConnectionConfiguration, DependencyConfiguration, \
    IrodsSynchronizer, WebdavSynchronizer, createSynchronizer
import ElementsServices.DataSync.DataSynchronizerMaker

from fixtures.ConfigFilesFixture import *

class TestDataSynchronizerMaker(object):

    def checkSynchronizerCreation(self, connectionConfig:str, expectedSynchronizer:object):
        connection = ConnectionConfiguration(connectionConfig)
        dependency = DependencyConfiguration(theDependencyConfig(), connection.localRoot)
        synchronizer = createSynchronizer(connection, dependency)
        assert type(synchronizer), expectedSynchronizer

    def testSynchronizerCreation(self):
        webdavConfigs = {
            theWebdavFrConfig(): WebdavSynchronizer,
        }
        irodsConfigs = {
            theIrodsFrConfig(): IrodsSynchronizer
        }
        if not IrodsSynchronizer.irodsIsInstalled():
            irodsConfigs = {}
            #TODO ugly...
        dssConfigs = {}
        configs = {**webdavConfigs, **irodsConfigs, **dssConfigs}
        for config, expected in configs.items():
            self.checkSynchronizerCreation(config, expected)
