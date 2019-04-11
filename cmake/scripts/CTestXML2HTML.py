#!/usr/bin/env python
# -*- coding:utf-8 -*-
import os
import sys
import json
import re
import shutil
import bisect
import time
import base64
import zlib

from datetime import datetime, timedelta
from optparse import OptionParser

try:
    import xml.etree.cElementTree as ET
except ImportError:
    import xml.etree.ElementTree as ET


def cleanXml(xmlFileName):
    """
    Removes xml illegal characters from a file.
    @param xmlFileName: The name of the xml file.
    """
    _illegal_xml_chars_Re = re.compile(
        u'[\x00-\x08\x0b\x0c\x0e-\x1F\uD800-\uDFFF\uFFFE\uFFFF]')

    def hexreplace(match):
        "Return the hex string "
        return "".join(map(hexConvert, match.group()))

    def hexConvert(char):
        return hex(ord(char))

    print(" Trying to repair the xml file")
    print(" Replacing invalid char")
    xmlFile = open(xmlFileName)
    data = xmlFile.read()
    xmlFile.close()
    xmlFile = open(xmlFileName + "temp", 'w')
    xmlFile.write(_illegal_xml_chars_Re.sub(hexreplace, data))
    xmlFile.close()
    try:

        tree = ET.parse(xmlFileName + "temp")
        xmlFile = open(xmlFileName, 'w')
        xmlFile.write(data)
        xmlFile.close()
        del tree
    except:
        print(" Removing invalid char")
        xmlFile = open(xmlFileName, 'w')
        xmlFile.write(_illegal_xml_chars_Re.sub("", data))
        xmlFile.close()


def formatMeasurementText(txt, escape=False, preformat=True):
    '''
    Helper to correctly convert CTest measurements to valid HTML
    (with or without special character escaping and in an optional <pre> tag).
    '''
    from xml.sax.saxutils import escape as escape_xml
    from codecs import encode
    if hasattr(txt, 'decode'):
        txt = txt.decode(errors='ignore')
    txt = encode(txt, 'utf-8', 'xmlcharrefreplace')
    if escape:
        txt = escape_xml(txt)
    if preformat:
        txt = '<pre>{0}</pre>'.format(txt)
    return txt


VALUE_DECODE = {'base64': base64.b64decode}
VALUE_DECOMP = {'gzip': zlib.decompress}


def dropCustomMeasurements(s):
    '''
    Remove custom measurements (sections encompassed by
    '<DartMeasurement ...></DartMeasurement>' tags) from the input string and
    return the new value.
    '''
    pos = s.find(b'<DartMeasurement')
    while pos >= 0:
        end_pos = s.find(b'</DartMeasurement>', pos)
        if end_pos < 0:
            break  # no end tag, better not to drop the section
        s = s[:pos] + s[end_pos + 18:]  # 18 is the size of the end tag
        pos = s.find(b'<DartMeasurement')
    return s


# Some regular expression that is going to be used in the next functions
space_Re = re.compile(r"[ ]")
illegal_web_chars_Re = re.compile(r"[<>\'\"#]")
brackets_Re = re.compile(r"[\[\]\{\}\(\)]")


def cleanWebChar(data):
    """ Removes HTML/JSON special characters """
    return illegal_web_chars_Re.sub("", data)


def cleanSpace(data):
    """ Replace spaces in a string by "_" """
    return space_Re.sub("_", data)


def cleanBrackets(data):
    """ Removes jquery characters """
    return brackets_Re.sub("", data)

# redefinition of timedelta.total_seconds() because it is not present in the 2.6 version


def total_seconds_replacement(timedelta):
    return timedelta.days * 86400 + timedelta.seconds + timedelta.microseconds / 1000000


