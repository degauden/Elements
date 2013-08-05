/**
 * @file ElementsProgram.h
 * 
 * Created on: Jul 18, 2013
 * 
 * @author dubath
 *
 * @section DESCRIPTION
 *
 * This class represents ...
 */

#ifndef ELEMENTSPROGRAM_H_
#define ELEMENTSPROGRAM_H_

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include "ElementsKernel/ElementsLogging.h"


/*
 *
 *  The ElementsProgramExample shows an example of how a program should be
 *  implemented. There are three required files, the ElementsProgramExample
 */
class ElementsProgram {
public:
  void setup(int argc, const char* argv[]);

protected:
  ElementsProgram();
  virtual ~ElementsProgram();

  /**
   * The pseudo main that all programs must implement.
   */
  virtual int pseudo_main() = 0;

  virtual boost::program_options::options_description defineSpecificProgramOptions() = 0;

  virtual std::string getVersion() = 0;

  const boost::program_options::variables_map& getVariablesMap() const {
     return m_variablesMap;
   }

private:

  const boost::filesystem::path getDefaultConfigFile(std::string programName) const;
  const boost::filesystem::path getDefaultLogFile(std::string programName) const;

  const boost::program_options::variables_map getProgramOptions(int argc, const char* argv[]);

  void logAllOptions(std::string programName);

  boost::filesystem::path m_logFile;
  /**
   * This is the BOOST program options variable_map used to store all program options
   */
  boost::program_options::variables_map m_variablesMap { };

};

#endif /* ELEMENTSPROGRAM_H_ */
