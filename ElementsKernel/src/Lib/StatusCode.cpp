
#include "ElementsKernel/StatusCode.h"

#include <exception>                    // for uncaught_exception
#include <iostream>                     // for cout
#include <string>                       // for operator<<, string

#include "ElementsKernel/Likely.h"      // for UNLIKELY
#include "ElementsKernel/System.h"      // for backTrace, getStackLevel


namespace Elements {

bool StatusCode::s_checking(false);

void StatusCode::enableChecking() {
  s_checking = true;
}

void StatusCode::disableChecking() {
  s_checking = false;
}


StatusCode::~StatusCode() {

  if(UNLIKELY(s_checking)) {

	    //
	    // 	Change by Pierre Dubath (August 5th, 2013) in order to get rid of the class
	    //	ElementsException created by Marco Clemencic and to replace it by EuclidException that will be renamed
	    // 	ElementsException once the old ElementsException is deleted
	    //
	    //if (!m_checked && !ElementsException::s_proc && !uncaught_exception() ) {
	        if (!m_checked && !std::uncaught_exception() ) {

      const size_t depth = 21;
      void* addresses[depth];

      std::string lib, fnc;

      /// @todo : (MCl) use backTrace(string&, const int, const int) instead
      if (Elements::System::backTrace(addresses, depth)) {
        void* addr = nullptr;
        if (Elements::System::getStackLevel(addresses[2], addr, fnc, lib)) {
        	std::cout << "Unchecked in " << fnc << " " << lib << std::endl;
        }
      }

    }
  }
}

} // Elements namespace
