#
# Copyright (C) 2012-2020 Euclid Science Ground Segment
#
# This library is free software; you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by the Free
# Software Foundation; either version 3.0 of the License, or (at your option)
# any later version.
#
# This library is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
#

""" This script check a name of a project, module or product agains a given
online naming DB. The script return 0 if the entity exists and 1 if it doesn't

:file: ElementsKernel/NameCheck.py
:author: Hubert Degaudenzi

:date: 17/01/17


"""

import os
import argparse
import json

from ElementsKernel import Logging
from ElementsKernel import Exit

# pyling: disable=bare-except
try:
    from urllib2 import urlopen  # @UnusedImport @UnresolvedImport
    from urllib2 import URLError  # @UnusedImport @UnresolvedImport
except:  # pylint: disable=bare-except
    from urllib.request import urlopen  # @ImportRedefinition
    from urllib.error import URLError

LOGGER = Logging.getLogger(__name__)

TYPES = ["cmake", "library", "executable"]
DEFAULT_TYPE = "cmake"

_localUrlOpen = urlopen


def getInfo(name, db_url, entity_type=DEFAULT_TYPE):
    """ Get the informations about a given entity of a specific type """
    full_url = db_url + "/NameCheck/exists?name=%s&type=%s" % (name, entity_type)
    LOGGER.debug("The url for the name request: %s", full_url)
    info = json.loads(_localUrlOpen(full_url).read().decode("utf-8"))
    for u in ["url", "private_url"]:
        if u in info and info[u]:
            info[u] = db_url + info[u]
    return info


def checkDataBaseUrl(db_url):
    """ check if the DB URL exists """
    site_exists = True
    if db_url:
        try:
            _localUrlOpen(db_url + "/NameCheck")
        except URLError:
            site_exists = False
        except ValueError:
            site_exists = False
    else:
        site_exists = False

    return site_exists

################################################################################


def defineSpecificProgramOptions():
    """
    Define program option(s)
    """
    description = """
    This script checks a name used for a software entity (project, module
    or product) against the URL stored in the ELEMENTS_NAMING_DB_URL. It
    return \n
    - 0 if the entity exists\n
    - 1 if the entity doesn't exist\n
    - 2 if the naming DB is not reachable\n
    - 3 if there was an issue querying the DB\n
    """
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('entity_name', metavar='entity-name',
                        type=str,
                        help='Entity name')
    parser.add_argument('-U', '--url', default=os.environ.get("ELEMENTS_NAMING_DB_URL", ""),
                        help='URL for the naming database')
    parser.add_argument('-t', '--type', default=DEFAULT_TYPE, choices=TYPES,
                        help='Type for the check')

    return parser

################################################################################


Exit.Code.update({"INVALID_URL":2, "DB_ERROR":3})


def mainMethod(args):
    """
    Main
    """

    exit_code = Exit.Code["NOT_OK"]

    entity_name = args.entity_name

    if not checkDataBaseUrl(args.url):
        LOGGER.critical("The Elements Naming DB URL is not valid")
        exit_code = Exit.Code["INVALID_URL"]
    else:
        info = getInfo(entity_name, args.url, args.type)

        if info["error"]:
            LOGGER.error("There was an error querying the DB: %s", info["message"])
            exit_code = Exit.Code["DB_ERROR"]
        else:
            if info["exists"]:
                LOGGER.warning("The \"%s\" name for the %s type already exists", entity_name, args.type)
                LOGGER.info("The result for the global query of the name \"%s\" in the DB: %s",
                            entity_name, info["url"])
                LOGGER.info("The full information for the \"%s\" name of type %s: %s", entity_name,
                            args.type, info["private_url"])
                exit_code = Exit.Code["OK"]
            else:
                LOGGER.warning("The \"%s\" name of type %s doesn't exist", entity_name, args.type)
                exit_code = Exit.Code["NOT_OK"]

    return exit_code
