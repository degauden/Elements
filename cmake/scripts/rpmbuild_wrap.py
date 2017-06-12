from subprocess import call
from os.path import dirname

import sys

if __name__ == '__main__':
    if len(sys.argv) < 2:
        sys.exit("Not enough arguments")

    spec_file = sys.argv[-1]
    extra_args = sys.argv[1:-1]
    rpm_root = dirname(dirname(spec_file))

    if rpm_root:
        print(rpm_root)
        cmd = 'rpmbuild %s -ba %s' % (" ".join(extra_args), spec_file)
        status = call(cmd, shell=True)
        sys.exit(status)
