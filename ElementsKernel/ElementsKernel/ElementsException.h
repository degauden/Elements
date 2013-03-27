// $Id: ElementsException.h,v 1.6 2007/12/20 18:28:33 marcocle Exp $
#ifndef ELEMENTSKERNEL_ELEMENTSEXCEPTION_H
#define ELEMENTSKERNEL_ELEMENTSEXCEPTION_H

// Include files
#include "ElementsKernel/Kernel.h"
#include "ElementsKernel/StatusCode.h"
#include "ElementsKernel/System.h"

#include <string>
#include <iostream>
#include <exception>

/**
 * @class ElementsException ElementsException.h ElementsKernel/ElementsException.h
 *
 * Define general base for Elements exception
 *
 * @author Vanya Belyaev
 * @author Sebastien Ponce
 */
class ELEMENTS_API ElementsException: virtual public std::exception {
  // friends
  friend inline std::ostream& operator<<(std::ostream& os,
      const ElementsException& ge);
  friend inline std::ostream& operator<<(std::ostream& os,
      const ElementsException* pge);
  friend class StatusCode;

public:
  /** Constructor (1)
   @param Message error message
   @param Tag "name tag", or exeption type
   @param Code status code
   */
  ElementsException(const std::string& Message, const std::string& Tag,
      const StatusCode & Code) :
      m_message(Message), m_tag(Tag), m_code(Code), m_previous(0) {
    s_proc = true;
  }

  /** Constructor (2)
   @param Message error message
   @param Tag "name tag", or exeption type
   @param Code status code
   @param Exception "previous"  exception
   */
  ElementsException(const std::string& Message, const std::string& Tag,
      const StatusCode & Code, const ElementsException& Exception) :
      m_message(Message), m_tag(Tag), m_code(Code), m_previous(
          Exception.clone()) {
  }

  /** Constructor (3)
   @param Message error message
   @param Tag "name tag", or exeption type
   @param Code status code
   @param Exception "previous" exception (used to improve the error message)
   */
  ElementsException(const std::string& Message, const std::string& Tag,
      const StatusCode & Code, const std::exception& Exception) :
      m_message(Message), m_tag(Tag), m_code(Code), m_previous(0) {
    s_proc = true;
    m_message += ": " + Elements::System::typeinfoName(typeid(Exception)) + ", "
        + Exception.what();
  }

  /// Copy constructor (deep copying!)
  ElementsException(const ElementsException& Exception) :
      std::exception(Exception) {
    s_proc = true;
    m_message = Exception.message();
    m_tag = Exception.tag();
    m_code = Exception.code();
    m_previous =
        (0 == Exception.previous()) ? 0 : Exception.previous()->clone();
  }

  /// destructor (perform the deletion of "previous" field!)
  virtual ~ElementsException() throw () {
    m_code.setChecked();
    if (0 != m_previous) {
      delete m_previous;
      m_previous = 0;
    }
    s_proc = false;
  }

  /// assignment operator
  ElementsException& operator=(const ElementsException& Exception) {
    if (&Exception == this) {
      return *this;
    }
    m_message = Exception.message();
    m_tag = Exception.tag();
    m_code = Exception.code();
    if (0 != m_previous) {
      delete m_previous;
      m_previous = 0;
    }
    m_previous =
        (0 == Exception.previous()) ? 0 : Exception.previous()->clone();
    return *this;
  }

  ///  error message to be printed
  virtual const std::string& message() const {
    return m_message;
  }

  /// update the error message to be printed
  virtual const std::string& setMessage(const std::string& newMessage) {
    m_message = newMessage;
    return message();
  }

  ///  name tag for the exception, or exception type
  virtual const std::string& tag() const {
    return m_tag;
  }

  /// update name tag
  virtual const std::string& setTag(const std::string& newTag) {
    m_tag = newTag;
    return tag();
  }

  /// StatusCode for Exception
  virtual const StatusCode& code() const {
    return m_code;
  }

  ///  update the status code for the exception
  virtual const StatusCode& setCode(const StatusCode& newStatus) {
    m_code = newStatus;
    return code();
  }

  /// get the previous exception ( "previous" element in the linked list)
  virtual ElementsException* previous() const {
    return m_previous;
  }

  /// methods  for overloaded printout to std::ostream& and MsgStream&
  virtual std::ostream& printOut(std::ostream& os = std::cerr) const {
    os << tag() << " \t " << message();
    switch (code()) {
    case StatusCode::SUCCESS:
      os << "\t StatusCode=SUCCESS";
      break;
    case StatusCode::FAILURE:
      os << "\t StatusCode=FAILURE";
      break;
    default:
      os << "\t StatusCode=" << code();
      break;
    }
    return (0 != previous()) ? previous()->printOut(os << std::endl) : os;
  }

  /// clone operation
  virtual ElementsException* clone() const {
    return new ElementsException(*this);
  }

  /// method from std::exception
  virtual const char* what() const throw () {
    return message().c_str();
  }
protected:
  mutable std::string m_message;  /// error message
  mutable std::string m_tag;  /// exception tag
  mutable StatusCode m_code;  /// status code for exception
  mutable ElementsException* m_previous; /// "previous" element in the linked list
  static bool s_proc;
};

/// overloaded printout to std::ostream
std::ostream& operator<<(std::ostream& os, const ElementsException& ge) {
  return ge.printOut(os);
}
std::ostream& operator<<(std::ostream& os, const ElementsException* pge) {
  return
      (0 == pge) ? (os << " ElementsException* points to NULL!") : (os << *pge);
}

#endif  // ELEMENTSKERNEL_ELEMENTSEXCEPTION_H
