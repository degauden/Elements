/*
 * ElementsLogging.cpp
 *
 *  Created on: Jul 29, 2013
 *      Author: dubath
 */

#include <memory>
#include <mutex>
#include <fstream>
#include <iostream>

#include "ElementsKernel/ElementsException.h"
#include "ElementsKernel/ElementsLogging.h"

#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"


#include <boost/filesystem.hpp>

using namespace std;


ElementsLogging::ElementsLogging() {
}


boost::filesystem::path ElementsLogging::s_logFileName = "default.log";
ElementsLogging::LoggingLevel ElementsLogging::s_loggingLevel = ElementsLogging::LoggingLevel::INFO;

void ElementsLogging::setupLogger(ElementsLogging::LoggingLevel loggingLevel, boost::filesystem::path logFileName) {
	ElementsLogging::s_loggingLevel = loggingLevel;
	ElementsLogging::s_logFileName = logFileName;
}

void ElementsLogging::initializeLogger() {

	switch(s_loggingLevel)
	{
	case ElementsLogging::LoggingLevel::DEBUG:
		m_log4cppLogger.setPriority(log4cpp::Priority::DEBUG);
		break;
	case ElementsLogging::LoggingLevel::INFO:
			m_log4cppLogger.setPriority(log4cpp::Priority::INFO);
			break;
	case ElementsLogging::LoggingLevel::WARN:
			m_log4cppLogger.setPriority(log4cpp::Priority::WARN);
			break;
	case ElementsLogging::LoggingLevel::ERROR:
			m_log4cppLogger.setPriority(log4cpp::Priority::ERROR);
			break;
	case ElementsLogging::LoggingLevel::FATAL:
			m_log4cppLogger.setPriority(log4cpp::Priority::FATAL);
			break;
	case ElementsLogging::LoggingLevel::NONE:
			m_log4cppLogger.setPriority(log4cpp::Priority::EMERG);
			break;
	default:
		stringstream errorBuffer;
		errorBuffer << "Unrecognized logging level: " << s_loggingLevel << "\n";
		throw ElementsException(errorBuffer.str());
		break;
	}

	log4cpp::Appender *consoleAppender = new log4cpp::OstreamAppender("console", &std::cout);
	consoleAppender->setLayout(new log4cpp::BasicLayout());

	log4cpp::Appender *fileAppender = new log4cpp::FileAppender("file", s_logFileName.string());
	fileAppender->setLayout(new log4cpp::BasicLayout());

	m_log4cppLogger.addAppender(consoleAppender);
	m_log4cppLogger.addAppender(fileAppender);

}



std::unique_ptr<ElementsLogging> ElementsLogging::s_instance = nullptr;

ElementsLogging& ElementsLogging::getLogger() {
	if (!s_instance) {
		ElementsLogging* el = new ElementsLogging();
		el->initializeLogger();
		s_instance.reset(el);
	}
	return *(s_instance.get());
}


//static ElementsLogging& ElementsLogging::getLogger(boost::filesystem::path logFile) {
//	static ElementsLogging logger(logFile); // Instantiated on first use and guaranteed to be destroyed in the end
//	return logger;
//}


//	// use of functions for logging messages
////  root.error("root error");
////  root.info("root info");
//	log4cppLogger.rror function log");
//	log4cppLogger.warn("Warn function log");
//
//	// printf-style for logging variables
//	log4cppLogger.warn("%d + %d == %s ?", 1, 1, "two");
//
//	// use of streams for logging messages
////  root << log4cpp::Priority::ERROR << "Streamed root error";
////  root << log4cpp::Priority::INFO << "Streamed root info";
//	log4cppLogger << log4cpp::Priority::ERROR << "Streamed logger error";
//	log4cppLogger << log4cpp::Priority::WARN << "Streamed logger warn";
//	log4cppLogger << log4cpp::Priority::INFO << "Streamed logger info";
//
//	// or this way:
//	log4cppLogger.errorStream() << "Another logger streamed error";
//}

//static ElementsLogging& ElementsLogging::getLogger() {
//	static ElementsLogging logger;
//	return logger;
//}

