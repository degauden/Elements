import os, sys, re

import logging
_log = logging.getLogger(__name__)

class LogFormatter(logging.Formatter):
    def __init__(self, fmt=None, datefmt=None, prefix = "# "):
        logging.Formatter.__init__(self, fmt, datefmt)
        self.prefix = prefix
    def format(self, record):
        fmsg = logging.Formatter.format(self, record)
        prefix = self.prefix
        if record.levelno >= logging.WARNING:
            prefix += record.levelname + ": "
        s = "\n".join([ prefix + line
                        for line in fmsg.splitlines() ])
        return s

class LogFilter(logging.Filter):
    def __init__(self, name = ""):
        logging.Filter.__init__(self, name)
        self.printing_level = 0
        self.enabled = True
        self.threshold = logging.WARNING
    def filter(self, record):
        return record.levelno >= self.threshold or (self.enabled and self.printing_level <= 0)
    def printOn(self, step = 1, force = False):
        """
        Decrease the printing_level of 'step' units. ( >0 means no print)
        The level cannot go below 0, unless the force flag is set to True.
        A negative value of the threshold disables subsequent "PrintOff"s.
        """
        if force:
            self.printing_level -= step
        else:
            if self.printing_level > step:
                self.printing_level -= step
            else:
                self.printing_level = 0
    def printOff(self, step = 1):
        """
        Increase the printing_level of 'step' units. ( >0 means no print)
        """
        self.printing_level += step
    def disable(self, allowed = logging.WARNING):
        self.enabled = False
        self.threshold = allowed
    def enable(self, allowed = logging.WARNING):
        self.enabled = True
        self.threshold = allowed

class ConsoleHandler(logging.StreamHandler):
    def __init__(self, stream = None, prefix = None):
        if stream is None:
            stream = sys.stdout
        logging.StreamHandler.__init__(self, stream)
        if prefix is None:
            prefix = "# "
        self._filter = LogFilter(_log.name)
        self._formatter = LogFormatter(prefix = prefix)
        self.setFormatter(self._formatter)
        self.addFilter(self._filter)
    def setPrefix(self, prefix):
        self._formatter.prefix = prefix
    def printOn(self, step = 1, force = False):
        """
        Decrease the printing_level of 'step' units. ( >0 means no print)
        The level cannot go below 0, unless the force flag is set to True.
        A negative value of the threshold disables subsequent "PrintOff"s.
        """
        self._filter.printOn(step, force)
    def printOff(self, step = 1):
        """
        Increase the printing_level of 'step' units. ( >0 means no print)
        """
        self._filter.printOff(step)
    def disable(self, allowed = logging.WARNING):
        self._filter.disable(allowed)
    def enable(self, allowed = logging.WARNING):
        self._filter.enable(allowed)

_consoleHandler = None
def GetConsoleHandler(prefix = None, stream = None):
    global _consoleHandler
    if _consoleHandler is None:
        _consoleHandler = ConsoleHandler(prefix = prefix, stream = stream)
    elif prefix is not None:
        _consoleHandler.setPrefix(prefix)
    return _consoleHandler

def InstallRootLoggingHandler(prefix = None, level = None, stream = None):
    root_logger = logging.getLogger()
    if not root_logger.handlers:
        root_logger.addHandler(GetConsoleHandler(prefix, stream))
        root_logger.setLevel(logging.WARNING)
    if level is not None:
        root_logger.setLevel(level)

def PrintOn(step = 1, force = False):
    GetConsoleHandler().printOn(step, force)
def PrintOff(step = 1):
    GetConsoleHandler().printOff(step)

class ParserError(RuntimeError):
    pass

def _find_file(f):
    # expand environment variables in the filename
    f = os.path.expandvars(f)
    if os.path.isfile(f):
        return os.path.realpath(f)

    path = os.environ.get('JOBOPTSEARCHPATH','').split(os.pathsep)
    # find the full path to the option file
    candidates = [d for d in path if os.path.isfile(os.path.join(d,f))]
    if not candidates:
        raise ParserError("Cannot find '%s' in %s" % (f,path))
    return os.path.realpath(os.path.join(candidates[0],f))

_included_files = set()
def _to_be_included(f):
    if f in _included_files:
        _log.warning("file '%s' already included, ignored.", f)
        return False
    _included_files.add(f)
    return True

