/**
 * @file ElementsProgram.cpp
 * 
 * Created on: Jul 18, 2013
 * 
 * @author dubath
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * This class represents ...
 */

#include <string>
#include <fstream>
#include <iostream>
#include "ElementsKernel/EuclidException.h"
#include "ElementsKernel/ElementsProgram.h"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "ElementsKernel/ElementsLogging.h"

using namespace std;

ElementsProgram::ElementsProgram() {
	// TODO Auto-generated constructor stub
}

ElementsProgram::~ElementsProgram() {
	// TODO Auto-generated destructor stub
}

const boost::filesystem::path ElementsProgram::getDefaultConfigFile(
		std::string programName) const {

	boost::filesystem::path pn(programName);

	pn.replace_extension("conf");

	boost::filesystem::path programRootPath = getenv("ELEMENTSEXAMPLESROOT");

	boost::filesystem::path defaultConfigFile = programRootPath / "conf" / pn;

	return defaultConfigFile;

}

const boost::filesystem::path ElementsProgram::getDefaultLogFile(
		std::string programName) const {

	boost::filesystem::path pn(programName);

	pn.replace_extension("log");

	boost::filesystem::path defaultLogFile = "." / pn;

	return defaultLogFile;

}

const boost::program_options::variables_map ElementsProgram::getProgramOptions(
		int argc, const char* argv[]) {

	int defaultLogLevel = 400;

	boost::filesystem::path configFile;
	std::string programName = argv[0];

	boost::filesystem::path defaultConfigFile = getDefaultConfigFile(
			programName);
	boost::filesystem::path defaultLogFile = getDefaultLogFile(programName);

	/**
	 * Declare a group of generic options that will be allowed only on command line
	 *
	 * The path to the configuration file can either be an absolute path or a relative one to the
	 * execution directory (build...)
	 *
	 */
	po::options_description genericOptions("Generic options");
	genericOptions.add_options()
	//("version,v", "Print version string")
	("version", "Print version string")("help", "Produce help message")(
			"config-file",
			po::value<boost::filesystem::path>(&configFile)->default_value(
					defaultConfigFile),
			"Name of a configuration file (default programName.conf")(
			"log-level", po::value<int>()->default_value(defaultLogLevel),
			"Log level: NONE=0, FATAL=100, ERROR=200, WARN=300, INFO=400 (default), DEBUG=500")(
			"log-file",
			po::value<boost::filesystem::path>(&m_logFile)->default_value(
					defaultLogFile),
			"Name of a log file (default: programName.log)");
	/**
	 * Document all program options specific to this program
	 */
	po::options_description configFileOptions =
			this->defineSpecificProgramOptions();

	/**
	 * Group all options together
	 */
	po::options_description cmdlineOptions;
	cmdlineOptions.add(genericOptions).add(configFileOptions);

	//  /**
	//   * These two lines make the specification of the --config="..." optional.
	//   * I.e.,
	//   *  ExampleProgram --config="../ElementsExamples/src/main/ExampleMain.cfg"
	//   *
	//   *  can be replace by
	//   *
	//   *  ExampleProgram "../ElementsExamples/src/main/ExampleMain.cfg"
	//   */
	//  po::positional_options_description p;
	//  p.add("config", -1);
	//  po::store(po::command_line_parser(argc, argv).
	//              options(cmdline_options).positional(p).run(), variables_map);

	/**
	 * Parse the command line and store the options in the variable map
	 */
	po::store(po::command_line_parser(argc, argv).options(cmdlineOptions).run(),
			m_variablesMap);
	notify(m_variablesMap);

	if (m_variablesMap.count("help")) {
		cout << cmdlineOptions << "\n";
		exit(0);
	}

	// TODO get the system version
	if (m_variablesMap.count("version")) {
		cout << "Version : " << programName << this->getVersion() << endl;
		exit(0);
	}

	/**
	 * Open the configuration file
	 */
	ifstream ifs(configFile.c_str());
	if (!ifs) {
		stringstream errorBuffer;
		errorBuffer << "Cannot open config file: " << configFile << "\n";
		throw EuclidException(errorBuffer.str());
	} else {
		/**
		 * Read the configuration file into the variable map, taking values only if they are not already specified on the command line
		 */
		store(parse_config_file(ifs, configFileOptions), m_variablesMap);
		notify(m_variablesMap);
	}

	return m_variablesMap;
}

