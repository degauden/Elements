/**
 * @file ElementsKernel/Exit.h
 * @brief define a list of standard exit codes for executables
 * @date Aug 25, 2014
 * @author Hubert Degaudenzi
 */

#ifndef ELEMENTSKERNEL_EXIT_H_
#define ELEMENTSKERNEL_EXIT_H_


#include <type_traits>

namespace Elements {

/** @enum ExitCode
 * @brief Strongly typed exit numbers
 * @details This has to be used with the MAIN_FOR macro
 * There is no clear universal specification for exit codes. Every system
 * has more or less its convention. Please have a look at http://en.wikipedia.org/wiki/Exit_status.
 * These exit codes do exist for this very documentation. They allow a minimal communication
 * with the batch framework. This gives a clear answer to the caller without the need of
 * parsing the output of the executable.
 * Notes:
 *    - by default (without any implementation), the uncaught signal produces an
 *      exit code of 128+signal value. Please have a look at "man 7 signal". For example the good old SIGSEGV (11)
 *      would trigger a system generated exit code of 139 = 128 + 11
 *    - we can use the exit code range [1,63] to define our own specific codes. Which should be
 *      less general that the ones below and reflect the behaviour of our own code.
 *      - OK : Everything is fine
 *      - NOT_OK : Something went wrong. This should be the default value for any
 *              unspecified failure.
 *    - the numbers used below are extracted from the header file /usr/include/sysexits.h
 *      - USAGE : The command was used incorrectly, e.g., with
 *              the wrong number of arguments, a bad flag, a bad
 *              syntax in a parameter, or whatever.
 *      - DATAERR : The input data was incorrect in some way.
 *              This should only be used for user's data & not
 *              system files.
 *      - NOINPUT : An input file (not a system file) did not
 *              exist or was not readable.  This could also include
 *              errors like "No message" to a mailer (if it cared
 *              to catch it).
 *      - NOUSER : The user specified did not exist.  This might
 *              be used for mail addresses or remote logins.
 *      - NOHOST : The host specified did not exist.  This is used
 *              in mail addresses or network requests.
 *      - UNAVAILABLE : A service is unavailable.  This can occur
 *              if a support program or file does not exist.  This
 *              can also be used as a catchall message when something
 *              you wanted to do doesn't work, but you don't know
 *              why.
 *      - SOFTWARE : An internal software error has been detected.
 *              This should be limited to non-operating system related
 *              errors as possible.
 *      - OSERR : An operating system error has been detected.
 *              This is intended to be used for such things as "cannot
 *              fork", "cannot create pipe", or the like.  It includes
 *              things like getuid returning a user that does not
 *              exist in the passwd file.
 *      - OSFILE : Some system file (e.g., /etc/passwd, /etc/utmp,
 *              etc.) does not exist, cannot be opened, or has some
 *              sort of error (e.g., syntax error).
 *      - CANTCREAT : A (user specified) output file cannot be
 *              created.
 *      - IOERR : An error occurred while doing I/O on some file.
 *      - TEMPFAIL : temporary failure, indicating something that
 *              is not really an error.  In sendmail, this means
 *              that a mailer (e.g.) could not create a connection,
 *              and the request should be reattempted later.
 *      - PROTOCOL : the remote system returned something that
 *              was "not possible" during a protocol exchange.
 *      - NOPERM : You did not have sufficient permission to
 *              perform the operation.  This is not intended for
 *              file system problems, which should use NOINPUT or
 *              CANTCREAT, but rather for higher level permissions.
 */
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

/// Underlying type of the ExitCode class
using ExitCodeType = std::underlying_type<ExitCode>::type;


} // namespace Elements

#endif // ELEMENTSKERNEL_EXIT_H_