class TestOrganizer:
    """ A class to produce a ready to use html from tests. It just have to be feed with
        ElementTree element which are Test Element from a Ctest like XML.
    """
    # status translation from cTest xml to the webinterface
    status = {
        "passed": "PASS",
        "failed": "FAIL",
        "skipped": "SKIPPED",
                   "notrun": "SKIPPED",
                   "error": "ERROR",
                   "untested": "UNTESTED"
    }

    def __init__(self, linkreference, fileName):
        """ Initialisation of the class
        @param linkreference: link to the main container which is destinated the html. for
                example : "index.html?test=all_results" This link will be completed to have
                a link to each test
        @param fileName: The name of the file where the html will be stored
        """
        self._root = ET.Element(
            "ul", {"linkreference": linkreference, "class": "topGroup"})
        self.tree = ET.ElementTree(self._root)
        self.seeker = self.elementSeeker()
        self.fileName = fileName
        self._groups = {"Statistics": {}, "GroupList": [], "TestList": []}
        self.statsWritten = False

    def classify(self, testElement):
        """ Classify takes a testElement and add it to the element tree representing the html.
            The element is first classed by package hierarchy and then by alphabetical order.
            @param testElement: The test ET.element taken from a ctest Like xml
            @return: True if the test has been inserted, and false in the other cases.
        """
        if testElement is None:
            return False
        Name = testElement.find("Name").text.lower()
        adress = Name.split(".")
        entry = self._root

        groups = self._groups
        oldGroups = groups

        index = 0
        j = None
        for j in range(len(adress[:-1])):
            if adress[j] not in groups:
                if j > 0:
                    index = bisect.bisect(
                        oldGroups[adress[j - 1]]["GroupList"], adress[j])
                    oldGroups[adress[j - 1]
                              ]["GroupList"].insert(index,  adress[j])
                else:
                    index = bisect.bisect(oldGroups["GroupList"], adress[j])
                    oldGroups["GroupList"].insert(index,  adress[j])

                element = self._addsubgroup(entry, adress[j], j, index)
                groups[adress[j]] = {
                    "Element": element, "Statistics": {}, "GroupList": [], "TestList": []}
            else:
                element = groups[adress[j]]["Element"]

            entry = element
            oldGroups = groups
            groups = groups[adress[j]]

        self._addtest(entry, testElement, groups["TestList"], j)

        return True

    def getHtml(self):
        """ return the html without statistics"""
        return ET.dump(self._root)

    def write(self):
        """ Add statistics and write data to the file. """
        self._addStatistics()
        self.tree.write(self.fileName, "utf-8")

    def _addsubgroup(self, father, sonName, fatherLevel, index):
        """ Add a  html group element and return it
            @param father: the  "group" father html  element.
            @param sonName: The name of the group to be added.
            @param fatherLevel: the hierarchical level of the father.
            @param index: the index which points where the group has to be placed
                    to be in alphabetical order with its brothers
        """

        # find the ul or create it
        ul = self.seeker.findElement(
            {"tag": "ul", "class": "groupContainer"}, father)
        if ul is None:
            ul = ET.Element("ul", {"class": "groupContainer"})
            father.insert(1, ul)
            if fatherLevel != 0:
                ul.set("style", "display:none;")

        element = ET.Element("li", {"id": cleanBrackets(
            sonName) + str(fatherLevel), "class": "group foldable"})
        ul.insert(index, element)
        element.set("linkreference", father.get("linkreference") +
                    "/" + cleanBrackets(sonName) + str(fatherLevel))
        subSpan = ET.SubElement(element, "span", {"class": "clickable folded"})
        subImg = ET.SubElement(subSpan, "img", {"src": "images/plus.png"})
        subImg.text = sonName + ": "

        return element

    def _addtest(self, father, testElement, testList, fatherLevel):
        """ Add a html test element
            @param father: the  "group" father html  element.
            @param testElement: The test ET.element taken from a ctest Like xml
            @param testList: a list of the father's tests to find the correct position
                    where to place the test in alphabetical order.
        """
        container = self.seeker.findElement(
            {"class": "testContainer"}, father, 2)
        if container is None:
            # Make the container
            div = ET.SubElement(father, "div")
            if fatherLevel is not None:
                div.set("style", "display:none;")
            container = ET.SubElement(div, "ul", {"class": "testContainer"})
        # get the testName
        completeTestName = cleanSpace(
            cleanWebChar(testElement.find("Name").text))
        testName = cleanSpace(cleanWebChar(
            testElement.find("Name").text.split(".")[-1]))
        # Prepare the test
        # ,{"index":cleanBrackets(testName.lower())})
        span = ET.Element("span")

        # test
        li = ET.SubElement(span, "li", {"class": "test foldable"})
        span2 = ET.SubElement(li, "span", {"class": "clickable folded"})
        ET.SubElement(span2, "img", {"src": "images/plus.png"})
        ET.SubElement(span2, "span", {"id": cleanBrackets(
            testName), "class": "testid"}).text = testName + ": "

        # direct link
        link = ET.SubElement(li, "a", {"class": "permalink", "href": father.get(
            "linkreference") + "//" + cleanBrackets(testName)})
        ET.SubElement(
            link, "img", {"title": "Direct link to the test", "src": "images/link.png"})

        status = testElement.get("Status")
        if status in self.status:
            status = self.status[status]
        else:
            status = cleanSpace(cleanWebChar(status))

        ET.SubElement(span2, "span", {"class": status}).text = status + " "

        # add status in groups
        groups = self._groups
        adresses = completeTestName.lower().split(".")[0:-1]
        if len(adresses) > 0:
            for adr in adresses:
                if status not in groups[adr]["Statistics"]:
                    groups[adr]["Statistics"][status] = 0
                groups[adr]["Statistics"][status] += 1
                groups = groups[adr]
        # field container
        ul = ET.SubElement(li, "ul", {
                           "class": "fieldid", "testName": completeTestName, "style": "display: none;"})

        self._addField(ul, "start_time")
        self._addField(ul, "end_time")

        # find the well position
        index = bisect.bisect(testList, completeTestName.lower())
        testList.insert(index, completeTestName.lower())

        # insert the test
        container.insert(index, span)

        for namedMeasurment in testElement.find("Results").getiterator("NamedMeasurement"):
            name = namedMeasurment.get("name")
            if name == 'Causes':
                cause = namedMeasurment.find("Value")
                if cause is not None:
                    ET.SubElement(span2, "span", {
                                  "class": "cause"}).text = cause.text + " "
                continue
            fieldName = cleanSpace(cleanWebChar(name)).lower()
            if fieldName in ('end_time', 'start_time', 'qmtest.cause', 'qmtest.target'):
                continue
            self._addField(ul, fieldName)

        self._addField(ul, "stdout")

    def _addField(self, parent, fieldName):
        """ Add a html fiel to a test.
            @param parent: The test html element.
            @param fieldName: The name of the field.
        """
        if parent is not None and fieldName is not None:
            li = ET.Element("li", {"class": "field"})
            li.text = fieldName.lower()
            parent.append(li)
    fieldToAvoidList = ["Statistics", "GroupList", "TestList", "Element"]

    def _addStatistics(self):  # ,groupContainer,fatherAdresses):
        """ add statistics to the html structure."""
        for group in self._groups.iteritems():
            if group[0] not in self.fieldToAvoidList:
                self._addStatistic(group[1])

        self.statsWritten = True

    def _addStatistic(self, group):
        """ intern recursive method to actually write the statistics
            @param group: Group entry in the self._groups dictionnary.
        """
        if not self.statsWritten:
            stats = ET.SubElement(self.seeker.findElement(
                {"tag": "span"}, group["Element"]), "span", {"class": "statistics"})
        else:
            stats = self.seeker.findElement({"tag": "span", "class": "statistics"}, self.seeker.findElement({
                                            "tag": "span"}, group["Element"]))
            if stats is None:
                stats = ET.SubElement(self.seeker.findElement(
                    {"tag": "span"}, group["Element"]), "span", {"class": "statistics"})
            else:
                stats.clear()
                stats.set("class", "statistics")

        for stat in group["Statistics"].iteritems():
            ET.SubElement(stats, "span", {
                          "class": stat[0]}).text = '  ' + stat[0] + ' = ' + str(stat[1])

        # process all the subgroups
        for grp in group.iteritems():
            if grp[0] not in self.fieldToAvoidList:
                self._addStatistic(grp[1])

    def parseXml(self, fileName):
        """ Method to find the test element in a cTest like xml.
            @param fileName: the name of the file to parse
        """
        def cleanXml(xmlFileName):
            """
            Removes xml illegal characters from a file.
            @param xmlFileName: The name of the xml file.
            """
            _illegal_xml_chars_Re = re.compile(
                u'[\x00-\x08\x0b\x0c\x0e-\x1F\uD800-\uDFFF\uFFFE\uFFFF]')
            xmlFile = open(xmlFileName)
            data = xmlFile.read()
            xmlFile.close()
            xmlFile = open(xmlFileName, 'w')
            xmlFile.write(_illegal_xml_chars_Re.sub("", data))
            xmlFile.close()
        try:
            tree = ET.parse(fileName)
        except:
            cleanXml(fileName)
            tree = ET.parse(fileName)
        newdataset = tree.getroot()

        # Look for all the sites ( there is one site per default but
        # as in the specification it can be more, we consider them
        if newdataset.tag == "Site":
            sites = [newdataset]
        else:
            sites = newdataset.getchildren()
        for site in sites:
            print("    -> Process the site: " + site.get("BuildStamp"))
            Testing = site.find("Testing")
            # process each test case
            for Test in Testing.findall("Test"):
                self.classify(Test)

    class elementSeeker:
        """ Class which permit to search into an element tree structure."""

        def __init__(self):
            self.swicthtests = {
                "text": self._testtext,
                "tag": self._testtag,
                "class": self._testclass
            }

        def _testtext(self, element, value):
            return element.text is None or element.text != value

        def _testtag(self, element, value):
            return element.tag is None or element.tag != value

        def _testclass(self, element, value):
            return element.get("class") is None or value not in element.get("class")

        def findElement(self, attributeDictionnary, master, depthMax=-1):
            """
            Recursive function to find an element.Use a depth first algorithm.
            @param attributeDictionnary: A dictionnary of attributes that will be tested
            @param master: the element to search into.
            @param depthMax: The maximum depth to search in. A negative value
                correspond to no limit.
            """
            iterator = master.getchildren()
            if len(iterator) == 0 or depthMax == 0:
                return None
            found = False
            for element in iterator:
                found = True
                for atributes in attributeDictionnary.items():
                    if atributes[0] in self.swicthtests:
                        if self.swicthtests[atributes[0]](element, atributes[1]):
                            found = False
                            break
                    elif element.get(atributes[0]) is None or element.get(atributes[0]) != atributes[1]:
                        found = False
                        break
                if found:

                    return element

            for element in iterator:
                elementFound = self.findElement(
                    attributeDictionnary, element, depthMax - 1)
                if elementFound is not None:
                    return elementFound
            return None


