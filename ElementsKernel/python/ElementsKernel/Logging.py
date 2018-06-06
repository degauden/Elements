'''
@file ElementsKernel/Logging.py

@date Apr 29, 2016

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


import logging

# Check we are running for the first time. If we are we need to change a bit the
# naming of the logging levels to match the ones of the C++
if logging.getLevelName(logging.CRITICAL) == 'CRITICAL':
    logging.addLevelName(50, 'FATAL')
    logging.addLevelName(40, 'ERROR')
    logging.addLevelName(30, 'WARN')
    logging.addLevelName(20, 'INFO')
    logging.addLevelName(10, 'DEBUG')

_formatter = logging.Formatter(fmt='%(asctime)s %(name)s %(levelname)5s : %(message)s', datefmt='%Y-%m-%dT%X%Z')

# Check if we have the console logger set already. This might happen if the
# user reloads the module using the reload() method.
if not [h for h in logging.getLogger().handlers if h.get_name() == 'console']:
    # Set the default logging level
    logging.getLogger().setLevel(logging.INFO)
    # Add the stderr log handler
    _console = logging.StreamHandler()
    _console.set_name('console')
    _console.setFormatter(_formatter)
    logging.getLogger().addHandler(_console)

def getLogger(name=None):
    """ Forward call to logging.getLogger"""
    return logging.getLogger(name)

def setLevel(level):
    """ Forward call to logging.getLogger().setLevel"""
    logging.getLogger().setLevel(level)

def setLogFile(filename):
    """Set the log file name"""
    # If we already have a file appenders remove them
    for hd in [hl for hl in logging.getLogger().handlers if hl.get_name() == 'file']:
        logging.getLogger().removeHandler(hd)
        hd.close()
    # Add the new file handler
    hd = logging.FileHandler(filename)
    hd.set_name('file')
    hd.setFormatter(_formatter)
    logging.getLogger().addHandler(hd)
