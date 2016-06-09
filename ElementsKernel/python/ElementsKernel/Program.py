"""Main Program Class Module"""

import importlib
import os
import sys
import re
import ElementsKernel.Logging as log


class Program(object):
    """Main Program Class"""
    def __init__(self, app_module, parent_project_version=None, parent_project_name=None,
                 search_dirs=None):
        self._app_module = importlib.import_module(app_module)
        self._logger = log.getLogger('ElementsProgram')
        self._parent_project_version = parent_project_version
        self._parent_project_name = parent_project_name
        self._search_dirs = search_dirs

    @staticmethod
    def _setupLogging(arg_parser):
        options = arg_parser.parse_known_args()[0]
        if options.log_level:
            log.setLevel(options.log_level.upper())
        if options.log_file:
            log.setLogFile(options.log_file)

    def _findConfigFile(self):
        # Create the path which represents the package of the module (if any)
        rel_path = ''
        if '.' in self._app_module.__name__:
            rel_path = self._app_module.__name__[
                :self._app_module.__name__.index('.')]
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

    def _parseConfigFile(self, arg_parser, cmd_options):
        # First we check if the user gave the --config-file option
        config_file = arg_parser.parse_known_args()[0].config_file
        if not config_file:
            config_file = self._findConfigFile()
        conf = []
        if config_file:
            with open(config_file) as f:
                for line in f.readlines():
                    line = line.strip()
                    if line.startswith('#') or not '=' in line:
                        continue
                    key, value = line.split('=', 1)
                    key = key.strip()
                    if key.replace('-', '_') in [k for (k, v) in vars(cmd_options).iteritems() if v]:
                        continue
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
        group.add_argument(
            '--config-file', help='Name of a configuration file')
        group.add_argument('--log-file', help='Name of a log file')
        group.add_argument(
            '--log-level', help='Log level: FATAL, ERROR, WARN, INFO (default), DEBUG')
        group.add_argument(
            '--version', action='version', version=self.getVersion())
        # Setup the logging
        self._setupLogging(arg_parser)
        # First we get any options from the command line
        cmd_options = arg_parser.parse_known_args()[0]
        # Now redo the parsing including the configuration file
        options = sys.argv[1:]
        options.extend(self._parseConfigFile(arg_parser, cmd_options))
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

    def _logAllOptions(self, args, names):
        self._logger.info(
            "##########################################################")
        self._logger.info(
            "##########################################################")
        self._logger.info("#")
        self._logger.info(
            "#    Python program: %s starts ", self._app_module.__name__)
        self._logger.info("#")
        self._logger.info(
            "##########################################################")
        self._logger.info("#")
        self._logger.info("# List of all program options")
        self._logger.info("# ---------------------------")
        self._logger.info("#")
        # for (name,value) in [opt for opt in vars(args).iteritems() if
        # opt[1]]:
        for name, value in [opt for opt in vars(args).iteritems()]:
            self._logger.info(names[name] + ' = ' + str(value))
        self._logger.info("#")

    def getVersion(self):
        version = ""
        if self._parent_project_name:
            version += self._parent_project_name + " "
        if self._parent_project_version:
            version += self._parent_project_version
        return version

    def runProgram(self):
        args, names = self._parseParameters()
        self._logAllOptions(args, names)
        exit_code = 1
        try:
            exit_code = self._app_module.mainMethod(args)
        except:
            self._logger.exception(sys.exc_info()[1])
        return exit_code
