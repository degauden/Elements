/**
 * @file ElementsKernel/Exit.h
 *
 * @date Aug 25, 2014
 * @author Hubert Degaudenzi
 */

#ifndef ELEMENTSKERNEL_EXIT_H_
#define ELEMENTSKERNEL_EXIT_H_


namespace Elements {

/// Strongly type exit numbers
/// @details This has to be used with the MAIN_FOR macro
/// There is no clear universal specification for exit codes. Every system
/// has more or less its convention. Please have a look at http://en.wikipedia.org/wiki/Exit_status
/// Notes:
///    - by default (without any implementation), the uncaught signal produces an
///      exit code of 128+signal value.
///    - the numbers used below are extracted from the header file /usr/include/sysexits.h
///    - we can use the exit code range [1,63] to define our own specific codes. Which should be
///      less general that the ones below and reflect the behavior of our own code.
enum class ExitCode : int {

  OK          =  0,     ///< Everything is OK

  NOT_OK      =  1,     ///< Generic unknown failure

  USAGE       = 64,     ///< command line usage error
  DATAERR     = 65,     ///< data format error
  NOINPUT     = 66,     ///< cannot open input
  NOUSER      = 67,     ///< addressee unknown
  NOHOST      = 68,     ///< host name unknown
  UNAVAILABLE = 69,     ///< service unavailable
  SOFTWARE    = 70,     ///< internal software error
  OSERR       = 71,     ///< system error (e.g., can't fork)
  OSFILE      = 72,     ///< critical OS file missing
  CANTCREAT   = 73,     ///< can't create (user) output file
  IOERR       = 74,     ///< input/output error
  TEMPFAIL    = 75,     ///< temp failure; user is invited to retry
  PROTOCOL    = 76,     ///< remote error in protocol
  NOPERM      = 77,     ///< permission denied
  CONFIG      = 78      ///< configuration error

};

} // namespace Elements

#endif // ELEMENTSKERNEL_EXIT_H_
