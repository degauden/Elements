#!/usr/bin/env python

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
args = parser.parse_args()

if not os.path.exists(args.outdir):
    os.makedirs(args.outdir)
if not os.path.isdir(args.outdir):
    print 'Cannot create output directory', args.outdir
    exit(1)

template = """\
#!/usr/bin/env python
# Automatically generated file: do not modify!

is_installed = False

import sys, os

def _updateSysPath(extra_list):
    if extra_list:
        os_env_str = os.environ.get("PYTHONPATH", None)
        if os_env_str:
            sys_path_str = os.pathsep.join(sys.path)
            found_idx = sys_path_str.find(os_env_str)
            size_of_env = len(os_env_str)
            new_sys_path_str = sys_path_str[:found_idx+size_of_env]
            new_sys_path_str += os.pathsep + os.pathsep.join(extra_list)
            new_sys_path_str += sys_path_str[found_idx+size_of_env:]
            sys.path = new_sys_path_str.split(os.pathsep)
        else:
            sys.path = extra_list + sys.path

old_path = sys.path[:]

# insert neighbour python path after the env variable in sys.path
update_list = []
close_python_dir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), "python")
update_list.append(close_python_dir)
_updateSysPath(update_list)

from ThisProject import THIS_PROJECT_VERSION_STRING, THIS_PROJECT_NAME
from ThisProject import THIS_PROJECT_SEARCH_DIRS

sys.path = old_path

# insert python path list after the env variable in sys.path
_updateSysPath(update_list + [os.path.join(p, "python") for p in THIS_PROJECT_SEARCH_DIRS[1:]])

from ElementsKernel.Program import Program

p = Program('%(MODULE_NAME)s', THIS_PROJECT_VERSION_STRING, THIS_PROJECT_NAME, THIS_PROJECT_SEARCH_DIRS, os.path.realpath(__file__))
exit(p.runProgram())
""" % { 'MODULE_NAME' : args.module }

filename = os.path.join(args.outdir, args.execname)

with open(filename, 'w') as f:
    f.write(template)
os.chmod(filename, os.stat(filename).st_mode | stat.S_IEXEC)
