#define ELEMENTSKERNEL_STATUSCODE_CPP 1

#include "ElementsKernel/StatusCode.h"
#include "ElementsKernel/System.h"
#include "ElementsKernel/ElementsException.h"
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

    if (!m_checked && !ElementsException::s_proc && !uncaught_exception() ) {

      const size_t depth = 21;
      void* addresses[depth];

      string lib, fnc;

      // FIXME : (MCl) use backTrace(string&, const int, const int) instead
      if (Elements::System::backTrace(addresses, depth)) {
        void* addr = nullptr;
        if (Elements::System::getStackLevel(addresses[2], addr, fnc, lib)) {
        	cout << "Unchecked in " << fnc << " " << lib << endl;
        }
      }

    }
  }
}

