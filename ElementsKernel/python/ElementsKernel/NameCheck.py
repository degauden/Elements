"""
@file: ElementsKernel/NameCheck.py
@author: Hubert Degaudenzi

@date: 17/01/17

This script check a name of a project, module or product agains a given 
online naming DB. The script return 0 if the entity exists and 1 if it doesn't

@copyright: 2012-2020 Euclid Science Ground Segment

This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation; either version 3.0 of the License, or (at your option)
any later version.

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

"""

import os
import argparse
import ElementsKernel.Logging as log

import json
import urllib2
import ssl

logger = log.getLogger('NameCheck')

def getInfo(name, db_url):
    full_url = db_url + "/NameCheck/exists?name=%s" % name
    logger.debug("The url for the name request: %s" % full_url)
    req = urllib2.Request(full_url, headers={ 'X-Mashape-Key': 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX' })
    gcontext = ssl.SSLContext(ssl.PROTOCOL_TLSv1)
    info = urllib2.urlopen(req, context=gcontext).read()
    return json.loads(info)


################################################################################

def defineSpecificProgramOptions():
    """
    Define program option(s)
    """
    description = """
    This script checks a name used for a software entity (project, module
    or product) against the URL stored in the ELEMENTS_NAMING_DB_URL. It
    return 
    - 0 if the entity exists
    - 1 if the entity doesn't exist
    - 2 if the naming DB is not reachable
    """
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('entity_name', metavar='entity-name',
                        type=str,
                        help='Entitiy name')
    parser.add_argument('-U', '--url', default=os.environ.get("ELEMENTS_NAMING_DB_URL", ""), help='URL for the naming database')

    return parser

################################################################################

def mainMethod(args):
    """
    Main
    """

    exit_code = 1

    entity_name = args.entity_name
    url = args.url

    if not url:
        logger.critical("The Elements Naming DB URL is not valid")
        exit_code = 2
    else:
        info = getInfo(entity_name, url)

        if info["exists"]:
            logger.warn("The \"%s\" name already exists", entity_name)
            logger.warn("It is of type: %s", info["type"])
            exit_code = 0
        else:
            logger.warn("The \"%s\" name doesn't exist", entity_name)
            exit_code = 1

    return exit_code


