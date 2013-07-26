/**
 * @file ElementsProgramExample.cpp
 * 
 * Created on: Jul 18, 2013
 * 
 * @author dubath
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * This class represents ...
 */

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "ElementsKernel/EuclidException.h"
#include "ELEMENTS_VERSION.h"

#include <boost/program_options.hpp>
namespace po = boost::program_options;

using namespace std;

#include "ElementsExamples/ElementsProgramExample.h"

ElementsProgramExample::ElementsProgramExample() {
  // TODO Auto-generated constructor stub
}

ElementsProgramExample::~ElementsProgramExample() {
  // TODO Auto-generated destructor stub
}

po::options_description ElementsProgramExample::defineSpecificProgramOptions() {

   /**
    * Document all program options specific to this program
    */
   po::options_description configFileOptions("Configuration options");
   configFileOptions.add_options()
       ("string-value", po::value<string>()->default_value(string{}), "A string option" )
       ("long-long-value", po::value<int64_t>()->default_value(int64_t{}), "A long long option" )
       ("double-value", po::value<double>()->default_value(double{}), "A double option" )
       ("string-vector", po::value<vector<string>>()->multitoken()->default_value(vector<string>{},"Empty"), "A string vector" )
       ("int-vector", po::value<vector<int>>()->multitoken()->default_value(vector<int>{},"Empty"), "An int vector" );

   return configFileOptions;
}

string ElementsProgramExample::getVersion() {
  return "  1.2.0 is the version number TODO";
}

int ElementsProgramExample::pseudo_main() {

  try {
  cout << "##########################################################\n";
  cout << "#\n";
  cout << "#    Example main program : In the try block" << endl;
  cout << "#\n";
  cout << "##########################################################\n";


  // The map with all variables (of different types)
  const po::variables_map variableMap = this->getVariablesMap();

  // Retrieve the values from the po::variables_map
  string stringValue = variableMap["string-value"].as<string>();
  int64_t longLongValue = variableMap["long-long-value"].as<int64_t>();
  double doubleValue = variableMap["double-value"].as<double>();
  vector<int> intVector = variableMap["int-vector"].as<vector<int>>();
  vector<string> stringVector = variableMap["string-vector"].as<vector<string>>();

  cout << "\nProgram options printed in pseudo_main() \n";
  cout << "string-value        = " << stringValue << endl;
  cout << "double-value     = " << doubleValue << endl;
  cout << "long-long-value  = " << longLongValue << "\n\n";
  cout << "int-vector size    = " << intVector.size() << endl;
  cout << "string-vector size    = " << stringVector.size() << endl;

  cout << "----------------------------------------------------------\n\n";

  //-------------------------------------------------------------------------
  // Do something here

  //-------------------------------------------------------------------------
  // Clean up

} catch (const boost::exception_detail::error_info_injector<boost::program_options::unknown_option> & e) {
  cout << "boost::exception_detail : " << e.what() << endl;
} catch (const boost::bad_any_cast & e) {
  cout << "boost::bad_any_cast : " << e.what() << endl;
} catch (const EuclidException & e) {
  cout << "Euclid exception caught : " << e.what() << endl;
} catch (const std::exception & e) {
  cout << "std::exception caught : " << e.what() << endl;
}

return 0;
}
