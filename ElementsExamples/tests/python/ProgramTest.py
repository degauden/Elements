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

__updated__ = "2018-05-29"

import os
from subprocess import Popen, PIPE
import shlex
import unittest
import pytest

from ElementsKernel.Program import Program
from ElementsKernel.Auxiliary import getAuxiliaryPath


class TestProgramTest(object):

    def test_that_instance_can_be_created_without_raising_exception(self):
        _ = Program("ElementsExamples.PythonProgramExample")

    def test_that_instance_can_be_created_without_raising_exception_for_a_blank_original_path(self):
        _ = Program("ElementsExamples.PythonProgramExample", original_path="")

    @pytest.mark.parametrize(
        "default, default_conf, cmd_line_conf, cmd_line_arg, arg_name, expected",
        [
            ("default", None,           None,            None,         "int-option-with-default-no-default-in-conf",  444),
            ("default", "default_conf", None,            None,         "int-option-with-default-and-default-in-conf", 333),
            ("default", "default_conf", "cmd_line_conf", None,         "int-option-with-default-and-default-in-conf", 777),
            ("default", "default_conf", "cmd_line_conf", "regular",    "int-option-with-default-and-default-in-conf", 666),
            ("default", "default_conf", "cmd_line_conf", "with_space", "int-option-with-default-and-default-in-conf", 666),
            ("default", "default_conf", None,            "regular",    "int-option-with-default-and-default-in-conf", 666),
            ("default", "default_conf", None,            "with_space", "int-option-with-default-and-default-in-conf", 666),
            ("default", None,           "cmd_line_conf", None,         "int-option-with-default-no-default-in-conf",  888),
            ("default", None,           "cmd_line_conf", "regular",    "int-option-with-default-no-default-in-conf",  666),
            ("default", None,           "cmd_line_conf", "with_space", "int-option-with-default-no-default-in-conf",  666),
            ("default", None,           None,            "regular",    "int-option-with-default-no-default-in-conf",  666),
            ("default", None,           None,            "with_space", "int-option-with-default-no-default-in-conf",  666),
            (None,      "default_conf", None,            None,         "int-option",                                    3),
            (None,      "default_conf", "cmd_line_conf", None,         "int-option",                                    4),
            (None,      "default_conf", "cmd_line_conf", "regular",    "int-option",                                  666),
            (None,      "default_conf", "cmd_line_conf", "with_space", "int-option",                                  666),
            (None,      "default_conf", None,            "regular",    "int-option",                                  666),
            (None,      "default_conf", None,            "with_space", "int-option",                                  666),
            (None,      None,           "cmd_line_conf", None,         "int-option-no-default-not-defined-in-conf",   555),
            (None,      None,           "cmd_line_conf", "regular",    "int-option-no-default-not-defined-in-conf",   666),
            (None,      None,           "cmd_line_conf", "with_space", "int-option-no-default-not-defined-in-conf",   666),
            (None,      None,           None,            "regular",    "int-option-with-no-defaults-anywhere",        666),
            (None,      None,           None,            "with_space", "int-option-with-no-defaults-anywhere",        666),
        ]
    )
    def test_that_parameter_precedence_is_respected(self,
                                                    default,
                                                    default_conf,
                                                    cmd_line_conf,
                                                    cmd_line_arg,
                                                    arg_name,
                                                    expected):

        test_program_script = 'PythonProgramExample'

        cmd = test_program_script

        if cmd_line_conf is not None:
            cmd_line_conf_fpath = getAuxiliaryPath("ElementsExamples/tests/conf/%s.conf" % test_program_script)
            cmd = '{} --config-file={}'.format(cmd, cmd_line_conf_fpath)

        if cmd_line_arg is not None:
            if cmd_line_arg == "regular":
                cmd = '{} --{}=666'.format(cmd, arg_name)
            else:
                cmd = '{} --{} 666'.format(cmd, arg_name)
                

        process = Popen(shlex.split(cmd), stdout=PIPE, stderr=PIPE)
        _, stderr = process.communicate()
        output_arg_line = list(
            filter(
                lambda line: ' {} = '.format(arg_name) in line,
                stderr.decode().splitlines()
            )
        )

        assert len(output_arg_line) == 1, 'the argument {} should appear exactly once'.format(arg_name)

        output_arg_value = int(output_arg_line.pop().split('=')[-1])

        msg = (
            'The command\n\t{}\nfailed while checking for the value of argument\n\t{}\n'
            'The obtained value is "{}" while the expected value is "{}"'
        ).format(
            cmd,
            arg_name,
            output_arg_value,
            expected
        )
        assert output_arg_value == expected, msg


if __name__ == "__main__":
    unittest.main()
