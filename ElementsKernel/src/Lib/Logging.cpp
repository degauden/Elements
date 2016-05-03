/**
 * @file Logging.cpp
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
 *
 */

#include "ElementsKernel/Logging.h"     // for Logging, etc


#include <cstddef>                      // for NULL
#include <iostream>                     // for operator<<, stringstream, etc
#include <boost/filesystem.hpp>
#include <log4cpp/Category.hh>          // for Category
#include <log4cpp/FileAppender.hh>      // for FileAppender
#include <log4cpp/Layout.hh>            // for Layout
#include <log4cpp/OstreamAppender.hh>   // for OstreamAppender
#include <log4cpp/PatternLayout.hh>     // for PatternLayout
#include <log4cpp/Priority.hh>          // for Priority, Priority::::INFO, etc
#include <memory>                       // for unique_ptr
#include <string>                       // for char_traits, string
#include <boost/algorithm/string.hpp>

#include "ElementsKernel/Exception.h"   // for Exception



namespace Elements {

static const std::map<std::string, const int> LOG_LEVEL {{"FATAL", log4cpp::Priority::FATAL},
                                                         {"ERROR", log4cpp::Priority::ERROR},
                                                         {"WARN", log4cpp::Priority::WARN},
                                                         {"INFO", log4cpp::Priority::INFO},
                                                         {"DEBUG", log4cpp::Priority::DEBUG}};



std::unique_ptr<log4cpp::Layout> getLogLayout() {
  log4cpp::PatternLayout* layout = new log4cpp::PatternLayout {};
  layout->setConversionPattern("%d{%FT%T%Z} %c %5p : %m%n");
  return std::unique_ptr<log4cpp::Layout>(layout);
}

Logging::Logging(log4cpp::Category& log4cppLogger)
    : m_log4cppLogger(log4cppLogger) { }

Logging Logging::getLogger(const std::string& name) {
  if (log4cpp::Category::getRoot().getAppender("console") == NULL) {
    log4cpp::OstreamAppender* consoleAppender = new log4cpp::OstreamAppender {"console", &std::cerr};
    consoleAppender->setLayout(getLogLayout().release());
    log4cpp::Category::getRoot().addAppender(consoleAppender);
    if (log4cpp::Category::getRoot().getPriority() == log4cpp::Priority::NOTSET) {
      log4cpp::Category::setRootPriority(log4cpp::Priority::INFO);
    }
  }
  return Logging {log4cpp::Category::getInstance(name)};
}


void Logging::setLevel(std::string level) {
  boost::to_upper(level);
  auto it = LOG_LEVEL.find(level);
  if ( it != LOG_LEVEL.end() ) {
    log4cpp::Category::setRootPriority(it->second);
  } else {
    std::stringstream error_buffer;
    error_buffer << "Unrecognized logging level: " << level << std::endl;
    throw Exception(error_buffer.str());
  }
}

void Logging::setLogFile(const boost::filesystem::path& fileName) {
  log4cpp::Category& root = log4cpp::Category::getRoot();
  root.removeAppender(root.getAppender("file"));
  if (fileName.has_filename()) {
    log4cpp::FileAppender* fileAppender = new log4cpp::FileAppender("file", fileName.string());
    fileAppender->setLayout(getLogLayout().release());
    root.addAppender(fileAppender);
  }
  root.setPriority(root.getPriority());
}

Logging::LogMessageStream::LogMessageStream(log4cpp::Category& logger, P_log_func log_func)
    : m_logger(logger), m_log_func{log_func} { }

Logging::LogMessageStream::LogMessageStream(LogMessageStream&& other)
    : m_logger(other.m_logger), m_log_func{other.m_log_func} { }

Logging::LogMessageStream::~LogMessageStream() {
  (m_logger.*m_log_func) (m_message.str());
}


} // Elements namespace
