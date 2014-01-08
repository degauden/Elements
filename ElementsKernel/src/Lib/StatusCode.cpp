#define ELEMENTSKERNEL_STATUSCODE_CPP 1

#include "ElementsKernel/StatusCode.h"
#include "ElementsKernel/System.h"
#include "ElementsKernel/Likely.h"
//#include "ElementsKernel/ElementsException.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <exception>

using namespace std;

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
	        if (!m_checked && !uncaught_exception() ) {

      const size_t depth = 21;
      void* addresses[depth];

      string lib, fnc;

      /// @toto : (MCl) use backTrace(string&, const int, const int) instead
      if (Elements::System::backTrace(addresses, depth)) {
        void* addr = nullptr;
        if (Elements::System::getStackLevel(addresses[2], addr, fnc, lib)) {
        	cout << "Unchecked in " << fnc << " " << lib << endl;
        }
      }

    }
  }
}

