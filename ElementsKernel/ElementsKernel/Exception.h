/**
 * @file Exception.h
 * @brief defines the base Elements exception class
 * @date Feb 20, 2013
 * @author Pavel Binko - The Euclid Consortium
 */

#ifndef ELEMENTSEXCEPTION_H_
#define ELEMENTSEXCEPTION_H_

#include <string>
#include <sstream>
#include <cstdio>
#include <utility>
#include <exception>

class ElementsException: public std::exception {
public:
  /**
   * Default constructor. The message is set  to the empty string.
   */
  ElementsException() = default;

  /** Constructor (C strings).
   *  @param message C-style string error message.
   *                 The string contents are copied upon construction.
   *                 Hence, responsibility for deleting the char* lies
   *                 with the caller.
   */
  explicit ElementsException(const char* message) :
      m_error_msg(message) {
  }

  /** Constructor (C++ STL strings).
   *  @param message The error message.
   */
  explicit ElementsException(const std::string& message) :
      m_error_msg(message) {
  }

  /**
   * @brief Constructs a new ElementsException with a message using format specifiers
   *
   * @param stringFormat The message containing the format specifiers
   * @param args The values to replace the format specifiers with
   */
  template <typename ...Args>
  explicit ElementsException(const char* stringFormat, Args &&...args) {
    size_t len = snprintf(NULL, 0, stringFormat, std::forward<Args>(args)...)+1;
    char* message = new char[len];
    snprintf(message, len, stringFormat, std::forward<Args>(args)...);
    m_error_msg = std::string {message};
    delete [] message;
  }

  /** Virtual destructor.
   */
  virtual ~ElementsException() noexcept {
  }

  /** Returns a pointer to the (constant) error description.
   *  @return A pointer to a const char *. The underlying memory
   *          is in possession of the Exception object. Callers must
   *          not attempt to free the memory.
   */
  const char * what() const noexcept override {
    return m_error_msg.c_str();
  }

  /**
   * @brief Appends in the end of the exception message the parameter
   * @details
   * The passed parameters can be of any type the &lt;&lt; operator of the
   * std::stringstream can handle.
   * @param message The message to append
   * @return A reference to the Exception with the appended message
   */
  template <typename T>
  ElementsException& operator<<(const T& message) {
    std::stringstream new_message;
    new_message << m_error_msg << message;
    m_error_msg = new_message.str();
    return *this;
  }

protected:
  /** Error message.
   */
  std::string m_error_msg {};
};

#endif /* ELEMENTSEXCEPTION_H_ */