class JobOptsParser:
    comment = re.compile(r'(//.*)$')
    # non-perfect R-E to check if '//' is inside a string
    # (a tokenizer would be better)
    comment_in_string = re.compile(r'(["\']).*//.*\1')
    directive = re.compile(r'^\s*#\s*([\w!]+)\s*(.*)\s*$')
    comment_ml = ( re.compile(r'/\*'), re.compile(r'\*/') )
    statement_sep = ";"
    reference = re.compile(r'^@([\w.]*)$')

    def __init__(self):
        # parser level states
        self.units = {}
        self.defines = {}
        if sys.platform != 'win32':
            self.defines[ "WIN32" ] = True

    def _include(self,file,function):
        file = _find_file(file)
        if _to_be_included(file):
            _log.info("--> Including file '%s'", file)
            function(file)
            _log.info("<-- End of file '%s'", file)

    def parse(self,file):
        # states for the "translation unit"
        statement = ""

        ifdef_level = 0
        ifdef_skipping = False
        ifdef_skipping_level = 0

        f = open(_find_file(file))
        l = f.readline()
        if l.startswith("#!"):
            # Skip the first line if it starts with "#!".
            # It allows to use options files as scripts.
            l = f.readline()

        while l:
            l = l.rstrip()+'\n' # normalize EOL chars (to avoid problems with DOS new-line on Unix)

            # single line comment
            m = self.comment.search(l)
            if m:
                # check if the '//' is part of a string
                m2 = self.comment_in_string.search(l)
                # the '//' is part of a string if we find the quotes around it
                # and they are not part of the comment itself
                if not ( m2 and m2.start() < m.start() ):
                    # if it is not the case, we can remove the comment from the
                    # statement
                    l = l[:m.start()]+l[m.end():]
            # process directives
            m = self.directive.search(l)
            if m:
                directive_name = m.group(1)
                directive_arg = m.group(2).strip()
                if directive_name == "include":
                    included_file = directive_arg.strip("'\"")
                    importOptions(included_file)
                elif directive_name == "units":
                    units_file = directive_arg.strip("'\"")
                    self._include(units_file,self._parse_units)
                elif directive_name in [ "ifdef", "ifndef"]:
                    ifdef_skipping_level = ifdef_level
                    ifdef_level += 1
                    if directive_arg in self.defines:
                        ifdef_skipping = directive_name == "ifndef"
                    else:
                        ifdef_skipping = directive_name == "ifdef"
                elif directive_name == "else":
                    ifdef_skipping = not ifdef_skipping
                elif directive_name == "endif":
                    ifdef_level -= 1
                    if ifdef_skipping and ifdef_skipping_level == ifdef_level:
                        ifdef_skipping = False
                elif directive_name == "pragma":
                    if not directive_arg:
                        l = f.readline()
                        continue
                    pragma = directive_arg.split()
                    if pragma[0] == "print":
                        if len(pragma) > 1:
                            if pragma[1].upper() in [ "ON", "TRUE", "1" ]:
                                PrintOn()
                            else:
                                PrintOff()
                else:
                    _log.warning("unknown directive '%s'", directive_name)
                l = f.readline()
                continue

            if ifdef_skipping:
                l = f.readline()
                continue

            # multi-line comment
            m = self.comment_ml[0].search(l)
            if m:
                l,l1 = l[:m.start()],l[m.end():]
                m = self.comment_ml[1].search(l1)
                while not m:
                    l1 = f.readline()
                    if not l1:
                        break # EOF
                    m = self.comment_ml[1].search(l1)
                if not l1 and not m:
                    raise ParserError("End Of File reached before end of multi-line comment")
                l += l1[m.end():]

            if self.statement_sep in l:
                i = l.index(self.statement_sep)
                statement += l[:i]
                self._eval_statement(statement.replace("\n","").strip())
                statement = l[i+1:]
                # it may happen (bug #37479) that the rest of the statement
                # contains a comment.
                if statement.lstrip().startswith("//"):
                    statement = ""
            else:
                statement += l

            l = f.readline()

    def _parse_units(self,file):
        for line in open(file):
            if '//' in line:
                line = line[:line.index('//')]
            line = line.strip()
            if not line:
                continue
            nunit, value = line.split('=')
            factor, unit = nunit.split()
            value = eval(value)/eval(factor)
            self.units[unit] = value

    def _eval_statement(self,statement):
        from GaudiKernel.Proxy.Configurable import (ConfigurableGeneric,
                                                    Configurable,
                                                    PropertyReference)
        #statement = statement.replace("\n","").strip()
        _log.info("%s%s", statement, self.statement_sep)

        property,value = statement.split("=",1)

        inc = None
        if property[-1] in [ "+", "-" ]:
            inc = property[-1]
            property = property[:-1]

        property = property.strip()
        value = value.strip()

        ## find the configurable to apply the property to
        #parent_cfg = None
        #while '.' in property:
        #    component, property = property.split('.',1)
        #    if parent_cfg:
        #        if hasattr(parent_cfg,component):
        #            cfg = getattr(parent_cfg,component)
        #        else:
        #            cfg = ConfigurableGeneric(component)
        #            setattr(parent_cfg,component,cfg)
        #    else:
        #        cfg = ConfigurableGeneric(component)
        #    parent_cfg = cfg

        # remove spaces around dots
        property = '.'.join([w.strip() for w in property.split('.')])
        component, property = property.rsplit('.',1)
        if component in Configurable.allConfigurables:
            cfg = Configurable.allConfigurables[component]
        else:
            cfg = ConfigurableGeneric(component)

        #value = os.path.expandvars(value)
        value = value.replace('true','True').replace('false','False')
        if value[0] == '{' :
            # Try to guess if the values looks like a dictionary
            if ':' in value and not ( value[:value.index(':')].count('"')%2 or value[:value.index(':')].count("'")%2 ) :
                # for dictionaries, keep the surrounding {}
                value = '{'+value[1:-1].replace('{','[').replace('}',']')+'}'
            else : # otherwise replace all {} with []
                value = value.replace('{','[').replace('}',']')

        # We must escape '\' because eval tends to interpret them
        value = value.replace('\\','\\\\')
        # Replace literal '\n' and '\t' with spaces (bug #47258)
        value = value.replace("\\n", " ").replace("\\t", " ")

        # interprete the @ operator
        m = self.reference.match(value)
        if m:
            # this allows late binding of references
            value = PropertyReference(m.group(1))
        else:
            value = eval(value,self.units)

        #if type(value) is str    : value = os.path.expandvars(value)
        #elif type(value) is list : value = [ type(item) is str and os.path.expandvars(item) or item for item in value ]

        if property not in cfg.__slots__ and not hasattr(cfg,property):
            # check if the case of the property is wrong (old options are case insensitive)
            lprop = property.lower()
            for p in cfg.__slots__:
                if lprop == p.lower():
                    _log.warning("property '%s' was requested for %s, but the correct spelling is '%s'", property, cfg.name(), p)
                    property = p
                    break

        # consider the += and -=
        if inc == "+":
            if hasattr(cfg,property):
                prop = getattr(cfg,property)
                if type(prop) == dict:
                    for k in value:
                        prop[k] = value[k]
                else:
                    prop += value
            else:
                setattr(cfg,property,value)
        elif inc == "-":
            if hasattr(cfg,property):
                prop = getattr(cfg,property)
                if type(prop) is dict:
                    for k in value:
                        if k in prop:
                            del prop[k]
                        else:
                            _log.warning("key '%s' not in %s.%s", k, cfg.name(), property)
                else:
                    for k in value:
                        if k in prop:
                            prop.remove(k)
                        else:
                            _log.warning("value '%s' not in %s.%s", k, cfg.name(), property)
        else:
            setattr(cfg,property,value)

