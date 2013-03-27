## explicit list for wildcard imports
__all__ = [
           "GaudiHandle",
           "PublicToolHandle", "PrivateToolHandle",
           "ServiceHandle",
           "GaudiHandleArray",
           "ServiceHandleArray",
           "PublicToolHandleArray", "PrivateToolHandleArray",
           ]
__version__ = "$Revision: 1.6 $"
__doc__ = """The python module holding python bindings to XyzHandles"""

from os import linesep

class GaudiHandle(object):
    componentType = "Unspecified" # must be overridden by derived class
    isPublic = True               #  can be overridden by derived class

    def __init__(self,typeAndName):
        object.__init__(self)
        if hasattr(typeAndName,"toStringProperty"):
            # this is a GaudiHandle or equivalent
            typeAndName = typeAndName.toStringProperty()
        if type(typeAndName) != str:
            raise TypeError("Argument to %s must be a string. Got a %s instead" % \
                            ( self.__class__.__name__, type(typeAndName).__name__) )
        self.typeAndName = typeAndName

    def __repr__(self):
        return "%s(%r)" % (self.__class__.__name__,self.toStringProperty())

    def __str__(self):
        # FIXME: (Patch #1668) this creates problem with 2.5
        # return "%s:%s" % (self.__class__.__name__, self.toStringProperty())
        return self.toStringProperty()

    #
    # Several member functions which are the same as Configurables
    #
    def toStringProperty(self):
        return self.typeAndName

    def getType(self):
        """Get the 'type' part of the \"type/name\" string."""
        slash = self.typeAndName.find('/')
        if slash != -1:
            # includes name. Return part before /
            return self.typeAndName[0:slash]
        else:
            # only type is given, return full string
            return self.typeAndName

    def getName(self):
        """Get the 'name' part of the \"type/name\" string.
        If only a type is given, this will be returned as the name.
        If the \"type/name\" string is empty, it will return an emtpy string."""
        slash = self.typeAndName.find('/')
        if slash != -1:
            # explicit name. Return part after the /
            return self.typeAndName[slash+1:]
        else:
            # only type is given. return type as default name
            return self.typeAndName

    def getGaudiHandle(self):
        return self

    def getFullName(self):
        return self.toStringProperty()

class PublicToolHandle(GaudiHandle):
    __slots__ = ()
    componentType = "AlgTool"
    isPublic = True

    def __init__(self, toolTypeAndName=''):
        GaudiHandle.__init__( self, toolTypeAndName )


class PrivateToolHandle(GaudiHandle):
    __slots__ = ()
    componentType = "AlgTool"
    isPublic = False

    def __init__(self, toolTypeAndName=''):
        GaudiHandle.__init__( self, toolTypeAndName )


class ServiceHandle(GaudiHandle):
    __slots__ = ()
    componentType = "Service"
    isPublic = True

    def __init__(self, serviceName=''):
        GaudiHandle.__init__( self, serviceName )

#
# The HandleArrays
#
class GaudiHandleArray(list):
    """A list of GaudiHandles. Only handles of one type are allowed, as specified by self.__class__.handleType
    """
    __slots__ = ( 'typesAndNames' )
    handleType = None # must be set by derived class to the handle type

    def __init__(self,typesAndNames=None):
        if typesAndNames is None: typesAndNames = []
        list.__init__(self)
        # check the type
        if type(typesAndNames) != list:
            raise TypeError("Argument to %s must be a list. Got a %s instead" % \
                            ( self.__class__.__name__, type(typesAndNames).__name__) )
        # add entries to list
        for tn in typesAndNames: self.append( tn )

    def __repr__(self):
        """Return class name with list of type/name strings as argument"""
        rep = self.__class__.__name__ + '(['
        for h in self:
            rep += repr(h.toStringProperty()) + ','
        # remove last comma
        if rep[-1] == ',': rep = rep[:-1]
        return rep + '])'

    def __str__(self):
        """Print entries, one per line"""
        shortName = self.__class__.__name__
        return "%s:%s" % (shortName, linesep + linesep.join([str(s) for s in self]))

    def __getitem__(self,index):
        if type(index) == str:
            # seach by instance name
            for h in self:
                if h.getName() == index:
                    return h
            raise IndexError( "%s does not have a %s with instance name %s" % \
                              (self.__class__.__name__, self.handleType.componentType, index) )
        else:
            return list.__getitem__(self,index)

    def __delitem__( self, key ):
        super( GaudiHandleArray, self ).__delitem__( self.index(self[key]) )

    def __iadd__(self,array):
        arrayType = type(array)
        if arrayType == list or arrayType == type(self):
            for v in array:
                self.append( v )
        else:
            raise TypeError( "Can not add a %s to a %s" % (arrayType.__name__, self.__class__.__name__) )

        return self

    def append( self, value ):
        """Only allow appending compatible types. It accepts a string, a handle or a configurable."""
        if type(value) == str:
            # convert string to handle
            value = self.__class__.handleType(value)
        elif type(value) == self.__class__.handleType:
            pass # leave handle as-is
        elif isinstance( value, GaudiHandle ):
            # do not allow different type of handles
            raise TypeError( "Can not add a %s to a %s" % (value.__class__.__name__, self.__class__.__name__) )
        elif value.getGaudiType() != self.__class__.handleType.componentType:
            # assume it is a configurable: allow only correct types
            raise TypeError( "Can not append %s (%s) to a %s" % \
                             (value.__class__.__name__, value.getGaudiType(), self.__class__.__name__) )
        elif hasattr(value,'isPublic'):
            # check public vs private if applicable for this configurable
            pop = value.isPublic() and 'Public' or 'Private'
            if value.isPublic() != self.__class__.handleType.isPublic:
                raise TypeError( "Can not append %s (%s %s) to a %s" % \
                                 (value.__class__.__name__, pop, value.getGaudiType(), self.__class__.__name__) )

        # check that an instance name appears only once in the list
        try:
            oldValue = self.__getitem__( value.getName() )
        except IndexError:
            # not yet there, so add it
            list.append( self, value )
        else:
            print "%s    WARNING %r with instance name %r already in list. Not adding %r" % \
                  (self.__class__.__name__, oldValue, oldValue.getName(), value)


    def isPublic(self):
        return self.__class__.handleType.isPublic

    #
    # Member functions which are the same as Configurables
    #
    def toStringProperty(self):
        rep = '['
        # add entries
        for v in self:
            rep += repr( v.toStringProperty() ) + ','
        # remove last comma
        if rep[-1] == ',': rep = rep[:-1]
        return rep + ']'

    # pickle support
    def __getstate__ (self):
        return { 'typesAndNames' : self.typesAndNames }

    def __setstate__ ( self, dict ):
        self.typesAndNames = dict[ 'typesAndNames' ]


class ServiceHandleArray(GaudiHandleArray):
    __slots__ = ()
    handleType = ServiceHandle

    def __init__(self, serviceTypesAndNames=None):
        GaudiHandleArray.__init__( self, serviceTypesAndNames )

class PublicToolHandleArray(GaudiHandleArray):
    __slots__ = ()
    handleType = PublicToolHandle

    def __init__(self, toolTypesAndNames=None):
        GaudiHandleArray.__init__( self, toolTypesAndNames )


class PrivateToolHandleArray(GaudiHandleArray):
    __slots__ = ()
    handleType = PrivateToolHandle

    def __init__(self, toolTypesAndNames=None):
        GaudiHandleArray.__init__( self, toolTypesAndNames )
