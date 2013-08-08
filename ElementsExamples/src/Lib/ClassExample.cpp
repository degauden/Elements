/**
 * @file ClassExample.cpp
 *
 * Created on: Aug 8, 2013Jun 20, 2013
 *     Author: Pierre Dubath
 */

#include "ElementsExamples/ClassExample.h"
#include "ElementsKernel/ElementsLogging.h"
#include "ElementsKernel/ElementsException.h"


// Default desctructor
ClassExample::~ClassExample() {
  // destructor code if any
}

std::string ClassExample::s_staticString = "This is a static field example";

void ClassExample::summingAndDividing(double first, double second) {

  ElementsLogging& logger = ElementsLogging::getLogger();

  logger.info("#");
  logger.info("#  Logging from the doSomething() class example: ");
  logger.info(
      "#     examples of all logging levels (check with log-level option  ");
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
    logger.info(
        " Divide the two numbers.This throws an exception if second is zero");
    m_result = this->divideNumbers(first, second);
  } catch (ElementsException& e) {
    logger.info("#");
    logger.info("  In ClassExample::summingAndDividing(...)");
    logger.info("      an exception is caught from divideNumbers");
    logger.info("      the exception is trown again!");
    logger.info("       (assuming we do not what to do to fix it!)");
    logger.info("#");
    throw ElementsException(e.what());
  }

}

double ClassExample::computeSum(double first, double second) const noexcept {
  return first + second;
}

double ClassExample::divideNumbers(double first, double second) const {
  // to avoid rounding issues when testing whether second is zero, a tolerence is defined
  double tolerance = 1e-15;
  // and a test is achieve
  if (std::abs(second) < tolerance) {
    // build a stringstream error message
    std::stringstream errorBuffer;
    errorBuffer << "Dividing by: " << second
        << " exception thrown in divideNumber!";
    throw ElementsException(errorBuffer.str());
  }
  return first / second;
}
