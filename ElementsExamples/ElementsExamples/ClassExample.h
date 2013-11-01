/**
 * @file ClassExample.h
 *
 * Created on: Aug 8, 2013Jun 20, 2013
 *     Author: Pierre Dubath
 */

#ifndef CLASSEXAMPLE_H_
#define CLASSEXAMPLE_H_

#include<string>
#include "ElementsKernel/Kernel.h"


/**
 * @class ClassExample
 * @brief
 * 		A class example for the Element framework
 * @details
 * 		The naming convention and coding standard displayed in this
 * 		class can be used as a model for other classes
 */
class ClassExample {

public:

  /**
   * @brief Constructor
   */
  ELEMENTS_API ClassExample(int64_t source_id, double ra, double dec) :
      m_source_id(source_id), m_ra(ra), m_dec(dec) {
    m_result = 0.0;
  }

  /**
   * @brief Destructor
   */
  ELEMENTS_API virtual ~ClassExample() {};

  /**
   * @brief
   *    Compute the sum of two doubles
   * @details
   *    This is really a simple class. In general, the syntax
   *
   *    output = methodName(input)
   *
   *    is prefered. The C++11 keyword "noexcept" is used to indicate
   *    that this method do not throw (any exceptions). If this keyword
   *    is not present, the exception must be documented.
   *
   * @param first
   *    The first double value
   * @param second
   *    The second double value
   * @return
   *    The sum of the two values
   */
  ELEMENTS_API double computeSum(double first, double second) const noexcept;

  /**
   * @brief
   *    Divide two doubles
   * @details
   *    This is a simple class to illustrate the case of a method which can
   *    throw an exception
   *
   * @param first
   *    The first double value
   * @param second
   *    The second double value
   * @return
   *    The sum of the two values
   * @throws
   *   EuclidException, if the second number is (close to) zero
   */
  ELEMENTS_API double divideNumbers(double first, double second) const;

  /**
   * @brief
   *    Sum and divide two doubles
   * @details
   *    This one does not throw. The exception which can be thrown when
   *    attempting to divide by zero is catch and processed internally.
   *
   *    The result is store in the class member m_result. This makes this method
   *    a bit less readable that the previous ones as one can not immediately see
   *    what/where is the result
   *
   * @param first
   *    The first double value
   * @param second
   *    The second double value
   * @throws
   *   EuclidException, if the second number is (close to) zero
   */
  ELEMENTS_API void summingAndDividing(double first, double second);

  /*
   * Getter to access the static private string
   */
  ELEMENTS_API static const std::string& getStaticString() {
    return s_static_string;
  }

  /**
   * Getter to access private sourceId
   */
  int64_t getSourceId() const {
    return m_source_id;
  }

  /**
   * Getter to access private m_ra
   */
  double getRa() const {
    return m_ra;
  }

  /// Getter
  double getDec() const {
    return m_dec;
  }

  /// Getter
  double getResult() const {
    return m_result;
  }

private:

  /// An example of a static string
  ELEMENTS_API static std::string s_static_string;

  /// Source ID as an example of a long private member
  int64_t m_source_id { 0 };

  /// Source right ascension
  double m_ra { 0.0 };

  /// Source declination
  double m_dec { 0.0 };

  /// A double to store a result
  double m_result { 0.0 };

};

#endif /* CLASSEXAMPLE_H_ */
