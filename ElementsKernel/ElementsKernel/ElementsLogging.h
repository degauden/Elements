/**
 * @file ElementsLogging.h
 * @date January 13, 2014
 * @author Nikolaos Apostolakos
 */

#ifndef ELEMENTSLOGGING_H
#define	ELEMENTSLOGGING_H

#include <string>
#include <boost/filesystem.hpp>
#include <log4cpp/Category.hh>
#include "ElementsKernel/Export.h" // ELEMENTS_API

/**
 * @class ElementsLogging
 * 
 * @brief Logging API of the Elements framework
 * 
 * 
 * The ElementsLogging class provides the logging API of the Elements framework.
 * To use the logging API the ElementsLogging::getLogger method can be used to
 * retrieve a logger instance, which can be further used for logging messages 
 * of different severities. For example:
 * 
 * \code
 * ElementsLogging logger = ElementsLogging::getLogger("name");
 * logger.debug("A debug message");
 * logger.info("A value %d in a printf style info message", 15);
 * logger.error("Something went seriously wrong");
 * \endcode
 * 
 * The name given as parameter of the ElementsLogging::getLogger method can be
 * used for identification of the log messages and for further tuning of the
 * underlying logging framework.
 * 
 * By default the logging level is set to INFO and the default behavior is to
 * forward all the messages to the standard error stream. This behavior can be
 * modified by using the method ElementsLogging::setLevel, which can be used
 * to set the level to show, and the method ElementsLogging::setLogFile, which
 * can be used to redirect the messages in a file (in addition to the standard
 * error stream). Note that these methods have a global effect to the application.
 * 
 * If the ElementsProgram API is used, the logging level and the log file
 * can be set by using the command line parameters <b>--log-level</b> and
 * <b>--log-file</b> and no direct use of the ElementsLogging::setLevel and
 * ElementsLogging::setLogFile methods is necessary. Exception of this rule is
 * any log messages which are sent before the command line parameters are handled
 * (like global or static variable initializations, or any other action before
 * the call of the main method). These messages (without an explicit call to the
 * ElementsLogging::setLogFile method) will only appear in the standard error
 * stream.
 */
class ElementsLogging {
  
public:

  /**
   * The different available levels of logging
   */
  typedef enum {
    /// Panic messages (affecting multiple apps/servers/sites)
    NONE = 0,
    /// Very severe error events that will lead the application to abort
    FATAL = 100,
    /// Error events that might still allow the application to continue running
    ERROR = 200,
    /// Designates potentially harmful situations
    WARN = 300,
    /// Informational messages that highlight the progress of the application
    INFO = 400,
    /// Fine-grained informational events
    DEBUG = 500
  } LoggingLevel;
  
  /**
   * Returns an instance of ElementsLogging which can be used for logging
   * messages of different severities.
   * @param name The name to use for identifying the logger messages
   * @return A logger instance
   */
  ELEMENTS_API static ElementsLogging getLogger(const std::string& name);
  
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
  static void setLevel(LoggingLevel level);
  
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
  
private:
  
  ElementsLogging(log4cpp::Category& log4cppLogger);
  
  log4cpp::Category& m_log4cppLogger;
  
}; /* ElementsLogging */

#endif	/* ELEMENTSLOGGING_H */