void ElementsProgram::logAllOptions(string programName) {

	ElementsLogging& logger = ElementsLogging::getLogger();

		logger.info("##########################################################");
		logger.info("##########################################################");
		logger.info("#");
		logger.info("#    %s start ", programName.c_str());
		logger.info("#");
		logger.info("##########################################################");
		logger.info("#");
		logger.info("# List of all program options");
		logger.info("# ---------------------------");
		logger.info("#");

		// Build a log message
		stringstream logMessage { };

		// Loop over all options included in the variable_map
		for (po::variables_map::iterator iter = m_variablesMap.begin();
				iter != m_variablesMap.end(); ++iter) {
			// string option
			if (iter->second.value().type() == typeid(string)) {
				logMessage << iter->first << " = " << iter->second.as<string>();
			// double option
			} else if (iter->second.value().type() == typeid(double)) {
				logMessage << iter->first << " = " << iter->second.as<double>();
			// int64_t option
			} else if (iter->second.value().type() == typeid(int64_t)) {
				logMessage << iter->first << " = " << iter->second.as<int64_t>();
			// int option
			} else if (iter->second.value().type() == typeid(int)) {
				logMessage << iter->first << " = " << iter->second.as<int>();
			// boost::filesystem::path option
			} else if (iter->second.value().type() == typeid(boost::filesystem::path)) {
				logMessage << iter->first << " = " << iter->second.as<boost::filesystem::path>();
			// int vector option
			} else if (iter->second.value().type() == typeid(vector<int>)) {
					vector<int> intVec = iter->second.as<vector<int>>();
					stringstream vecContent {};
					for(std::vector<int>::iterator it = intVec.begin(); it != intVec.end(); ++it) {
						vecContent << " " << *it;
					}
					logMessage << iter->first << " = {" << vecContent.str() << " }";
			// double vector option
			} else if (iter->second.value().type() == typeid(vector<double>)) {
					vector<double> intVec = iter->second.as<vector<double>>();
					stringstream vecContent {};
					for(std::vector<double>::iterator it = intVec.begin(); it != intVec.end(); ++it) {
						vecContent << " " << *it;
					}
					logMessage << iter->first << " = {" << vecContent.str() << " }";
			// string vector option
			} else if (iter->second.value().type() == typeid(vector<string>)) {
				vector<string> intVec = iter->second.as<vector<string>>();
				stringstream vecContent {};
				for(std::vector<string>::iterator it = intVec.begin(); it != intVec.end(); ++it) {
					vecContent << " " << *it;
				}
				logMessage << iter->first << " = {" << vecContent.str() << " }";
			// if nothing else
			} else {
				logMessage << "Option " << iter->first << " of type " << iter->second.value().type().name()
						<< " not supported in logging !" << "\n";
			}
			// write the log message
			logger.info(logMessage.str());
			logMessage.str("");
		}
		logger.info("#");

}

void ElementsProgram::setup(int argc, const char* argv[]) {

	// get all program options into the varaiable_map
	m_variablesMap = getProgramOptions(argc, argv);

	// get the program options related to the logging
	ElementsLogging::LoggingLevel loggingLevel =
			(ElementsLogging::LoggingLevel) m_variablesMap["log-level"].as<int>();
	boost::filesystem::path logFileName = m_variablesMap["log-file"].as<
			boost::filesystem::path>();

	// setup the logging
	ElementsLogging::setupLogger(loggingLevel, logFileName);

	// log all program options
	this->logAllOptions(argv[0]);

}

