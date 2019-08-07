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

#include "ElementsKernel/Logging.h"              // for Logging, etc

#include <iostream>                              // for operator<<, stringstream, etc
#include <map>                                   // for map
#include <memory>                                // for unique_ptr
#include <sstream>                               // for stringstream
#include <string>                                // for char_traits, string

#include <boost/algorithm/string/case_conv.hpp>  // for to_upper
#include <boost/filesystem/path.hpp>             // for path

#include <log4cpp/Category.hh>                   // for Category
#include <log4cpp/FileAppender.hh>               // for FileAppender
#include <log4cpp/OstreamAppender.hh>            // for OstreamAppender
#include <log4cpp/PatternLayout.hh>              // for PatternLayout
#include <log4cpp/Priority.hh>                   // for Priority, Priority::::INFO, etc

#include "ElementsKernel/Exception.h"   // for Exception

using std::string;
using std::unique_ptr;
using log4cpp::Category;
using log4cpp::Priority;
using log4cpp::Layout;

namespace Elements {

static const std::map<string, const int> LOG_LEVEL {{"FATAL", Priority::FATAL},
                                                    {"ERROR", Priority::ERROR},
                                                    {"WARN", Priority::WARN},
                                                    {"INFO", Priority::INFO},
                                                    {"DEBUG", Priority::DEBUG}};

unique_ptr<Layout> getLogLayout() {
  log4cpp::PatternLayout* layout = new log4cpp::PatternLayout {};
  layout->setConversionPattern("%d{%FT%T%Z} %c %5p : %m%n");
  return unique_ptr<Layout>(layout);
}

Logging::Logging(Category& log4cppLogger)
    : m_log4cppLogger(log4cppLogger) { }

Logging Logging::getLogger(const string& name) {
  if (Category::getRoot().getAppender("console") == NULL) {
    log4cpp::OstreamAppender* consoleAppender = new log4cpp::OstreamAppender {"console", &std::cerr};
    consoleAppender->setLayout(getLogLayout().release());
    Category::getRoot().addAppender(consoleAppender);
    if (Category::getRoot().getPriority() == Priority::NOTSET) {
      Category::setRootPriority(Priority::INFO);
    }
  }
  return Logging {Category::getInstance(name)};
}


void Logging::setLevel(string level) {
  boost::to_upper(level);
  auto it = LOG_LEVEL.find(level);
  if ( it != LOG_LEVEL.end() ) {
    Category::setRootPriority(it->second);
  } else {
    std::stringstream error_buffer;
    error_buffer << "Unrecognized logging level: " << level << std::endl;
    throw Exception(error_buffer.str());
  }
}

void Logging::setLogFile(const boost::filesystem::path& fileName) {
  Category& root = Category::getRoot();
  root.removeAppender(root.getAppender("file"));
  if (fileName.has_filename()) {
    log4cpp::FileAppender* fileAppender = new log4cpp::FileAppender("file", fileName.string());
    fileAppender->setLayout(getLogLayout().release());
    root.addAppender(fileAppender);
  }
  root.setPriority(root.getPriority());
}

/// @cond Doxygen_Suppress
Logging::LogMessageStream::LogMessageStream(Category& logger, P_log_func log_func)
    : m_logger(logger), m_log_func{log_func} { }
/// @endcond Doxygen_Suppress

Logging::LogMessageStream::LogMessageStream(LogMessageStream&& other)
    : m_logger(other.m_logger), m_log_func{other.m_log_func} { }

Logging::LogMessageStream::LogMessageStream(const LogMessageStream& other)
    : m_logger(other.m_logger), m_log_func{other.m_log_func} { }


Logging::LogMessageStream::~LogMessageStream() {
  (m_logger.*m_log_func) (m_message.str());
}


}  // namespace Elements
