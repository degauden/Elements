""" Script to transform the CTest XML output into Junit XML """

import StringIO
import sys
import os

from optparse import OptionParser


def main():
    """ main function of this script module """

    from lxml import etree

    parser = OptionParser(
        usage="ERROR: Usage %prog <build_dir> <xsl_file>")
    parser.add_option("-q", "--quiet", action="store_true",
                      help="Do not print messages.")
    opts, args = parser.parse_args()

    if len(args) != 2:
        parser.error("wrong number of arguments")

    build_dir, xsl_file = args
    if not opts.quiet:
        print("Processing %s with %s", build_dir, xsl_file)

    tag_file = os.path.join(build_dir, "Testing", "TAG")
    dir_name = open(tag_file).readline().strip()
    qtest_xml_file = os.path.join(build_dir, "Testing", dir_name, "Test.xml")
    junit_xml_file = os.path.join(build_dir, "Testing", dir_name, "JUnitTest.xml")

    xml_content = open(qtest_xml_file).read()
    xsl_content = open(xsl_file).read()
    xml_doc = etree.parse(StringIO.StringIO(xml_content))
    xslt_root = etree.XML(xsl_content)
    transform = etree.XSLT(xslt_root)
    result_tree = transform(xml_doc)

    open(junit_xml_file, "w").write(etree.tostring(result_tree, pretty_print=True))


if __name__ == '__main__':
    try:
        main()
    except:
        pass