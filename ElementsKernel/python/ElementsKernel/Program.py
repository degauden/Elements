import importlib
import os
import sys
import re
import ElementsKernel.Logging as log

class Program:
    
    def __init__(self, app_module):
        self._app_module = importlib.import_module(app_module)
        self._logger = log.getLogger('ElementsProgram')
    
    def _setupLogging(self, arg_parser):
        options = arg_parser.parse_known_args()[0]
        if options.log_level:
            log.setLevel(options.log_level.upper())
        if options.log_file:
            log.setLogFile(options.log_file)
    
    def _findConfigFile(self):
        name = os.sep + self._app_module.__name__.replace('.', os.sep) + '.conf'
        conf_file = None
        for conf_path in os.environ.get('ELEMENTS_CONF_PATH').split(':'):
            if os.path.isfile(conf_path + name):
                conf_file = conf_path + name
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
                    if key.replace('-','_') in [k for (k,v) in vars(cmd_options).iteritems() if v]:
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
        group.add_argument('--config-file', help='Name of a configuration file')
        group.add_argument('--log-level', help='Log level: FATAL, ERROR, WARN, INFO (default), DEBUG')
        group.add_argument('--log-file', help='Name of a log file')
        # Setup the logging
        self._setupLogging(arg_parser)
        # First we get any options from the command line
        cmd_options = arg_parser.parse_known_args()[0]
        # Now redo the parsing including the configuration file
        options = sys.argv[1:]
        options.extend(self._parseConfigFile(arg_parser, cmd_options))
        all_options = arg_parser.parse_args(options)
        return all_options
    
    def _logAllOptions(self, args):
        self._logger.info("##########################################################")
        self._logger.info("##########################################################")
        self._logger.info("#")
        self._logger.info("#    %s start ", self._app_module.__name__)
        self._logger.info("#")
        self._logger.info("##########################################################")
        self._logger.info("#")
        self._logger.info("# List of all program options")
        self._logger.info("# ---------------------------")
        self._logger.info("#")
        for (name,value) in [opt for opt in vars(args).iteritems() if opt[1]]:
            self._logger.info(name.replace('_','-') + ' = ' + str(value))
        self._logger.info("#")
    
    def runProgram(self):
        args = self._parseParameters()
        self._logAllOptions(args)
        exit_code = 1
        try:
            exit_code = self._app_module.mainMethod(args)
        except:
            self._logger.exception(sys.exc_info()[1])
        return exit_code