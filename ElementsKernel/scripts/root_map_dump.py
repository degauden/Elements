#!/usr/bin/env python

import os.path
from GaudiKernel.RootMap import getMaps, checkDict
from GaudiKernel.RootMap import printMaps, shortPrintMaps
from GaudiKernel.RootMap import printKeys


def _help(argv):
  print """
  %s [-h] [-s|-e|-c] [-r] [-p pattern] [listofpath]
  
     -h: dump this Help
     -s: Short format print
     -e: print Entry names only
     -c: only Checks duplication of entries
     -r: print also $ROOTSYS/etc/system.rootmap
     -p: print only pattern matching (Python style) entries
     
  dump rootmap dictionaries informations. If no path is given, the
  LD_LIBRARY_PATH is used. By default, prints the name of the entry,
  its corresponding library and its block definition inside the
  rootmap file(s) on separate lines. The output is sorted according to
  the name of the entry. A Warning is issued if an entry appears
  several times.
  """%os.path.basename(argv[0])



if __name__ == "__main__":
  import getopt
  import sys
  printFunc = printMaps
  sysrtmap = False
  pattern = None
  try:
    opts, args = getopt.getopt(sys.argv[1:], 'hsecrp:')
  except getopt.GetoptError:
    _help(sys.argv)
    sys.exit(2)
  for op,ar in opts :
    if op == '-h':
      _help(sys.argv)
      sys.exit(0)
    if op == '-s': printFunc = shortPrintMaps
    if op == '-e': printFunc = printKeys 
    if op == '-c': printFunc = checkDict 
    if op == '-r': sysrtmap  = True
    if op == '-p':
      import re
      pattern = re.compile(ar)
  if args:
    for p in args:
      maps = getMaps(p,sysrtmap)
      printFunc(maps,pattern)
  else:
    maps = getMaps("",sysrtmap)
    printFunc(maps,pattern)
