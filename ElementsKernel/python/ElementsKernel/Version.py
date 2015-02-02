'''
Created on Jan 29, 2015

@author: hubert
'''


def getVersionString(maj, min, pat=0):

    version = "%d.%d" % (maj, min)

    if pat:
        version += ".%d" % pat

    return version