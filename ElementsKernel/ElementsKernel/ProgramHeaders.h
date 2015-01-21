#ifndef PROGRAM_HEADER_H
#define PROGRAM_HEADER_H

// there is no version of boost with the std::__debug namespace
#include "ElementsKernel/NoGlibDebug.h"

#include <boost/program_options.hpp>        // for program options from configuration file of command line arguments
#include <boost/filesystem.hpp>             // for boost path type

#include "ElementsKernel/Exit.h"            // for ExitCode, ExitCode::OK
#include "ElementsKernel/Logging.h"         // for Logging::LogMessageStream, etc
#include "ElementsKernel/Exception.h"       // for Elements Exception

#include "ElementsKernel/Main.h"            // for the MAIN macro
#include "ElementsKernel/Program.h"         // for the Program interface

#include "ElementsKernel/Version.h"         // ??

#endif
