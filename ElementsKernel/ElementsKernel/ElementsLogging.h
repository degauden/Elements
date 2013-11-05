/*
 * ElementsLogging.h
 *
 *  Created on: Jul 29, 2013
 *      Author: dubath
 */

#ifndef ELEMENTSLOGGING_H_
#define ELEMENTSLOGGING_H_

#include <cstdarg>
#include <memory>
//#include <mutex>
#include <boost/filesystem.hpp>

#include "log4cpp/Category.hh"

#include "ElementsKernel/Export.h" // ELEMENTS_API

/**
 * This is a singleton logger class which provide a unique logger throughout a program
 */
class ElementsLogging {

public:

	typedef enum {
		NONE = 0,
		FATAL = 100,
		ERROR = 200,
		WARN = 300,
		INFO = 400,
		DEBUG = 500
	} LoggingLevel;

	ELEMENTS_API static ElementsLogging& getLogger();

	static void setupLogger(ElementsLogging::LoggingLevel loggingLevel, boost::filesystem::path logFileName);

	void debug(std::string logMessage) {
		m_log4cppLogger.debug(logMessage);
	}
	template<typename ...Args>
	void debug(const char *stringFormat, Args &&...args) {
		m_log4cppLogger.debug(stringFormat, std::forward<Args>(args)...);
	}

	void info(std::string logMessage) {
		m_log4cppLogger.info(logMessage);
	}
	template<typename ...Args>
	void info(const char *stringFormat, Args &&...args) {
		m_log4cppLogger.info(stringFormat, std::forward<Args>(args)...);
	}

	void warn(std::string logMessage) {
		m_log4cppLogger.warn(logMessage);
	}
	template<typename ...Args>
	void warn(const char *stringFormat, Args &&...args) {
		m_log4cppLogger.warn(stringFormat, std::forward<Args>(args)...);
	}

	void error(std::string logMessage) {
		m_log4cppLogger.error(logMessage);
	}
	template<typename ...Args>
	void error(const char *stringFormat, Args &&...args) {
		m_log4cppLogger.error(stringFormat, std::forward<Args>(args)...);
	}

	void fatal(std::string logMessage) {
		m_log4cppLogger.fatal(logMessage);
	}
	template<typename ...Args>
	void fatal(const char *stringFormat, Args &&...args) {
		m_log4cppLogger.fatal(stringFormat, std::forward<Args>(args)...);
	}

private:

	static ElementsLogging::LoggingLevel s_logging_level;
	static boost::filesystem::path s_log_file_name;

	static std::unique_ptr<ElementsLogging> s_instance;

	//ElementsLogging() {}
	ElementsLogging();

	/**
	 * Prevent any Copy constructor and Equal operator implementation
	 */
	ElementsLogging(ElementsLogging const& el) = delete;
	void operator=(ElementsLogging const& el) = delete;

	void initializeLogger();

	log4cpp::Category& m_log4cppLogger = log4cpp::Category::getInstance(
			std::string("Elements"));

};

#endif /* ELEMENTSLOGGING_H_ */
