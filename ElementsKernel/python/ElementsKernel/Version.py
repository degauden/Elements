'''
Created on Jan 29, 2015

@author: hubert
'''


def getVersionString(major, minor, patch=0):

    version = "%d.%d" % (major, minor)

    if patch:
        version += ".%d" % patch

    return version