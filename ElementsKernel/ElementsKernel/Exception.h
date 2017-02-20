/**
 * @file Exception.h
 * @brief defines the base Elements exception class
 * @date Feb 20, 2013
 * @author Pavel Binko
 *
 * @copyright 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @addtogroup ElementsKernel ElementsKernel
 * @{
 */

#ifndef ELEMENTSEXCEPTION_H_
#define ELEMENTSEXCEPTION_H_

#include <string>
#include <sstream>
#include <cstdio>
#include <utility>
#include <exception>
#include <type_traits>

#include "ElementsKernel/Exit.h"

namespace Elements {

class Exception: public std::exception {
public:
  /**
   * Default constructor. The message is set  to the empty string.
   * @param e: this is an optional exit code. By default is is set
   *           to NOT_OK.
   */
  explicit Exception(ExitCode e=ExitCode::NOT_OK) :
    m_exit_code{e} {
  }

  /** Constructor (C strings).
   *  @param message C-style string error message.
   *                 The string contents are copied upon construction.
   *                 Hence, responsibility for deleting the char* lies
   *                 with the caller.
   *  @param e: this is an optional exit code. By default is is set
   *            to NOT_OK.
   */
  explicit Exception(const char* message, ExitCode e=ExitCode::NOT_OK) :
      m_error_msg(message),  m_exit_code{e} {
  }

  /** Constructor (C++ STL strings).
   *  @param message The error message.
   *  @param e: this is an optional exit code. By default is is set
   *            to NOT_OK.
   */
  explicit Exception(const std::string& message, ExitCode e=ExitCode::NOT_OK) :
      m_error_msg(message), m_exit_code{e} {
  }

  /**
   * @brief Constructs a new Exception with a message using format specifiers
   *
   * @param stringFormat The message containing the format specifiers
   * @param args The values to replace the format specifiers with
   */
  template <typename ...Args>
  explicit Exception(const char* stringFormat, Args &&...args)
              : m_exit_code{ExitCodeHelper<Args...>{args...}.code} {
    size_t len = snprintf(NULL, 0, stringFormat, std::forward<Args>(args)...)+1;
    char* message = new char[len];
    snprintf(message, len, stringFormat, std::forward<Args>(args)...);
    m_error_msg = std::string {message};
    delete [] message;
  }

  /** Virtual destructor.
   */
  virtual ~Exception() noexcept {
  }

  /** Returns a pointer to the (constant) error description.
   *  @return A pointer to a const char *. The underlying memory
   *          is in possession of the Exception object. Callers must
   *          not attempt to free the memory.
   */
  const char * what() const noexcept override {
    return m_error_msg.c_str();
  }

  /** Return the exit code of the Exception
   *
   * @return the exit code
   */
  ExitCode exitCode() const noexcept {
    return m_exit_code;
  }

  /**
   * @brief Appends in the end of the exception message the parameter
   * @details
   * The passed parameters can be of any type the &lt;&lt; operator of the
   * std::stringstream can handle.
   * @param message The message to append
   */
  template <typename T>
  void appendMessage(const T& message) {
    std::stringstream new_message;
    new_message << m_error_msg << message;
    m_error_msg = new_message.str();
  }
  
protected:
  /** Error message.
   */
  std::string m_error_msg {};
  const ExitCode m_exit_code {ExitCode::NOT_OK};

private:

  /// The following class keeps in its member variable 'code' the same ExitCode
  /// given as the last parameter of its constructor, or ExitCode::NOT_OK if the
  /// last argument of the constructor is not an ExitCode object.
  template<typename... Args>
  struct ExitCodeHelper{};

  // Specialization which handles the last argument
  template<typename Last>
  struct ExitCodeHelper<Last> {
    explicit ExitCodeHelper(const Last& last) : code{getCode(last)} {}
    ExitCode code;
  private:
    // This method is used if the T is an ExitCode object
    template<typename T, typename std::enable_if<std::is_same<T, ExitCode>::value>::type* = nullptr>
    ExitCode getCode(const T& t) {
      return t;
    }
    // This method is used when the T is not an ExitCode object
    template<typename T, typename std::enable_if<not std::is_same<T, ExitCode>::value>::type* = nullptr>
    ExitCode getCode(const T&) {
      return ExitCode::NOT_OK;
    }
  };

  // Specialization which handles two or more arguments
  template<typename First, typename... Rest>
  struct ExitCodeHelper<First, Rest...> : ExitCodeHelper<Rest...> {
    ExitCodeHelper(const First&, const Rest&... rest) : ExitCodeHelper<Rest...>(rest...) {}
  };

};

template <typename Ex, typename T,
           typename=typename std::enable_if<std::is_base_of<Exception,
           typename std::remove_reference<Ex>::type>::value>::type>
auto operator<<(Ex&& ex, const T& message) -> decltype(std::forward<Ex>(ex)) {
  ex.appendMessage(message);
  return std::forward<Ex>(ex);
}

} // namespace Elements

#endif /* ELEMENTSEXCEPTION_H_ */

/**@}*/
