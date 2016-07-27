
import platform

if (platform.system() == "Darwin"):
    SHLIB_VAR_NAME = "DYLD_LIBRARY_PATH"
else:
    SHLIB_VAR_NAME = "LD_LIBRARY_PATH"

LIB_PREFIX = "lib"


if (platform.system() == "Darwin"):
    LIB_EXTENSION = "dylib"
else:
    LIB_EXTENSION = "so"

LIB_SUFFIX = "." + LIB_EXTENSION

SHLIB_SUFFIX = LIB_SUFFIX
