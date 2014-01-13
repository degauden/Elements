/**
 * @file ElementsLogging.cpp
 * @date January 13, 2014
 * @author Nikolaos Apostolakos
 */

#include <sstream>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include "ElementsKernel/ElementsException.h"
#include "ElementsKernel/ElementsLogging.h"

ElementsLogging::ElementsLogging(log4cpp::Category& log4cppLogger)
    : m_log4cppLogger(log4cppLogger) { }
    
ElementsLogging ElementsLogging::getLogger(const std::string& name) {
  if (log4cpp::Category::getRoot().getAppender("console") == NULL) {
    log4cpp::Category::getRoot().addAppender(new log4cpp::OstreamAppender("console", &std::cerr));
  }
  return ElementsLogging {log4cpp::Category::getInstance(name)};
}

void ElementsLogging::setLevel(ElementsLogging::LoggingLevel level) {
  switch (level) {
  case ElementsLogging::LoggingLevel::DEBUG:
    log4cpp::Category::getRoot().setPriority(log4cpp::Priority::DEBUG);
    break;
  case ElementsLogging::LoggingLevel::INFO:
    log4cpp::Category::getRoot().setPriority(log4cpp::Priority::INFO);
    break;
  case ElementsLogging::LoggingLevel::WARN:
    log4cpp::Category::getRoot().setPriority(log4cpp::Priority::WARN);
    break;
  case ElementsLogging::LoggingLevel::ERROR:
    log4cpp::Category::getRoot().setPriority(log4cpp::Priority::ERROR);
    break;
  case ElementsLogging::LoggingLevel::FATAL:
    log4cpp::Category::getRoot().setPriority(log4cpp::Priority::FATAL);
    break;
  case ElementsLogging::LoggingLevel::NONE:
    log4cpp::Category::getRoot().setPriority(log4cpp::Priority::EMERG);
    break;
  default:
    std::stringstream error_buffer;
    error_buffer << "Unrecognized logging level: " << level << "\n";
    throw ElementsException(error_buffer.str());
    break;
  }
}

void ElementsLogging::setLogFile(const boost::filesystem::path& fileName) {
  log4cpp::Category& root = log4cpp::Category::getRoot();
  root.removeAppender(root.getAppender("file"));
  if (fileName.has_filename()) {
    root.addAppender(new log4cpp::FileAppender("file", fileName.string()));
  }
}
