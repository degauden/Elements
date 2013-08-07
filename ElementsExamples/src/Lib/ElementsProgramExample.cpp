/**
 * @file ElementsProgramExample.cpp
 * 
 * Created on: Jul 18, 2013
 * 
 * @author Pierre Dubath
 *
 */

#define VERSION "Version svn: $Id$ --- $Revision$"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "ElementsKernel/ElementsException.h"
#include "ELEMENTS_VERSION.h"

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "ElementsKernel/ElementsLogging.h"
#include "ElementsExamples/ElementsProgramExample.h"
#include "ElementsExamples/ClassExample.h"

using namespace std;


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
	// A example string option
	("string-value", po::value<string>()->default_value(string { }), "A string option")
	// A example long int option
	("long-long-value", po::value<int64_t>()->default_value(int64_t { }), "A long long option")
	// A double option
	("double-value", po::value<double>()->default_value(double { }), "A double option")
	// A string vector option
	("string-vector",po::value<vector<string>>()->multitoken()->default_value(vector<string> { }, "Empty"), "A string vector")
	// A int vector option
	("int-vector",po::value<vector<int>>()->multitoken()->default_value(vector<int> { }, "Empty"), "An int vector")
	// A double vector option
	("double-vector",po::value<vector<double>>()->multitoken()->default_value(vector<double> { }, "Empty"), "A double vector");

	return configFileOptions;
}


void ElementsProgramExample::pseudoMain() {

	try {

		ElementsLogging& logger = ElementsLogging::getLogger();
		logger.info("#");
		logger.info("#  Logging from the pseudo_main() of the ElementsProgramExample ");
		logger.info("#");

		// The map with all variables (of different types)
		const po::variables_map variableMap = this->getVariablesMap();

		// Retrieve the values from the po::variables_map
		string stringValue = variableMap["string-value"].as<string>();
		int64_t longLongValue = variableMap["long-long-value"].as<int64_t>();
		double doubleValue = variableMap["double-value"].as<double>();
		vector<int> intVector = variableMap["int-vector"].as<vector<int>>();
		vector<string> stringVector = variableMap["string-vector"].as<
				vector<string>>();

		//-------------------------------------------------------------------------
		// Do something here
		ClassExample ce {};

		logger.info("#");
		logger.info("#  Call the doSomething of the ClassExample ");
		logger.info("#");
		double aDouble = static_cast<double>(longLongValue);
		ce.doSomething(aDouble, doubleValue);

		double result = ce.getResult();


		//-------------------------------------------------------------------------
		// Clean up
		logger.info("#");
		logger.info("#  The result of doSomething is %e ", result);
		logger.info("#");

		/**
		 * Here we should call a method to persist the result of the calculation
		 */
		/// persist(result);

	} catch (const boost::exception_detail::error_info_injector<
			boost::program_options::unknown_option> & e) {
		cout << "boost::exception_detail : " << e.what() << endl;
	} catch (const boost::bad_any_cast & e) {
		cout << "boost::bad_any_cast : " << e.what() << endl;
	} catch (const ElementsException & e) {
		cout << "Euclid exception caught : " << e.what() << endl;
	} catch (const std::exception & e) {
		cout << "std::exception caught : " << e.what() << endl;
	}

}

//int main(int argc, const char* argv[]) {
//
//  ElementsProgramExample epe{};
//
//  epe.run(argc, argv);
//
//  //return epe.pseudo_main();
//
//  return 0;
//
//} // Eof main()

BUILD_MAIN_FOR(ElementsProgramExample)
