/*
 * TemplatedDataSourceUser.h
 *
 *  Created on: Sep 23, 2015
 *      Author: fdubath
 */

#ifndef EXAMPLEMODULE_EXAMPLEMODULE_TEMPLATEDDATASOURCEUSER_H_
#define EXAMPLEMODULE_EXAMPLEMODULE_TEMPLATEDDATASOURCEUSER_H_



namespace Elements{
namespace Examples {



/**
 * @class TemplatedDataSourceUser
 * @brief This class has been created to demonstrate unit testing. It manipulate
 * an object representing a DataSource using template to achieve polymorphism.
 */
class TemplatedDataSourceUser{
public:

  /**
     * @brief Compute the sum of the values of the records stored into the provided
     * DataSource
     *
     * @tparam T
     * A type representing a DataSource. The type must declare the methods
     * + size_t countRecords() const
     * + double getRecordValue(size_t index) const
     *
     * @param data_source
     * A reference to a DataSource object.
     *
     * @return The sum of the values of the records into the DataSource object
     */
  template <typename T>
  double sumRecords(const T& data_source);
};

} // namespace ElementsExamples
} // namespace Elements



#include "ElementsExamples/_impl/TemplatedDataSourceUser.icpp"

#endif /* EXAMPLEMODULE_EXAMPLEMODULE_TEMPLATEDDATASOURCEUSER_H_ */
