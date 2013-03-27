#!/usr/bin/env python
"""
Test that the there is no signature for MsgStream operator<< <char []> in
GaudiKernel.
See https://savannah.cern.ch/bugs?87340
"""
import os
import sys
import re

from subprocess import Popen, PIPE

# find libGaudiKernel.so in the LD_LIBRARY_PATH
libname = 'libGaudiKernel.so'

searchpath = [os.path.curdir, 'lib']
# The day we can run the test on other platforms we can do this:
#varname = {'darwin': 'DYLD_LIBRARY_PATH',
#           'win32': 'PATH'}.get(sys.platform, 'LD_LIBRARY_PATH')
varname = 'LD_LIBRARY_PATH'
searchpath.extend(os.environ.get(varname, "").split(os.pathsep))

try:
    lib = (p
           for p in (os.path.join(n, libname)
                     for n in searchpath)
           if os.path.exists(p)).next()
except StopIteration:
    print >> sys.stderr, 'FAILURE: Cannot find', repr(libname), 'in', searchpath
    sys.exit(2)

nm = Popen(["nm", '-C', lib], stdout=PIPE)
output, _ = nm.communicate()

if nm.returncode:
    print output
    print >> sys.stderr, 'FAILURE: nm call failed'
    sys.exit(nm.returncode)

signature = re.compile(r"MsgStream&amp; operator&lt;&lt; &lt;char \[\d+\]&gt;")

lines = filter(signature.search, output.splitlines())
if lines:
    print "\n".join(lines)
    print >> sys.stderr, "FAILURE: found MsgStream operator<< specialization"
    sys.exit(1)
else:
    print "SUCCESS: no MsgStream operator<< specialization found"