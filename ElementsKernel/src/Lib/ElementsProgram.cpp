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

#include <fstream>
#include <iostream>
#include "ElementsKernel/EuclidException.h"
#include "ElementsKernel/ElementsProgram.h"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"



using namespace std;

ElementsProgram::ElementsProgram() {
  // TODO Auto-generated constructor stub
}

ElementsProgram::~ElementsProgram() {
  // TODO Auto-generated destructor stub
}

const boost::filesystem::path ElementsProgram::getDefaultConfigFile(std::string programName) const {

  boost::filesystem::path pn(programName);

  pn.replace_extension("cfg");

  boost::filesystem::path programRootPath = getenv("ELEMENTSEXAMPLESROOT");

  boost::filesystem::path defaultConfigFile = programRootPath / "conf" / pn;

  return defaultConfigFile;

}

const boost::program_options::variables_map ElementsProgram::getProgramOptions(int argc, const char* argv[]) {

   boost::filesystem::path configFile;
   std::string programName = argv[0];

   boost::filesystem::path defaultConfigFile = getDefaultConfigFile(programName);

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
       ("version", "Print version string")
       ("help","Produce help message")
       ("config-file", po::value<boost::filesystem::path>(&configFile)->default_value(defaultConfigFile
           ), "Name of a configuration file.");

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
     errorBuffer << "Can not open config file: " << configFile << "\n";
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

void ElementsProgram::loggingTest() {


  log4cpp::Appender *consoleAppender = new log4cpp::OstreamAppender("console", &std::cout);
  consoleAppender->setLayout(new log4cpp::BasicLayout());

  log4cpp::Appender *fileAppender = new log4cpp::FileAppender("file", "program.log");
  fileAppender->setLayout(new log4cpp::BasicLayout());

  log4cpp::Category& logger = log4cpp::Category::getInstance(std::string("logger"));
  logger.setPriority(log4cpp::Priority::INFO);
  logger.addAppender(consoleAppender);
  logger.addAppender(fileAppender);

  // use of functions for logging messages
//  root.error("root error");
//  root.info("root info");
  logger.error("Error function log");
  logger.warn("Warn function log");


  // printf-style for logging variables
  logger.warn("%d + %d == %s ?", 1, 1, "two");

  // use of streams for logging messages
//  root << log4cpp::Priority::ERROR << "Streamed root error";
//  root << log4cpp::Priority::INFO << "Streamed root info";
  logger << log4cpp::Priority::ERROR << "Streamed logger error";
  logger << log4cpp::Priority::WARN << "Streamed logger warn";
  logger << log4cpp::Priority::INFO << "Streamed logger info";

  // or this way:
  logger.errorStream() << "Another logger streamed error";
}

void ElementsProgram::setup(int argc, const char* argv[]) {

 m_variablesMap = getProgramOptions(argc, argv);

 loggingTest();

}
