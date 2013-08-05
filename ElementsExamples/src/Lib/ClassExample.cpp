/*
 * ClassExample.cpp
 *
 *  Created on: Jul 31, 2013
 *      Author: dubath
 */

#include "ElementsExamples/ClassExample.h"
#include "ElementsKernel/ElementsLogging.h"
#include "ElementsKernel/EuclidException.h"

ClassExample::ClassExample() {
	// TODO Auto-generated constructor stub

}

ClassExample::~ClassExample() {
	// TODO Auto-generated destructor stub
}

std::string ClassExample::s_staticString = "This is a static field example";

void ClassExample::doSomething(double first, double second) {

	ElementsLogging& logger = ElementsLogging::getLogger();
	logger.info("#");
	logger.info("#  Logging from the doSomething() class example: ");
	logger.info("#     examples of all logging levels (check with log-level option  ");
	logger.info("#");
	logger.fatal("A Fatal log message, appears with log-level > 100");
	logger.error("An Error log message, appears with log-level > 200");
	logger.warn("A Warning log message, appears with log-level > 300");
	logger.info("An Info log message, appears with log-level > 400");
	logger.debug("A Debug log message, appears with log-level > 500");
	logger.info("#");
	logger.info("#");


	logger.info(" Compute the sum of two numbers");
	m_result = this->computeSum(first, second);

	try {
		logger.info(" Divide the two numbers.This throws an exception if the second one is zero");
		m_result = this->divideNumbers(first, second);
	} catch (EuclidException& e) {
		logger.info(" The exception is caught and it continues");
		logger.debug("	A Elements exception is caught. The message is");
		logger.debug(" 		-- %s", e.what());
		logger.debug("	Exception should be caught only when knowing what to do to recover. ");
		logger.debug("	Here, it continues as this is a silly example. ");
	}

}

double ClassExample::computeSum(double first, double second) {
	return first+second;
}

double ClassExample::divideNumbers(double first, double second) {
	double tolerance = 1e-15;
	if (std::abs(second) < tolerance ) {
		std::stringstream errorBuffer;
		errorBuffer << "Exception: attempt to divide by: " << second << " a number to close to zero!";
		throw EuclidException(errorBuffer.str());
	}
	return first/second;
}
