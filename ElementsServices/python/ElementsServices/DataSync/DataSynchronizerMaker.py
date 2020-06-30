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

from .ConnectionConfiguration import DataHost
from .IrodsSynchronizer import IrodsSynchronizer
from .WebdavSynchronizer import WebdavSynchronizer


synchronizerRegister = {
    DataHost.IRODS: IrodsSynchronizer,
    DataHost.WEBDAV: WebdavSynchronizer,
}


def createSynchronizer (
        connection_config,
        dependency_config):
    """Create the appropriate data synchronizer
    as specified in a given connection configuration.
    """
    host = connection_config.host
    if host == DataHost.IRODS:
        return IrodsSynchronizer(connection_config, dependency_config)
    elif host == DataHost.WEBDAV:
        return WebdavSynchronizer(connection_config, dependency_config)
    else:
        raise NotImplementedError("Unknown test data host.")
    #TODO use register: synchronizerRegister[host](connectionConfiguration, dependencyConfig)
