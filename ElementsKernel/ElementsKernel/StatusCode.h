// $Id: StatusCode.h,v 1.12 2008/10/28 17:21:58 marcocle Exp $
#ifndef ELEMENTSKERNEL_STATUSCODE_H
#define ELEMENTSKERNEL_STATUSCODE_H

#include <ostream>
#include <cstdint>

#include "ElementsKernel/Export.h" // ELEMENTS_API
#include "boost/shared_ptr.hpp"

/**
 * @class StatusCode StatusCode.h ElementsKernel/StatusCode.h
 *
 * This class is used for returning status codes from appropriate routines.
 *
 * @author Iain Last
 * @author Pere Mato
 * @author Sebastien Ponce
 */

class IMessageSvc;
class IStatusCodeSvc;

class IgnoreError {
};

class StatusCode {
public:
  enum {
    FAILURE = 0, SUCCESS = 1, RECOVERABLE = 2
  };

  /// Constructor.
  StatusCode() :
      d_code(SUCCESS), m_checked(false) {
  }

  StatusCode(uint32_t code, bool checked = false) :
      d_code(code), m_checked(checked) {
  }

  StatusCode(const StatusCode& rhs) :
      d_code(rhs.d_code), m_checked(rhs.m_checked) {
    rhs.m_checked = true;
  }

  /// Destructor.
  ELEMENTS_API ~StatusCode();

  /** Test for a status code of SUCCESS.
   * N.B. This is the only case where a function has succeeded.
   */
  bool isSuccess() const {
    m_checked = true;
    return (d_code == SUCCESS);
  }

  /** Test for a status code of FAILURE.
   * N.B. This is a specific type of failure where there aren't any more
   * appropriate status codes. To test for any failure use :
   * if ( !StatusCode.isSuccess() ) ...
   */
  bool isFailure() const {
    return !isSuccess();
  }
  bool isRecoverable() const {
    m_checked = true;
    return (d_code == RECOVERABLE);
  }

  /// Get the status code by value.
  uint32_t getCode() const {
    m_checked = true;
    return d_code;
  }

  /// Set the status code by value.
  void setCode(uint32_t value) {
    m_checked = false;
    d_code = value;
  }

  /// Ignore the checking code;
  void setChecked() const {
    m_checked = true;
  }
  void ignore() const {
    setChecked();
  }

  /// Cast operator.
  operator uint32_t() const {
    return getCode();
  }

  /// Assignment operator.
  StatusCode& operator=(uint32_t value) {
    setCode(value);
    return *this;
  }
  StatusCode& operator=(const StatusCode& rhs) {
    if (this == &rhs)
      return *this; // Protection against self-assignment
    d_code = rhs.d_code;
    m_checked = rhs.m_checked;
    rhs.m_checked = true;
    return *this;
  }

  /// Comparison operator.
  friend bool operator<(const StatusCode& a, const StatusCode& b) {
    return a.d_code < b.d_code;
  }

  /// Comparison operator.
  friend bool operator>(const StatusCode& a, const StatusCode& b) {
    return a.d_code > b.d_code;
  }

  operator IgnoreError() const {
    m_checked = true;
    return IgnoreError();
  }

  static ELEMENTS_API void enableChecking();
  static ELEMENTS_API void disableChecking();

protected:
  /// The status code.
  uint32_t d_code;      ///< The status code
  mutable bool m_checked;   ///< If the Status code has been checked

  static bool s_checking; ///< Global flag to control if StatusCode need to be checked
};

inline std::ostream& operator<<(std::ostream& s, const StatusCode& sc) {
  if (sc.isSuccess()) {
    return s << "SUCCESS";
  } else if (sc.isRecoverable()) {
    return s << "RECOVERABLE";
  }
  s << "FAILURE";
  if (StatusCode::FAILURE != sc.getCode()) {
    s << "(" << sc.getCode() << ")";
  }
  return s;
}

#endif  // ELEMENTSKERNEL_STATUSCODES_H

