/**
 * @file ElementsKernel/Logging.h
 * @brief Logging facility
 * @date January 13, 2014
 * @author Nikolaos Apostolakos
 *
 * @copyright 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @addtogroup ElementsKernel ElementsKernel
 * @{
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_LOGGING_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_LOGGING_H_

#include <string>
#include <map>
#include <utility>                  // for forward

#include <boost/filesystem.hpp>     // for path
#include <log4cpp/Category.hh>

#include "ElementsKernel/Export.h"  // ELEMENTS_API

namespace Elements {

/**
 * @class Logging
 *
 * @brief Logging API of the Elements framework
 *
 *
 * The Elements::Logging class provides the logging API of the Elements framework.
 * To use the logging API the Elements::Logging::getLogger method can be used to
 * retrieve a logger instance, which can be further used for logging messages
 * of different severities. For construction of more complicated messages, the
 * printf style and stream style syntax are supported. For example:
 *
 * \code
 * Elements::Logging logger = Elements::Logging::getLogger("name");
 * logger.debug("A debug message");
 * logger.info("A value %d in a printf style info message", 15);
 * logger.error() << "A value " << 15 << " in a steam style error message";
 * \endcode
 *
 * The name given as parameter of the Elements::Logging::getLogger method can be
 * used for identification of the log messages and for further tuning of the
 * underlying logging framework.
 *
 * The format of the logged messages follows the format:
 * \code
 * YYYY-MM-DDTHH:MM:SSZ LOGGER LEVEL : MESSAGE
 * \endcode
 *
 * For example, the previous code snippet will produce the following messages:
 *
 * \code
 * 2014-03-17T16:20:20CET name DEBUG : A debug message
 * 2014-03-17T16:20:20CET name  INFO : A value 15 in a printf style info message
 * 2014-03-17T16:20:20CET name ERROR : A value 15 in a steam style error message
 * \endcode
 *
 * By default the logging level is set to INFO and the default behavior is to
 * forward all the messages to the standard error stream. This behavior can be
 * modified by using the method Elements::Logging::setLevel, which can be used
 * to set the level to show, and the method Elements::Logging::setLogFile, which
 * can be used to redirect the messages in a file (in addition to the standard
 * error stream). Note that these methods have a global effect to the application.
 *
 * If the Elements::Program API is used, the logging level and the log file
 * can be set by using the command line parameters <b>--log-level</b> and
 * <b>--log-file</b> and no direct use of the Elements::Logging::setLevel and
 * Elements::Logging::setLogFile should be performed. Exception of this rule is
 * any log messages which are sent before the command line parameters are handled
 * (like global or static variable initializations, or any other action before
 * the call of the main method). These messages (without an explicit call to the
 * Elements::Logging::setLogFile method) will only appear in the standard error
 * stream.
 */
class ELEMENTS_API Logging {

private:

  // We declare the LogMessageStream here because it is used from the public
  // functions. It is defined in the private section at the end.
  class LogMessageStream;

public:

  /**
   * Returns an instance of Elements::Logging which can be used for logging
   * messages of different severities.
   * @param name The name to use for identifying the logger messages
   * @return A logger instance
   */
  static Logging getLogger(const std::string& name = "");

  /**
   * @brief
   * Sets the global message level
   * @details
   * This call has effect to all the loggers already retrieved as well as loggers
   * which will be retrieved in the future. Exceptions are loggers which have
   * been fine tuned using the underlying framework configuration methods.
   *
   * @param level The new message level
   */
  static void setLevel(std::string level);

  /**
   * @brief
   * Sets the file to store the log messages
   * @details
   * This call has effect to all the loggers already retrieved, as well as loggers
   * which will be retrieved in the future. There can be only one global file
   * in which the messages are redirected. Multiple calls of this method will
   * just switch the file the messages are stored in. Note that this method does
   * not affect any file handlers attached to specific loggers by using the
   * underlying framework configuration methods.
   *
   * If an empty string is given as fileName, then the loggers will stop storing
   * the messages in any files.
   *
   * @param fileName The file where the log messages will be stored
   */
  static void setLogFile(const boost::filesystem::path& fileName);

  /**
   * Logs a debug message.
   * @param logMessage The message to log
   */
  void debug(const std::string& logMessage) {
    m_log4cppLogger.debug(logMessage);
  }

  /**
   * Logs a debug message using format specifiers.
   * @param stringFormat The message containing the format specifiers
   * @param args The values to replace the format specifiers with
   */
  template<typename ...Args>
  void debug(const char *stringFormat, Args &&...args) {
    m_log4cppLogger.debug(stringFormat, std::forward<Args>(args)...);
  }

  /**
   * Returns an object which can be used for logging a debug message using the
   * "<<" operator.
   * @return An object used for logging a debug message using the "<<" opearator
   */
  LogMessageStream debug() {
    return LogMessageStream {m_log4cppLogger, &log4cpp::Category::debug};
  }

  /**
   * Logs an info message.
   * @param logMessage The message to log
   */
  void info(const std::string& logMessage) {
    m_log4cppLogger.info(logMessage);
  }

