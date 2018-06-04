# @PydevCodeAnalysisIgnore
##########################################################
# stolen and (slighty) adapted from:
# http://aspn.activestate.com/ASPN/Cookbook/Python/Recipe/65203
# #

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
        # remember current position
        pos = filename.tell()
        filename.seek(0)
        # By default, python tries about 10 times, then throws an exception.
        # We want to wait forever.
        acquired = False
        while not acquired:
            try:
                msvcrt.locking(filename.fileno(), msvcrt.LK_LOCK, 10)
                acquired = True
            except IOError as x:
                # 36, AKA 'Resource deadlock avoided', is normal
                if x.errno != 36:
                    raise
        # reset position
        filename.seek(pos)

    def unlock(filename):
        """
        Unlock first 10 bytes of a filename.
        """
        # remember current position
        pos = filename.tell()
        filename.seek(0)
        msvcrt.locking(filename.fileno(), msvcrt.LK_UNLCK, 10)
        # reset position
        filename.seek(pos)

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
        except OSError as e:
            if e.errno != errno.ENOENT:
                raise
            return -1

    def _read(fileName):
        try:
            fp = open(fileName)
            try:
                read_file_name = fp.read()
            finally:
                fp.close()
            return read_file_name
        except EnvironmentError as e:
            if e.errno != errno.ENOENT:
                raise
            return None

    def _sleep():
        time.sleep(8)

    def lock(filename):
        file_name = filename.name
        tmp_file_name = _tmpFileName(file_name)
        fp = open(tmp_file_name, "w")
        fp.write(tmp_file_name)
        fp.close()

        lock_file_name = _lckFileName(file_name)
        while True:
            if _linkCount(lock_file_name) != -1:
                _sleep()
            try:
                os.link(tmp_file_name, lock_file_name)
                # we acquired the lock
                return
            except OSError as e:
                if e.errno != errno.EEXIST:
                    os.unlink(tmp_file_name)
                    raise
                elif _read(lock_file_name) == tmp_file_name:
                    raise
                else:
                    # someone else owns that lock
                    pass
            # didn't get the lock !!
            # say something ?
            # print _id_()," failed to acquire the lock ..."
            _sleep()
        return

    def unlock(filename):
        file_name = filename.name
        tmp_file_name = _tmpFileName(file_name)
        lock_file_name = _lckFileName(file_name)

        try:
            os.unlink(lock_file_name)
        except OSError as e:
            if e.errno != errno.ENOENT:
                raise
        # remove the tmp filename
        try:
            os.unlink(tmp_file_name)
        except OSError as e:
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