def get_cpuinfo():
    '''
    Return the CPU information of the machine as a list of dictionaries.

    Only Linux is supported.
    '''
    cpuinfo = []
    try:
        current = {}
        for l in open('/proc/cpuinfo'):
            try:
                k, v = map(str.strip, l.split(':', 1))
                if k == 'processor':
                    current = {k: v}
                    cpuinfo.append(current)
                elif k == 'flags':
                    current[k] = v.split()
                else:
                    current[k] = v
            except ValueError:
                pass  # ignore lines without a ':'
    except IOError:
        pass  # ignore missing /proc/cpuinfo
    return cpuinfo


def updateAnnotations(site, Testing, outputTemp, annotationsList):
    """
    Update the global annotation file with the data of the current test.
    @param site: The Site ET.Element object  where the annotation information is stored.
    @param Testing: The Testing ET.Element object where datetime information is stored.
    @param outputTemp: The folder where the annotation file is saved.
    @param annotationsList: A list containing the Annotation dictionary.
        Used as a pointer to this object to allow modification.

    """

    if annotationsList[0] == {}:
        annotationsList.pop(0)
        # create the global annotation file
        annotations = {
            "hostname": cleanWebChar(site.get("Hostname")),
            "end_time": cleanWebChar(Testing.find("EndDateTime").text),
            "end_ts": int(cleanWebChar(Testing.find("EndTestTime").text or '0')),
            "start_time": cleanWebChar(Testing.find("StartDateTime").text),
            "start_ts": int(cleanWebChar(Testing.find("StartTestTime").text or '0')),
            "uname": cleanWebChar(' '.join([site.get("OSName"),
                                            site.get("Name"),
                                            site.get("OSRelease"),
                                            site.get("OSVersion"),
                                            site.get("OSPlatform")])),
            "version": [cleanWebChar(site.get("Generator"))]
        }
        if site.get("qmtest.run.userid") is not None:
            annotations["qmtest.run.userid"] = cleanWebChar(
                site.get("qmtest.run.userid"))
        if site.get("qmtest.run.username") is not None:
            annotations["qmtest.run.username"] = cleanWebChar(
                site.get("qmtest.run.username"))
        if site.get("qmtest.run.command_line") is not None:
            annotations["qmtest.run.command_line"] = [
                cleanWebChar(site.get("qmtest.run.command_line"))]

        # FIXME: we assume the conversion is run on the same machine as the test
        cpuinfo = get_cpuinfo()
        if cpuinfo:
            annotations['cpuinfo'] = [
                'ncpus: {0}'.format(len(cpuinfo)),
                'model name: {0}'.format(cpuinfo[0]['model name']),
                'flags: {0}'.format(' '.join(cpuinfo[0]['flags'])),
            ]

    else:
        annotations = annotationsList.pop(0)
        # update the start time
        startTimeOld = annotations["start_ts"]
        startTimeNew = time.gmtime(int(Testing.find("StartTestTime").text))
        if startTimeNew < startTimeOld:
            annotations["start_time"] = time.strftime(
                "%b %d %H:%M %Z", time.localtime(startTimeNew))
            annotations["start_ts"] = startTimeNew
        # update the end time
        endTimeOld = int(annotations["end_ts"])
        endTimeNew = int(Testing.find("EndTestTime").text or '0')
        delta = endTimeOld - endTimeNew
        if endTimeOld < endTimeNew:
            annotations["end_time"] = time.strftime(
                "%b %d %H:%M %Z", time.localtime(endTimeNew))
            annotations["end_ts"] = endTimeNew
        # adding the generator if new
        if site.get("Generator") not in annotations["version"]:
            annotations["version"].append(cleanWebChar(site.get("Generator")))
        # update qmtest attributes
        if "qmtest.run.userid" not in annotations and site.get("qmtest.run.userid") is not None:
            annotations["qmtest.run.userid"] = cleanWebChar(
                site.get("qmtest.run.userid"))
        if "qmtest.run.username" not in annotations and site.get("qmtest.run.username") is not None:
            annotations["qmtest.run.username"] = cleanWebChar(
                site.get("qmtest.run.username"))
        # if "qmtest.run.command_line" not in annotations and site.get("qmtest.run.command_line") is not None:
        #   annotations["qmtest.run.command_line"] = site.get("qmtest.run.command_line")
        if site.get("qmtest.run.command_line") is not None:
            if "qmtest.run.command_line" not in annotations:
                annotations["qmtest.run.command_line"] = [
                    cleanWebChar(site.get("qmtest.run.command_line"))]
            else:
                annotations["qmtest.run.command_line"].append(
                    cleanWebChar(site.get("qmtest.run.command_line")))

    annotationsList.append(annotations)
    # write in the json file
    globalAnnotationFile = open(os.path.join(
        outputTemp, "annotations.json"), "w")
    globalAnnotationFile.write(json.dumps(
        annotations, sort_keys=True, indent=4))
    globalAnnotationFile.close()