  /**
   * Logs an info message using format specifiers.
   * @param stringFormat The message containing the format specifiers
   * @param args The values to replace the format specifiers with
   */
  template<typename ...Args>
  void info(const char *stringFormat, Args &&...args) {
    m_log4cppLogger.info(stringFormat, std::forward<Args>(args)...);
  }

  /**
   * Returns an object which can be used for logging a info message using the
   * "<<" operator.
   * @return An object used for logging a info message using the "<<" opearator
   */
  LogMessageStream info() {
    return LogMessageStream {m_log4cppLogger, &log4cpp::Category::info};
  }

  /**
   * Logs a warning message.
   * @param logMessage The message to log
   */
  void warn(const std::string& logMessage) {
    m_log4cppLogger.warn(logMessage);
  }

  /**
   * Logs a warning message using format specifiers.
   * @param stringFormat The message containing the format specifiers
   * @param args The values to replace the format specifiers with
   */
  template<typename ...Args>
  void warn(const char *stringFormat, Args &&...args) {
    m_log4cppLogger.warn(stringFormat, std::forward<Args>(args)...);
  }

  /**
   * Returns an object which can be used for logging a warn message using the
   * "<<" operator.
   * @return An object used for logging a warn message using the "<<" opearator
   */
  LogMessageStream warn() {
    return LogMessageStream {m_log4cppLogger, &log4cpp::Category::warn};
  }

  /**
   * Logs an error message.
   * @param logMessage The message to log
   */
  void error(const std::string& logMessage) {
    m_log4cppLogger.error(logMessage);
  }

  /**
   * Logs an error message using format specifiers.
   * @param stringFormat The message containing the format specifiers
   * @param args The values to replace the format specifiers with
   */
  template<typename ...Args>
  void error(const char *stringFormat, Args &&...args) {
    m_log4cppLogger.error(stringFormat, std::forward<Args>(args)...);
  }

  /**
   * Returns an object which can be used for logging a error message using the
   * "<<" operator.
   * @return An object used for logging a error message using the "<<" opearator
   */
  LogMessageStream error() {
    return LogMessageStream {m_log4cppLogger, &log4cpp::Category::error};
  }

  /**
   * Logs a fatal message.
   * @param logMessage The message to log
   */
  void fatal(const std::string& logMessage) {
    m_log4cppLogger.fatal(logMessage);
  }

  /**
   * Logs a fatal message using format specifiers.
   * @param stringFormat The message containing the format specifiers
   * @param args The values to replace the format specifiers with
   */
  template<typename ...Args>
  void fatal(const char *stringFormat, Args &&...args) {
    m_log4cppLogger.fatal(stringFormat, std::forward<Args>(args)...);
  }

  /**
   * Returns an object which can be used for logging a fatal message using the
   * "<<" operator.
   * @return An object used for logging a fatal message using the "<<" opearator
   */
  LogMessageStream fatal() {
    return LogMessageStream {m_log4cppLogger, &log4cpp::Category::fatal};
  }


  /**
   * Logs a message.
   * @param level The logging level of the message
   * @param logMessage The message to log
   */
  void log(log4cpp::Priority::Value level, const std::string& logMessage) {
    m_log4cppLogger.log(level, logMessage);
  }

  /**
   * Logs an log message using a level and format specifiers.
   * @param level The logging level of the message
   * @param stringFormat The message containing the format specifiers
   * @param args The values to replace the format specifiers with
   */
  template<typename ...Args>
  void log(log4cpp::Priority::Value level, const char *stringFormat, Args &&...args) {
    m_log4cppLogger.log(level, stringFormat, std::forward<Args>(args)...);
  }

private:

  explicit Logging(log4cpp::Category& log4cppLogger);

  log4cpp::Category& m_log4cppLogger;

  /**
   * @class LogMessageStream
   * @brief A helper class for logging messages using the "<<" operator
   * @details
   * Each instance of the LogMessageStream class is used for logging one single
   * message. It keeps a reference of the logger to use and a pointer of the
   * related function (to allow different logging levels). The message is logged
   * during the destruction of the object. Instances can only be retrieved by
   * using the Elements::Logging::debug, Elements::Logging::info, etc methods.
   */
  class LogMessageStream {
    // The P_log_func is a pointer to member function. If you have no idea what
    // this is don't get scared! Just have a look in the following link:
    // http://www.codeproject.com/Articles/7150/Member-Function-Pointers-and-the-Fastest-Possible
    using P_log_func = void (log4cpp::Category::*)(const std::string&);
  public:
    LogMessageStream(log4cpp::Category& logger, P_log_func log_func);
    LogMessageStream(LogMessageStream&& other);
    LogMessageStream(const LogMessageStream& other);
    ~LogMessageStream();
    template <typename T>
    LogMessageStream& operator<<(const T& m) {
      m_message << m;
      return *this;
    }
  private:
    log4cpp::Category& m_logger;
    P_log_func m_log_func;
    std::stringstream m_message {};
  };

};

}  // namespace Elements

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_LOGGING_H_

/**@}*/
