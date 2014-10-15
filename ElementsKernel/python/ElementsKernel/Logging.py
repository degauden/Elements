##
# @file ElementsKernel/Logging.py
#


import logging

# Check we are running for the first time. If we are we need to change a bit the
# naming of the logging levels to match the ones of the C++
if logging.getLevelName(logging.CRITICAL) == 'CRITICAL':
    logging.addLevelName(50,'FATAL')
    logging.addLevelName(40,'ERROR')
    logging.addLevelName(30,'WARN')
    logging.addLevelName(20,'INFO')
    logging.addLevelName(10,'DEBUG')

_formatter = logging.Formatter(fmt='%(asctime)s %(name)s %(levelname)5s : %(message)s', datefmt='%Y-%m-%dT%X%Z')

# Check if we have the console logger set already. This might happen if the
# user reloads the module using the reload() method.
if not [h for h in logging.getLogger().handlers if h.get_name() == 'console']:
    # Set the default logging level
    logging.getLogger().setLevel(logging.INFO)
    # Add the stderr log handler
    _console = logging.StreamHandler()
    _console.set_name('console')
    _console.setFormatter(_formatter)
    logging.getLogger().addHandler(_console)

def getLogger(name):
    return logging.getLogger(name)

def setLevel(level):
    logging.getLogger().setLevel(level)

def setLogFile(filename):
    # If we already have a file appenders remove them
    for h in [h for h in logging.getLogger().handlers if h.get_name() == 'file']:
        logging.getLogger().removeHandler(h)
        h.close()
    # Add the new file handler
    h = logging.FileHandler(filename)
    h.set_name('file')
    h.setFormatter(_formatter)
    logging.getLogger().addHandler(h)