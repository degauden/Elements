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


from ElementsServices.DataSync import IrodsSynchronizer

from fixtures.ConfigFilesFixture import theIrodsFrConfig
from fixtures.TestDataSynchronizer import TestDataSynchronizer


class TestIrodsSynchronizer(TestDataSynchronizer):

    # TODO test_irodsGetCmd

    def test_irodsFixture(self):
        if not IrodsSynchronizer.irodsIsInstalled():
            return
        irods_fr = theIrodsFrConfig()
        self.checkSynchronization(irods_fr)
        self.checkDownloadTestData(irods_fr)

    def test_downloadErrorReport(self):
        if not IrodsSynchronizer.irodsIsInstalled():
            return
        irods_fr = theIrodsFrConfig()
        self.checkDownloadErrorReport(irods_fr)
