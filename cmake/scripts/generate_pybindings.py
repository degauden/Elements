#!/usr/bin/env python3
# *-* coding: utf-8 *-*

from optparse import OptionParser
from configparser import ConfigParser
from io import StringIO
import os
import sys
from os import path
import keyword
import re
from os import walk
from lxml import etree
from os import makedirs
import pyxb
import subprocess

# The prefix of the namespace to remove for creating the module name
NS_PREFIX = "http://euclid.esa.org/schema/"
DEFAULT_MODULE = 'euclid.dm'

# Parse the command line parameters.
parser = OptionParser(usage='generate_pybindings.py [options]', description='Generates PyXB stubs for XSD files in the given directory, by using the pyxbgen command.\nExample: generate_pybindings.py -d ./datamodel_root -m euclid.dm -o python -i euc-test.*')
parser.add_option("-d", "--dmdir", help="directory containing the XSD files of the DM", metavar='DIR')
parser.add_option("-m", "--module", help="root module name", default=DEFAULT_MODULE)
parser.add_option("-n", "--namespace-prefix", help="namespace prefix", default=NS_PREFIX)
parser.add_option('-o', '--outdir', help='directory where the stubs will be created [default: current directory]', metavar='DIR', default='generated_bindings')
parser.add_option('-i', '--inregex', help='regex of filenames to create stubs for [default: all files]', metavar='REGEX', default='.*')
parser.add_option('-e', '--exregex', help='regex of filenames to exclude (supersedes -i)', metavar='REGEX')
args = sys.argv[:1]
if os.path.isfile('generate_pybindings.conf'):
    conf_string = '[root]\n' + open('generate_pybindings.conf', 'r').read()
    conf_fp = StringIO.StringIO(conf_string)
    conf_parser = ConfigParser.SafeConfigParser()
    conf_parser.readfp(conf_fp)
    for (key, value) in conf_parser.items(conf_parser.sections()[0]):
        args.append('--' + key + '=' + value)
args.extend(sys.argv[1:])
(options, args) = parser.parse_args(args)

# Check that the user gave a directory
if not options.dmdir:
    print("Please give the DM directory (parameter -d or --dmdir)")
    exit(1)
if options.dmdir and not path.isdir(options.dmdir):
    print("DM directory does not exist")
    print("Exiting...")
    exit(1)

# Check that if the user gave a root module name, it is valid
if options.module:
    for subname in options.module.split('.'):
        if not subname or not re.match("[_A-Za-z][_a-zA-Z0-9]*$", subname) or keyword.iskeyword(subname):
            print("Invalid root module name")
            print("Exiting...")
            exit(1)

# Check that the user gave the output directory and we can create it
if not options.outdir:
    print("Please give output directory (parameter -o or --outdir)")
    exit(1)
if not path.exists(options.outdir):
    makedirs(options.outdir)
if not path.isdir(options.outdir):
    print("Cannot create output directory")
    print("Exiting...")
    exit(1)

# Create a list with all the XSD files in the given directory (recursively)
if options.dmdir:
    xsdFiles = [d + '/' + f for (d, _, l) in walk(options.dmdir) for f in l if (f.endswith('.xsd') or f.endswith('.XSD')) and (not options.inregex or re.match(options.inregex, f)) and (not options.exregex or not re.match(options.exregex, f))]
    if not xsdFiles:
        print('No XSD files found in directory ' + options.dmdir)
        print('Exiting...')
        exit(1)

fileModulePairs = []
for f in xsdFiles:
    doc = etree.parse(f)
    ns = {"xs":"http://www.w3.org/2001/XMLSchema"}
    root = doc.xpath("//xs:schema", namespaces=ns).pop()
    module = root.attrib.get('targetNamespace', 'nonamespace')
    if module.startswith(options.namespace_prefix):
        module = module[len(options.namespace_prefix):]
    module = module.replace('/', '.')
    module = module.replace('-', '_')
    module += '_stub'
    fileModulePairs.append((f, module))

# Create the PyXB command for generating the stubs
pyxbCommand = []
pyxbCommand.append('pyxbgen')
pyxbCommand.append('-r')
pyxbCommand.append('--binding-root=' + options.outdir)
if options.module:
    pyxbCommand.append('--module-prefix=' + options.module)
for x in fileModulePairs:
    pyxbCommand.append('--schema-location=' + x[0])
    pyxbCommand.append('--module=' + x[1])

# Generate the stubs
print('Generating stubs...\n')
ret = subprocess.call(pyxbCommand)

# Add the required lines to all the __init__.py files to avoid module conflicts
initFiles = [d + '/' + f for (d, _, l) in walk(options.outdir) for f in l if (f == '__init__.py')]
for initFile in initFiles:
    with open(initFile, 'r') as initIn:
        initText = initIn.read()
    if not 'extend_path' in initText:
        with open(initFile, 'w') as initOut:
            initOut.write('from pkgutil import extend_path\n')
            initOut.write('__path__ = extend_path(__path__, __name__)\n')
            initOut.write(initText)

# If we have PyXB version 1.2.1 or earlier we need to comment out all the lines
# related with the ModuleRecord variable
pyxbVer = int(pyxb.__version__.replace('.', ''))
if (pyxbVer < 122):
    print('\n\nDetected PyXB version < 1.2.2. Applying ModuleRecord hack...')
    hackDir = options.outdir + os.sep
    if (len(options.module) > 0):
        hackDir += options.module.replace('.', os.sep)
    pyFileList = [d + '/' + f for (d, _, l) in walk(hackDir) for f in l if (f.endswith('.py'))]
    for pyFile in pyFileList:
        with open(pyFile, 'r') as sources:
            lines = sources.readlines()
        with open(pyFile, 'w') as sources:
            for line in lines:
                sources.write(re.sub(r'^ModuleRecord', r'#ModuleRecord', line))

print('\nFinished! Look above for any PyXB errors.')

if ret != 0:
     if ret < 0:
         print("Killed by signal", -ret)
     else:
         print("Binding failed ", ret)
         exit(1)
else:
     print("SUCCESS!!")

