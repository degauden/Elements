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
import os
from subprocess import Popen, PIPE
import shlex
import unittest
import pytest

from ElementsKernel.Program import Program


class TestProgramTest(object):

    def test_that_instance_can_be_created_without_raising_exception(self):
        _ = Program("ElementsExamples.PythonProgramExample")

    def test_that_instance_can_be_created_without_raising_exception_for_a_blank_original_path(self):
        _ = Program("ElementsExamples.PythonProgramExample", original_path="")


    @pytest.mark.parametrize(
        "default, default_conf, cmd_line_conf, cmd_line_arg, expected",
        [
            ("default", None,           None,            None,             111),
            ("default", "default_conf", None,            None,             333),
            ("default", "default_conf", "cmd_line_conf", None,             555),
            ("default", "default_conf", "cmd_line_conf", "cmd_line_param", 666),
            ("default", "default_conf", None,            "cmd_line_param", 666),
            ("default", None,           "cmd_line_conf", None,             555),
            ("default", None,           "cmd_line_conf", "cmd_line_param", 666),
            ("default", None,           None,            "cmd_line_param", 666),
            (None,      "default_conf", None,            None,             333),
            (None,      "default_conf", "cmd_line_conf", None,             555),
            (None,      "default_conf", "cmd_line_conf", "cmd_line_param", 666),
            (None,      "default_conf", None,            "cmd_line_param", 666),
            (None,      None,           "cmd_line_conf", None,             555),
            (None,      None,           "cmd_line_conf", "cmd_line_param", 666),
            (None,      None,           None,            "cmd_line_param", 666),
        ]
    )
    def test_that_parameter_precedence_is_respected(self,
                                                    default,
                                                    default_conf,
                                                    cmd_line_conf,
                                                    cmd_line_arg,
                                                    expected):

        test_program_script = 'PythonProgramExample'

        cmd = test_program_script

        if default is not None and default_conf is None:
            arg_to_test = "int-option-with-default-no-default-in-conf"
        elif default is not None and default_conf is not None:
            arg_to_test = "int-option-with-default-and-default-in-conf"
        elif default is None and default_conf is not None:
            arg_to_test = "int-option"
        elif default is None and default_conf is None:
            arg_to_test = "int-option-no-default-not-defined-in-conf"
        else:
            raise ValueError('unsupported test case')

        if cmd_line_conf is not None:
            cmd_line_conf_fpath = os.path.join(
                os.environ['ELEMENTSKERNELROOT'], 'tests', 'data', 'conf',
                test_program_script + '.conf'
            )
            cmd = '{} --config-file={}'.format(cmd, cmd_line_conf_fpath)

        if cmd_line_arg is not None:
            cmd = '{} --{}=666'.format(cmd, arg_to_test)

        # DEBUG
        print('\n', cmd)

        process = Popen(shlex.split(cmd), stdout=PIPE, stderr=PIPE)
        _, stderr = process.communicate()
        output_arg_line = list(
            filter(
                lambda line: ' {} = '.format(arg_to_test) in line,
                stderr.decode().splitlines()
            )
        )

        assert len(output_arg_line) == 1, 'the argument {} should appear exactly once'.format(arg_to_test)

        output_arg_value = int(output_arg_line.pop().split('=')[-1])

        assert output_arg_value == expected


if __name__ == "__main__":
    unittest.main()
