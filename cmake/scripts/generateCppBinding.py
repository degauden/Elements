""" Script for the C++ generation from the XSD file """

import argparse
import os
import subprocess
from lxml import etree

XSD_EXE = "xsdcxx"
DM_DIR = "datamodel"
NS = {"xs": "http://www.w3.org/2001/XMLSchema"}
INCL_PREF = "euclid/dm"
NS_PREFIX = "http://euclid.esa.org/schema"


def createBaseDirectory(file_path):
    base_dir = os.path.dirname(file_path)
    if base_dir and not os.path.exists(base_dir):
        os.makedirs(base_dir, exist_ok=True)
    return base_dir


def namespaceXml2cpp(xml_ns, namespace_prefix=NS_PREFIX):
    if xml_ns.startswith(namespace_prefix):
        xml_ns = xml_ns[len(namespace_prefix):]
    # remove hyphens that cannot be present in C++ namespaces
    # and turn / into ::
    cpp_ns = xml_ns.replace('-', '').replace('/', '::')
    return cpp_ns


def generate(xsd_file, cpp_file, h_file, d_file,
             xsd_exe=XSD_EXE,
             dm_dir=DM_DIR,
             incl_pref=INCL_PREF,
             namespace_prefix=NS_PREFIX):

    doc = etree.parse(xsd_file)
    root = doc.xpath("//xs:schema", namespaces=NS).pop()
    target_ns = root.attrib.get('targetNamespace', 'nonamespace')
    target_ns_cpp = namespaceXml2cpp(target_ns, namespace_prefix)

    # append cpp namespace map for each xmlns
    nsmaps = []
    nsmaps.append('--namespace-map')
    nsmaps.append('%s=%s' % (target_ns, target_ns_cpp))
    for prefix, ns in root.nsmap.items():
        if ns.startswith(namespace_prefix):
            cpp_ns = namespaceXml2cpp(ns, namespace_prefix)
            nsmaps.append('--namespace-map')
            nsmaps.append("%s=%s" % (ns, cpp_ns))

    # create the output directory if it doesn't exist
    out_dir = createBaseDirectory(cpp_file)

    file_dir = os.path.dirname(xsd_file.replace(dm_dir + '/', ''))

    # Build xsd command
    xsdcxx_command = []
    xsdcxx_command.append(xsd_exe)
    xsdcxx_command.append('cxx-tree')
    xsdcxx_command.append('--std')
    xsdcxx_command.append('c++11')
    xsdcxx_command.append('--hxx-suffix')
    xsdcxx_command.append('.h')
    xsdcxx_command.append('--cxx-suffix')
    xsdcxx_command.append('.cpp')
    xsdcxx_command.append('--include-prefix')
    xsdcxx_command.append(os.path.join(incl_pref, file_dir, ''))
    xsdcxx_command.append('--generate-serialization')
    xsdcxx_command.append('--generate-dep')
    xsdcxx_command.extend(nsmaps)
    xsdcxx_command.append('--output-dir')
    xsdcxx_command.append(out_dir)
    xsdcxx_command.append(xsd_file)
    print('Generating C++ bindings for %s' % xsd_file)
    sp = subprocess.Popen(xsdcxx_command, stdout=subprocess.PIPE,
                          stderr=subprocess.PIPE)
    (stdout, stderr) = sp.communicate()

    rc = sp.returncode
    if rc != 0:
        print('\nError while generating bindings for %s' % xsd_file)
        print(stderr)
    elif stderr:
        print('\nWarning while generating bindings for %s' % xsd_file)
        print(stderr)

    generated_h_file = "%s.h" % os.path.splitext(cpp_file)[0]
    h_dir = createBaseDirectory(h_file)
    if generated_h_file != h_file:
        os.rename(generated_h_file, h_file)
    generated_d_file = "%s.d" % os.path.splitext(cpp_file)[0]
    d_dir = createBaseDirectory(d_file)
    if generated_d_file != d_file:
        os.rename(generated_d_file, d_file)

    return rc


def defineOptions():
    parser = argparse.ArgumentParser(description='Generate C++ binding from XSD')

    parser.add_argument('-e', '--xsd-executable', dest='xsd_exe', action='store', default=XSD_EXE,
                        help='name of C++ XSD generator executable')
    parser.add_argument('-d', '--dm-dir', dest='dm_dir', action='store', default=DM_DIR,
                        help='Datamodel directory (containing .xsd files)')
    parser.add_argument('-I', '--incl-pref', dest='incl_pref', action='store', default=INCL_PREF,
                        help='Prefix for all includes')
    parser.add_argument('-N', '--namespace-prefix', dest='namespace_prefix', action='store', default=NS_PREFIX,
                        help='namespace prefix')

    parser.add_argument('xsd_file', metavar='xsd_file', default=None,
                        help='input XSD file')
    parser.add_argument('-C', '--cpp-file', dest='cpp_file', action='store', default=None,
                        help='Cpp source binding file (for generated sourcefile)')
    parser.add_argument('-H', '--h-file', dest='h_file', action='store', default=None,
                        help='Cpp header binding file (for generated header file)')
    parser.add_argument('-D', '--d-file', dest='d_file', action='store', default=None,
                        help='Cpp dependency binding file')

    return parser


def processOptions(parser):

    args = parser.parse_args()

    xsd_exe = args.xsd_exe
    dm_dir = args.dm_dir
    incl_pref = args.incl_pref
    namespace_prefix = args.namespace_prefix

    xsd_file = args.xsd_file

    if not xsd_file:
        print("There is no XSD input file")
        exit(1)

    xsd_base = os.path.splitext(os.path.basename(xsd_file))[0]

    cpp_file = args.cpp_file

    if not cpp_file:
        cpp_file = "%s.cpp" % xsd_base

    cpp_root = os.path.splitext(cpp_file)[0]

    h_file = args.h_file
    if not h_file:
        h_file = "%s.h" % cpp_root

    d_file = args.d_file
    if not d_file:
        d_file = "%s.d" % cpp_root

    return xsd_exe, dm_dir, incl_pref, namespace_prefix, xsd_file, cpp_file, h_file, d_file


if __name__ == '__main__':

    parser = defineOptions()
    xsd_exe, dm_dir, incl_pref, namespace_prefix, xsd_file, cpp_file, h_file, d_file = processOptions(parser)
    status = generate(xsd_file, cpp_file, h_file, d_file,
                      xsd_exe, dm_dir, incl_pref, namespace_prefix)
    exit(status)
