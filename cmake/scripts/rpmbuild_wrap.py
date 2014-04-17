#!/usr/bin/env python

from subprocess import Popen, PIPE, call
from os.path import dirname

import sys

if __name__ == '__main__':
    if len(sys.argv) < 2 :
        sys.exit("Not enough arguments")
        
    spec_file = sys.argv[1]
    rpm_root = dirname(dirname(spec_file))
    if rpm_root :
        print rpm_root
        cmd = 'rpmbuild --define="_topdir %s" -ba %s' % (rpm_root, spec_file)
        status = call(cmd, shell=True)
        sys.exit(status)