def main():

    #progStartTime = datetime.now()
    # print "program launch at "+progStartTime.strftime("%b %d %H:%M %Z")

    # parse the command line option
    usage = "usage: %prog [options] Convert CTest like XML into a webpage"
    epilog = "Author: Win-Lime Kevin <kevin.win-lime@telecom-bretagne.eu> (22/08/13)."
    parser = OptionParser(usage=usage, epilog=epilog)
    parser.add_option("-i", "--inputFile", action="store", metavar="FILE",
                      help="read the inputFile")
    parser.add_option("-d", "--inputDirectory", action="store",
                      metavar="DIRECTORY",
                      help="read all files ended by Test.xml in the input "
                           "directory. The default value is the active "
                           "directory")
    parser.add_option("-o", "--outputDirectory", action="store",
                      metavar="DIRECTORY",
                      help="Write the result to the output directory. The "
                           "default value is ./html/")
    parser.add_option("-s", "--skeleton", action="store",
                      metavar="SKELETON",
                      help="HTML report skeleton")


    parser.set_defaults(inputDirectory='.', outputDirectory='html')
    (options, args) = parser.parse_args()

    # verify the input file
    inputs = []
    if options.inputFile is not None:
        if not os.path.isfile(options.inputFile):
            print(" The input file is not a valid file")
            exit(1)
        else:
            inputs.append(options.inputFile)
    # verify the input directory and search in it for Test.xml file.
    # only if there is no filename specified. by default, the program search all the file in its (sub)directirie
    if not inputs:
        for r, d, f in os.walk(options.inputDirectory):
            for files in f:
                if files.endswith("Test.xml") and not files.endswith("PyTest.xml"):
                    inputs.append(os.path.join(r, files))

    # close the program if there is no file found
    if inputs == []:
        print(" No file found \n")
        exit()

    print("Converting *Test.xml files from %s to HTML format in %s" % (
        options.inputDirectory, options.outputDirectory))
    # verify or create the output directory
    output = options.outputDirectory
    if not os.path.isdir(output):
        os.makedirs(output)

    # Copy the project build manifest, if present.
    manifest_file = os.path.join(
        options.inputDirectory, 'config', 'manifest.xml')
    if os.path.exists(manifest_file):
        shutil.copy(manifest_file, output)

    # Initialise the annotations
    annotations = [{}]
    # create the summary file
    globalSummaryFile = open(os.path.join(output, "summary.json"), "w")
    globalSummaryFile.write("[]")
    globalSummaryFile.close()

    # Initialisation of the test organizer
    organizer_all = TestOrganizer(
        "index.html?test=all_results", os.path.join(output, "all_results"))
    organizer_failed = TestOrganizer(
        "index.html?test=results", os.path.join(output, "results"))
    failures_count = {'FAIL': 0, 'ERROR': 0}

    # process each file
    for fileName in inputs:
        print("Process the file : " + fileName)
        # read and parse the xml file
        try:
            tree = ET.parse(fileName)
        except:
            cleanXml(fileName)
            tree = ET.parse(fileName)

        newdataset = tree.getroot()

        # Look for all the sites ( there is one site per default but
        # as in the specification it can be more, we consider them
        if newdataset.tag == "Site":
            sites = [newdataset]
        else:
            sites = newdataset.getchildren()
        for site in sites:
            # print "    -> Process the site : "+site.get("BuildStamp")
            Testing = site.find("Testing")
            tests = Testing.findall('Test')
            if not tests:
                # no tests in this "site", let's ignore it
                continue

            # create a directory containing the html file for each site
            # os.path.join(output,os.path.split(fileName)[1][:-8]+str(site.get("BuildStamp")))
            outputTemp = output
            if not os.path.isdir(outputTemp):
                os.makedirs(outputTemp)

            updateAnnotations(site, Testing, outputTemp, annotations)

            """ If the generator is CTest add datetime element to the XML"""
            if "ctest" in site.get("Generator"):
                testStartTime = int(Testing.find("StartTestTime").text)
                # for test in Testing.findall("Test"):

            # find the skeleton directory
            try:
                if options.skeleton is not None:
                    skel = options.skeleton
                else:
                    skel = [os.path.join(d, 'HTMLTestReportSkel')
                            for d in os.environ.get('ELEMENTS_AUX_PATH', '').split(os.pathsep)
                            if os.path.isdir(os.path.join(d, 'HTMLTestReportSkel'))][0]
            except IndexError:
                print("Cannot find skeleton directory in ${ELEMENTS_AUX_PATH}")
                exit(1)

            # Copy the template directory (do not overwrite)
            for dirpath, dirnames, filenames in os.walk(skel):
                destpath = os.path.join(outputTemp,
                                        os.path.relpath(dirpath, skel))
                for d in dirnames:
                    if d in ('.svn', '.git'):
                        continue
                    d = os.path.join(destpath, d)
                    if not os.path.exists(d):
                        os.mkdir(d)
                for f in filenames:
                    src = os.path.join(dirpath, f)
                    dst = os.path.join(destpath, f)
                    if not os.path.exists(dst):
                        shutil.copy(src, dst)

            # process each test case
            for Test in tests:
                labels = [l.text for l in Test.findall('Labels/Label')]

                Results = Test.find("Results")

                """___ create the local summary   ___"""

                summary = {
                    "id": cleanSpace(cleanWebChar(Test.find("Name").text)),
                    # some interface specific field
                    "fields": ["end_time", "start_time", "stdout"],
                    "cause": ""
                }
                status = Test.get("Status")
                summary["outcome"] = TestOrganizer.status.get(status,
                                                              cleanSpace(cleanWebChar(status)))

                if 'test-wrapper' in labels and summary["outcome"] == 'PASS':
                    # we should not report the results of test wrappers (like
                    # QMTest) if they are successful
                    continue

                organizer_all.classify(Test)
                if summary["outcome"] != "PASS":
                    organizer_failed.classify(Test)
                if summary['outcome'] in failures_count:
                    failures_count[summary['outcome']] += 1

                # Fill the summary fields
                for NamedMeasurement in Results.getiterator("NamedMeasurement"):

                    # remove the space character
                    summary["fields"].append(cleanSpace(
                        cleanWebChar(NamedMeasurement.get("name"))))
                    # or NamedMeasurement.get("name") == "Pass Reason" :
                    if NamedMeasurement.get("name") == "Causes":
                        if NamedMeasurement.find("Value") is not None and NamedMeasurement.find("Value").text is not None:
                            summary["cause"] = cleanWebChar(
                                NamedMeasurement.find("Value").text)

                # update the global summary file
                globalSummaryFile = open(os.path.join(
                    outputTemp, "summary.json"), "r")
                globalSummary = json.loads(globalSummaryFile.read())
                globalSummaryFile.close()
                globalSummary.append(summary)
                globalSummaryFile = open(os.path.join(
                    outputTemp, "summary.json"), "w")
                globalSummaryFile.write(json.dumps(
                    globalSummary, sort_keys=True, indent=4))
                globalSummaryFile.close()

                """___ Create and fill the test directory ___"""
                # create the dir
                testCaseDir = os.path.join(outputTemp, summary["id"])
                if not os.path.isdir(testCaseDir):
                    os.mkdir(testCaseDir)
                # write the local summary
                summaryFile = open(os.path.join(
                    testCaseDir, "summary.json"), "w")
                summaryFile.write(json.dumps(
                    summary, sort_keys=True, indent=4))
                summaryFile.close()

                # write the stdout
                try:
                    value = Results.find("Measurement").find("Value")
                    text = value.text
                    if 'encoding' in value.attrib:
                        text = VALUE_DECODE[value.attrib['encoding']](text)
                    if 'compression' in value.attrib:
                        text = VALUE_DECOMP[value.attrib['compression']](text)
                    text = dropCustomMeasurements(text)
                    text = formatMeasurementText(text, escape=True)
                    # no "Measurement" or no "Value" or no text
                except AttributeError as x:
                    print('WARNING: {0[id]}: AttributeError: {1}'.format(
                        summary, x))
                    text = '<i>no stdout</i>'
                except KeyError as x:
                    print('WARNING: {0[id]}: KeyError: {1}'.format(
                        summary, x))
                    # encoding or compressions unknown, keep original text
                    text = formatMeasurementText(value=text, escape=True)
                with open(os.path.join(testCaseDir, "stdout"), "w") as stdout:
                    stdout.write(text)

                if "ctest" not in site.get("Generator"):
                    # write the other files
                    for NamedMeasurement in Results.getiterator("NamedMeasurement"):
                        value = NamedMeasurement.find("Value")
                        cleanName = cleanSpace(cleanWebChar(
                            NamedMeasurement.get("name"))).lower()
                        NamedMeasurementFile = open(
                            os.path.join(testCaseDir, cleanName), "w")

                        # Consider the void measurment
                        if value is not None and value.text is not None:
                            NamedMeasurementFile.write(
                                formatMeasurementText(value.text))
                        else:
                            NamedMeasurementFile.write("<pre></pre>")
                        NamedMeasurementFile.close()
                else:
                    executionTime = float(0)
                    # write the other files
                    for NamedMeasurement in Results.getiterator("NamedMeasurement"):
                        value = NamedMeasurement.find("Value")
                        cleanName = cleanSpace(cleanWebChar(
                            NamedMeasurement.get("name"))).lower()
                        if cleanName == "execution_time":
                            executionTime = float(value.text)
                        NamedMeasurementFile = open(
                            os.path.join(testCaseDir, cleanName), "w")

                        # Consider the void measurment
                        if value is not None and value.text is not None:
                            NamedMeasurementFile.write(
                                formatMeasurementText(value.text))
                        else:
                            NamedMeasurementFile.write("<pre></pre>")
                        NamedMeasurementFile.close()
                    # write the interface specific files
                    #!! The start_time and the end time are computed for the cTest generator.
                    # These values are only an aproximation.
                    startTimeFile = open(os.path.join(
                        testCaseDir, "start_time"), "w")
                    startTimeFile.write(time.strftime(
                        "%b %d %H:%M %Z", time.localtime(testStartTime)))
                    startTimeFile.close()
                    testEndTime = testStartTime + executionTime
                    endTimeFile = open(os.path.join(
                        testCaseDir, "end_time"), "w")
                    endTimeFile.write(time.strftime(
                        "%b %d %H:%M %Z", time.localtime(testEndTime)))
                    endTimeFile.close()
                    testStartTime = testEndTime
            organizer_all.write()
            organizer_failed.write()

    if sum(failures_count.values()):
        print('Some tests failed:')
        print('\n'.join('  %s: %s' % i for i in failures_count.items()))
        sys.exit(1)

    # print " End of the program"
    #progEndTime = datetime.now()
    #delta = progEndTime - progStartTime
    # print "program ended at "+progEndTime.strftime("%b %d %H:%M %Z")
    # print "Duration : "+str(total_seconds_replacement(delta))+" seconds"


if __name__ == '__main__':
    main()
