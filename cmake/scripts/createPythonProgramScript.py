import argparse
import os
import stat

parser = argparse.ArgumentParser()

parser.add_argument('--module', required=True,
                    help='The module containing the program implementation')

parser.add_argument(
    '--outdir', required=True, help='The directory to generate the script in')

parser.add_argument('--execname', required=True,
                    help='The name of the executable script to generate')

parser.add_argument('--local-python-path',
                    help='The local PYTHONPATH to the sources')
parser.add_argument('--project-name',
                    help='The name of the project of the script')

parser.add_argument('--elements-module-name',
                    help='The name of the Elements module of the script')

parser.add_argument('--elements-module-version',
                    help='The name of the Elements module of the script')

parser.add_argument('--python-explicit-version', default="",
                    help='the version of python used in the shebang line')

parser.add_argument('--elements-default-loglevel', default="DEBUG",
                    help='default log level for the Elements framework')

parser.add_argument('--no-config-file', default=False, action="store_true",
                    help='default log level for the Elements framework')


args = parser.parse_args()

if not os.path.exists(args.outdir):
    os.makedirs(args.outdir)
if not os.path.isdir(args.outdir):
    print('Cannot create output directory', args.outdir)
    exit(1)

if args.no_config_file:
    use_config_file_string = "False"
else:
    use_config_file_string = "True"

template = """\
#!/usr/bin/env python%(Python_version)s
# Automatically generated file: do not modify!

is_installed = False

import sys, os
import logging

# This function is embedded to allow standalone execution.
def _updateSysPath(extra_list):
    if extra_list:
        os_env_str = os.environ.get("PYTHONPATH", None)
        if os_env_str:
            sys_path_str = os.pathsep.join(sys.path)
            found_idx = sys_path_str.find(os_env_str)
            new_sys_path_str = sys_path_str
            if found_idx != -1:
                size_of_env = len(os_env_str)
                new_sys_path_str = sys_path_str[:found_idx+size_of_env]
                new_sys_path_str += os.pathsep + os.pathsep.join(extra_list)
                new_sys_path_str += sys_path_str[found_idx+size_of_env:]
            else:
                new_sys_path_str += os.pathsep + os.pathsep.join(extra_list)
            sys.path = new_sys_path_str.split(os.pathsep)
        else:
            sys.path = extra_list + sys.path

old_path = sys.path[:]

# insert neighbour python path after the env variable in sys.path
update_list = []
close_python_dir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), "python")
update_list.append(close_python_dir)
_updateSysPath(update_list)

from %(proj)s_VERSION import %(proj)s_VERSION_STRING, %(proj)s_VCS_VERSION
from %(proj)s_INSTALL import %(proj)s_SEARCH_DIRS

%(proj)s_NAME = "%(Proj)s"

ELEMENTS_MODULE_NAME = "%(Mod_name)s"
ELEMENTS_MODULE_VERSION = "%(Mod_version)s"

sys.path = old_path

# insert python path list after the env variable in sys.path
_updateSysPath(update_list + [os.path.join(p, "python") for p in %(proj)s_SEARCH_DIRS[1:]])

from ElementsKernel.Program import Program

p = Program('%(MODULE_NAME)s',
             %(proj)s_VERSION_STRING, %(proj)s_NAME,
             %(proj)s_VCS_VERSION,
             ELEMENTS_MODULE_NAME, ELEMENTS_MODULE_VERSION,
             %(proj)s_SEARCH_DIRS, os.path.realpath(__file__),
             logging.%(LogLevel)s,
             use_config_file=%(UseConfigFile)s)

exit(p.runProgram())
""" % {'MODULE_NAME' : args.module,
       'proj' : args.project_name.upper(),
       'Proj' : args.project_name,
       'Mod_name' : args.elements_module_name,
       'Mod_version' : args.elements_module_version,
       'Python_version': args.python_explicit_version,
       'LogLevel': args.elements_default_loglevel,
       'UseConfigFile': use_config_file_string
      }

filename = os.path.join(args.outdir, args.execname)

with open(filename, 'w') as f:
    f.write(template)
os.chmod(filename, os.stat(filename).st_mode | stat.S_IEXEC)
