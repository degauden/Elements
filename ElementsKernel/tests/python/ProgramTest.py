'''
Created on May 10, 2017

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
import subprocess
import unittest
import pytest

from ElementsKernel.Program import Program


class TestProgramTest(object):

    def test_that_instance_can_be_created_without_raising_exception(self):
        _ = Program("ElementsExamples.PythonProgramExample")

    def test_that_instance_can_be_created_without_raising_exception_for_a_blank_original_path(self):
        _ = Program("ElementsExamples.PythonProgramExample", original_path="")


    @pytest.mark.parametrize(
        "default, default_conf, cmd_line_conf, cmd_line_arg",
        [
            ("default", None, None, None),
            ("default", "default_conf", None, None),
            ("default", "default_conf", "cmd_line_conf", None),
            ("default", "default_conf", "cmd_line_conf", "cmd_line_param"),
            ("default", "default_conf", None, "cmd_line_param"),
            ("default", None, "cmd_line_conf", None),
            ("default", None, "cmd_line_conf", "cmd_line_param"),
            ("default", None, None, "cmd_line_param"),
            (None, "default_conf", None, None),
            (None, "default_conf", "cmd_line_conf", None),
            (None, "default_conf", "cmd_line_conf", "cmd_line_param"),
            (None, "default_conf", None, "cmd_line_param"),
            (None, None, "cmd_line_conf", None),
            (None, None, "cmd_line_conf", "cmd_line_param"),
            (None, None, None, "cmd_line_param"),
        ]
    )
    def test_that_parameter_precedence_is_respected(self,
                                                    default,
                                                    default_conf,
                                                    cmd_line_conf,
                                                    cmd_line_arg):
        prcedence = {
            "default": 0,
            "default_conf": 1,
            "cmd_line_conf": 2,
            "cmd_line_param": 3
        }
        process = subprocess.Popen("ERun ")


if __name__ == "__main__":
    unittest.main()
