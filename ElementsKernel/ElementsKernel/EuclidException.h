/*
 * EuclidException.h
 *
 *  Created on: Feb 20, 2013
 *      Author: Pavel Binko
 */

#ifndef EUCLIDEXCEPTION_H_
#define EUCLIDEXCEPTION_H_

#include <string>
#include <exception>

class EuclidException: public std::exception {
public:
  /** Constructor (C strings).
   *  @param message C-style string error message.
   *                 The string contents are copied upon construction.
   *                 Hence, responsibility for deleting the char* lies
   *                 with the caller.
   */
  explicit EuclidException(const char* message) :
      m_errorMsg(message) {
  }

  /** Constructor (C++ STL strings).
   *  @param message The error message.
   */
  explicit EuclidException(const std::string& message) :
      m_errorMsg(message) {
  }

  /** Virtual destructor.
   */
  virtual ~EuclidException() noexcept {
  }

  /** Returns a pointer to the (constant) error description.
   *  @return A pointer to a const char *. The underlying memory
   *          is in possession of the Exception object. Callers must
   *          not attempt to free the memory.
   */
  virtual const char * what() const noexcept {
    return m_errorMsg.c_str();
  }

protected:
  /** Error message.
   */
  std::string m_errorMsg;
};

#endif /* EUCLIDEXCEPTION_H_ */
