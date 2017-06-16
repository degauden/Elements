'''
Created on July 5th, 2016

@author: Hubert Degaudenzi

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

import unittest

import os, sys

from imp import reload

from ElementsKernel.Temporary import TempEnv


def _updateSysPath(extra_list):
    """ function to update the sys.path with the extra_list
        The list has to be placed after the PYTHONPATH entries.
    """
    if extra_list:
        os_env_str = os.environ.get("PYTHONPATH", None)
        if os_env_str:
            sys_path_str = os.pathsep.join(sys.path)
            found_idx = sys_path_str.find(os_env_str)
            size_of_env = len(os_env_str)
            new_sys_path_str = sys_path_str[:found_idx + size_of_env]
            new_sys_path_str += os.pathsep + os.pathsep.join(extra_list)
            new_sys_path_str += sys_path_str[found_idx + size_of_env:]
            sys.path = new_sys_path_str.split(os.pathsep)
        else:
            sys.path = extra_list + sys.path


class BoostrapCase(unittest.TestCase):

    def setUp(self):
        unittest.TestCase.setUp(self)
        self._py_path_name = "PYTHONPATH"
        self._extra_list = ["/tmp/toto", "/tmp/tata"]
        self._pyth_path_list = ["/tmp/foo", "/tmp/bar"]

    def tearDown(self):
        unittest.TestCase.tearDown(self)

    def testNoPythonPathUpdate(self):

        # create temporary environment
        e = TempEnv()

        # ensure that there is no PYTHONPATH
        # in the environment.
        if self._py_path_name in e:
            del e[self._py_path_name]

        # ensure that sys.path is updated with the
        # new environment
        import site
        reload(site)

        orig_sys_path = sys.path

        _updateSysPath(self._extra_list)

        new_sys_path = sys.path

        self.assert_(self._extra_list + orig_sys_path == new_sys_path)


if __name__ == '__main__':
    unittest.main()
