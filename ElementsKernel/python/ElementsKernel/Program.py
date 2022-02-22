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

"""Main Program Class Module"""

import importlib
import os
import sys
import re
from ElementsKernel import Logging
import logging
from ElementsKernel.Path import VARIABLE, SUFFIXES, joinPath, multiPathAppend
from ElementsKernel.Environment import Environment
from ElementsKernel.Configuration import getConfigurationPath, getConfigurationLocations
from ElementsKernel import Exit


def str_to_bool(s):
    """Convert string to bool (in argparse context)."""
    if s.lower() not in ['true', 'false']:
        raise ValueError('Need bool; got %r' % s)
    return {'true': True, 'false': False}[s.lower()]


class Program(object):
    """Main Program Class"""

    def __init__(self, app_module,
                 parent_project_version=None, parent_project_name=None,
                 parent_project_vcs_version=None,
                 elements_module_name=None, elements_module_version=None,
                 search_dirs=None, original_path="",
                 elements_loglevel=logging.DEBUG,
                 use_config_file=True):
        self._app_module = importlib.import_module(app_module)
        self._logger = Logging.getLogger('ElementsProgram')
        self._elements_loglevel = elements_loglevel
        self._use_config_file = use_config_file
        self._parent_project_version = parent_project_version
        self._parent_project_name = parent_project_name
        self._parent_project_vcs_version = parent_project_vcs_version
        self._elements_module_name = elements_module_name
        self._elements_module_version = elements_module_version
        self._search_dirs = search_dirs
        self._program_path = os.path.dirname(original_path)
        self._program_name = os.path.basename(original_path)
        self._env = Environment()

    @staticmethod
    def _setupLogging(arg_parser):
        options = arg_parser.parse_known_args()[0]
        if options.log_level:
            Logging.setLevel(options.log_level.upper())
        if options.log_file:
            Logging.setLogFile(options.log_file)

    def _findConfigFile(self):
        # Create the path which represents the package of the module (if any)
        rel_path = ''
        if '.' in self._app_module.__name__:
            rel_path = self._app_module.__name__[:self._app_module.__name__.index('.')]
            rel_path = rel_path.replace('.', os.sep)
        # Get the name of the executable, remove the prefix and change the
        # extension to .conf
        name = os.path.splitext(os.path.basename(sys.argv[0]))[0] + '.conf'
        if rel_path:
            rel_path = rel_path + os.sep + name
        else:
            rel_path = name
        conf_file = None
        for conf_path in os.environ.get('ELEMENTS_CONF_PATH').split(os.pathsep):
            if os.path.isfile(conf_path + os.sep + rel_path):
                conf_file = conf_path + os.sep + rel_path
                break
        return conf_file

    def getDefaultConfigFile(self, program_name, module_name):

        conf_name = os.path.splitext(program_name)[0] + '.conf'

        default_config_file = getConfigurationPath(conf_name, False)

        if not default_config_file:
            self._logger.warning('The "%s" configuration file cannot be found in:', conf_name)
            for l in getConfigurationLocations():
                self._logger.warning(" %s", l)
            if not module_name and '.' in self._app_module.__name__:
                module_name = self._app_module.__name__[:self._app_module.__name__.index('.')]
                module_name = module_name.replace('.', os.sep)
            if module_name:
                conf_name = os.sep.join([module_name, conf_name])
                self._logger.warning('Trying "%s".', conf_name)
                default_config_file = getConfigurationPath(conf_name, False)

        if not default_config_file:
            self._logger.debug('Couldn\'t find "%s" configuration file.', conf_name)
        else:
            self._logger.debug('Found "%s" configuration file at %s', conf_name, default_config_file)

        return default_config_file

    def _parseConfigFile(self, arg_parser):
        # First we check if the user gave the --config-file option
        config_file = arg_parser.parse_known_args()[0].config_file
        if not config_file:
            config_file = self.getDefaultConfigFile(self._program_name,
                                                    self._elements_module_name)
        conf = []
        if config_file:
            with open(config_file) as f:
                for line in f.readlines():
                    line = line.strip()
                    if line.startswith('#') or not '=' in line:
                        continue
                    key, value = line.split('=', 1)
                    key = key.strip()
                    # If the key is not mapping to any of the actions defined in
                    # the parser, fail with an error messsage
                    if not [act for act in arg_parser._actions if ('--' + key) in act.option_strings]:
                        self._logger.error('Unknown option "{}" in configuration file {}'.format(key, config_file))
                        exit(Exit.Code["NOT_OK"])
                    value = value.strip()
                    if '#' in value:
                        value = value[:value.find('#')]
                    conf.append('--' + key)
                    for v in re.split(r'''((?:[^ "']|"[^"]*"|'[^']*')+)''', value)[1::2]:
                        conf.append(v)
        return conf

    def _parseParameters(self):
        # Get the argument parser with the user options
        arg_parser = self._app_module.defineSpecificProgramOptions()
        # Add all the options which are common to all the programs
        group = arg_parser.add_argument_group('Generic Options')
        if self._use_config_file:
            group.add_argument(
                '--config-file', help='Name of a configuration file')
        group.add_argument('--log-file', help='Name of a log file')
        group.add_argument(
            '--log-level', help='Log level: FATAL, ERROR, WARN, INFO (default), DEBUG')
        group.add_argument(
            '--version', action='version', version=self.getVersion())
        # Setup the logging
        self._setupLogging(arg_parser)
        # Get the options from the config file
        if self._use_config_file:
            options = self._parseConfigFile(arg_parser)
        else:
            options = []
        # Append any options passed by the user in the command line. Because they
        # are after the ones from the configuration file, they are going to
        # override them (argparse behavior)
        options.extend(sys.argv[1:])
        # Now redo the parsing with all the options
        all_options = arg_parser.parse_args(options)

        # We create a map of the variable names to the option names to be used
        # for further references
        variable_to_option_name = {}
        # Iterate through the names of the variables keeping the option values
        for var in [v for v in dir(all_options) if not v.startswith('_')]:
            # We get the related action from the argparser
            action = None
            for a in arg_parser._actions:
                if a.dest == var and a.option_strings:
                    action = a
                    break

            if action:
                # We chose as name the longest option name and we strip any leading '-'
                variable_to_option_name[var] = max(action.option_strings, key=len).lstrip('-')
            else:
                # This happens if we didn't find an action for this variable or
                # if it didn't have option_strings. In this case we just use the
                # variable name
                variable_to_option_name[var] = var

        return all_options, variable_to_option_name

    def _logHeader(self):
        self._logger.log(self._elements_loglevel,
                         "##########################################################")
        self._logger.log(self._elements_loglevel,
                         "##########################################################")
        self._logger.log(self._elements_loglevel,
                         "#")
        self._logger.log(self._elements_loglevel,
            "#    Python program: %s starts ", self._app_module.__name__)
        self._logger.log(self._elements_loglevel,
                         "#")
        self._logger.debug("# Program Name: %s", self._program_name)
        self._logger.debug("# Program Path: %s", self._program_path)
        self._logger.debug("#")

    def _logFooter(self):
        self._logger.log(self._elements_loglevel,
            "##########################################################")
        self._logger.log(self._elements_loglevel,
                         "#")
        self._logger.log(self._elements_loglevel,
            "#    Python program: %s stops ", self._app_module.__name__)
        self._logger.log(self._elements_loglevel,
                         "#")
        self._logger.log(self._elements_loglevel,
            "##########################################################")
        self._logger.log(self._elements_loglevel,
            "##########################################################")

    def _logAllOptions(self, args, names):

        self._logger.log(self._elements_loglevel,
            "##########################################################")
        self._logger.log(self._elements_loglevel, "#")
        self._logger.log(self._elements_loglevel, "# List of all program options")
        self._logger.log(self._elements_loglevel, "# ---------------------------")
        self._logger.log(self._elements_loglevel, "#")
        for name, value in [opt for opt in vars(args).items()]:
            self._logger.log(self._elements_loglevel, names[name] + ' = ' + str(value))
        self._logger.log(self._elements_loglevel, "#")

    def _logTheEnvironment(self):
        self._logger.debug("##########################################################")
        self._logger.debug("#")
        self._logger.debug("# Environment of the Run")
        self._logger.debug("# ---------------------------")
        self._logger.debug("#")

        for v in VARIABLE:
            self._logger.debug("%s: %s", VARIABLE[v], self._env[VARIABLE[v]])

        self._logger.debug("#")

    def getVersion(self):
        version = ""
        if self._parent_project_name:
            version += self._parent_project_name + " "
        if self._parent_project_vcs_version:
            version += self._parent_project_vcs_version
        return version

    def _bootStrapEnvironment(self):
        self._program_path = os.path.dirname(os.path.realpath(sys.argv[0]))

        local_search_paths = [os.path.realpath(p) for p in self._search_dirs]

        this_parent_path = os.path.dirname(self._program_path)

        if local_search_paths[0] != this_parent_path:
            local_search_paths.insert(0, this_parent_path)

        for name, value in VARIABLE.items():
            if value in os.environ:
                self._env[value] += os.pathsep + joinPath(multiPathAppend(local_search_paths, SUFFIXES[name]))
            else:
                self._env[value] = joinPath(multiPathAppend(local_search_paths, SUFFIXES[name]))

    def _setup(self):

        self._bootStrapEnvironment()

        args, names = self._parseParameters()
        self._logHeader()
        self._logAllOptions(args, names)
        self._logTheEnvironment()
        return args, names

    def _tearDown(self, exit_code):

        if exit_code is not None:
            self._logger.debug("# Exit Code: %d", exit_code)
        self._logFooter()

    def getProgramName(self):
        return self._program_name

    def runProgram(self):

        args, _ = self._setup()

        exit_code = Exit.Code["NOT_OK"]
        try:
            exit_code = self._app_module.mainMethod(args)
        except Exception:
            self._logger.exception(sys.exc_info()[1])

        self._tearDown(exit_code)

        return exit_code