class _TempSysPath:
    def __init__(self, new_path):
        self.old_path = sys.path
        sys.path = new_path
    def __del__(self):
        sys.path = self.old_path

_parser = JobOptsParser()

def _import_python(file):
    execfile(file, {})

def _import_pickle(file):
    import pickle
    input = open(file, 'rb')
    catalog = pickle.load(input)
    _log.info('Unpickled %d configurables', len(catalog))

def _import_opts(file):
    _parser.parse(file)

_import_function_mapping = {
                             ".py"   : _import_python,
                             ".pkl"  : _import_pickle,
                             ".opts" : _import_opts,
                            }

def importOptions( optsfile ) :
    # expand environment variables before checking the extension
    optsfile = os.path.expandvars(optsfile)
    # check the file type (extension)
    dummy, ext = os.path.splitext(optsfile)
    if ext in _import_function_mapping:
        # check if the file has been already included
        optsfile = _find_file(optsfile)
        if _to_be_included(optsfile):
            _log.info("--> Including file '%s'", optsfile)
            # include the file
            _import_function_mapping[ext](optsfile)
            _log.info("<-- End of file '%s'", optsfile)
    else:
        raise ParserError("Unknown file type '%s' ('%s')" % (ext,optsfile))

## Import a file containing declaration of units.
#  It is equivalent to:
#
#  #units "unitsfile.opts"
#
def importUnits(unitsfile):
    # expand environment variables
    unitsfile = os.path.expandvars(unitsfile)
    # we do not need to check the file type (extension) because it must be a
    # units file
    _parser._include(unitsfile, _parser._parse_units)
