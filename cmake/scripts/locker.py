#@PydevCodeAnalysisIgnore
##########################################################
# stolen and (slighty) adapted from:
# http://aspn.activestate.com/ASPN/Cookbook/Python/Recipe/65203
##

import os
import time

if os.name == 'nt':
    import msvcrt
elif os.name == 'posix':
    import fcntl  # @UnresolvedImport
    LOCK_EX = fcntl.F_WRLCK
    LOCK_SH = fcntl.F_RDLCK
    LOCK_NB = fcntl.F_UNLCK
else:
    raise RuntimeError("Locker only defined for nt and posix platforms")

if os.name == 'nt':
    def lock(filename):
        """
        Lock first 10 bytes of a filename.
        """
        pos = filename.tell()  # remember current position
        filename.seek(0)
        # By default, python tries about 10 times, then throws an exception.
        # We want to wait forever.
        acquired = False
        while not acquired:
            try:
                msvcrt.locking(filename.fileno(), msvcrt.LK_LOCK, 10)
                acquired = True
            except IOError, x:
                # 36, AKA 'Resource deadlock avoided', is normal
                if x.errno != 36:
                    raise
        filename.seek(pos)  # reset position

    def unlock(filename):
        """
        Unlock first 10 bytes of a filename.
        """
        pos = filename.tell()  # remember current position
        filename.seek(0)
        msvcrt.locking(filename.fileno(), msvcrt.LK_UNLCK, 10)
        filename.seek(pos)  # reset position

elif os.name == 'posix':
    import socket
    import errno

    def _tmpFileName(fileName):
        return "%s.%s.%d" % (fileName, socket.gethostname(), os.getpid())

    def _lckFileName(fileName):
        return "%s.lock" % fileName

    def _linkCount(lockFileName):
        try:
            return os.stat(lockFileName).st_nlink
        except OSError, e:
            if e.errno != errno.ENOENT:
                raise
            return -1

    def _read(fileName):
        try:
            fp = open(fileName)
            try:
                readFileName = fp.read()
            finally:
                fp.close()
            return readFileName
        except EnvironmentError, e:
            if e.errno != errno.ENOENT:
                raise
            return None

    def _sleep(): time.sleep(8)

    def lock(filename):
        fileName = filename.name
        tmpFileName = _tmpFileName(fileName)
        fp = open(tmpFileName, "w")
        fp.write(tmpFileName)
        fp.close()

        lockFileName = _lckFileName(fileName)
        while True:
            if _linkCount(lockFileName) != -1:
                _sleep()
            try:
                os.link(tmpFileName, lockFileName)
                # we acquired the lock
                return
            except OSError, e:
                if e.errno == errno.ENOENT:
                    pass
                elif e.errno != errno.EEXIST:
                    os.unlink(tmpFileName)
                    raise
                elif _linkCount(lockFileName) != 2:
                    pass
                elif _read(lockFileName) == tmpFileName:
                    raise
                else:
                    # someone else owns that lock
                    pass
            # didn't get the lock !!
            # say something ?
            # print _id_()," failed to acquire the lock ..."
            _sleep()
            pass
        return

    def unlock(filename):
        fileName = filename.name
        tmpFileName = _tmpFileName(fileName)
        lockFileName = _lckFileName(fileName)

        try:
            os.unlink(lockFileName)
        except OSError, e:
            if e.errno != errno.ENOENT:
                raise
        # remove the tmp filename
        try:
            os.unlink(tmpFileName)
        except OSError, e:
            if e.errno != errno.ENOENT:
                raise
        return

import logging
# Lock a file.
#  The file for the lock is created if it doesn't exists and it the "temporary"
#  argument is set to True it will also be deleted when the lock is not needed.
#  The unlocking is done in the destructor (RAII pattern).


class LockFile(object):

    def __init__(self, name, temporary=False):
        self.name = name
        self.temporary = temporary
        self.file = None
        self.log = logging.getLogger("LockFile")
        self.log.info(
            "%s - Locking on %s", time.strftime("%Y-%m-%d_%H:%M:%S"), self.name)
        if not os.path.exists(name):
            mode = "w"
        else:
            # I do not want to delete a file I didn't create
            self.temporary = False
            mode = "r+"
        try:
            self.file = open(self.name, mode)
            lock(self.file)
        except:
            self.log.warning("Cannot acquire lock on %s", self.name)

    def __del__(self):
        if self.file:
            unlock(self.file)
            self.file.close()
            if self.temporary:
                try:
                    os.remove(self.name)
                except:
                    pass
            self.log.info(
                "%s - Lock on %s released", time.strftime("%Y-%m-%d_%H:%M:%S"), self.name)
