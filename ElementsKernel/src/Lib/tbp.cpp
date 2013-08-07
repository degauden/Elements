#include <iostream>
#include <vector>

using namespace std ;


// --------------------------------------------------------------------------------
// Library code
// --------------------------------------------------------------------------------


class BaseProgram {
public:
  BaseProgram() = default ; 

  // This function has to be implemented in the derived class
  virtual void defineOptions() = 0 ; 

  // This function has to be implemented in the derived class
  virtual int main() = 0 ;
  
  // this function cannot be overriden: final is a new keyword from C++11
  virtual int run(int argc, char **argv) final {
    setup(argc, argv) ;
    return main() ;
  }

  virtual const vector<string>& cmdLine() final {
    return m_argv ; 
  }

  virtual ~BaseProgram() {} 

private:

  void defineBaseOptions() {
    // define some base stuff common to all programs
    // can't be called directly
  }

  virtual void setup(int argc, char **argv) final {
    m_argc = argc ; 
    m_argv = vector<string>(argv, argv+argc) ; 
    defineBaseOptions() ;
    defineOptions() ;
  }

  int m_argc ;
  vector<string> m_argv ; 

};

#define ELEMENTS_PROG(X) \
  int main(int argc, char** argv) \
  {                               \
                                  \
    X P ;                         \
                                  \
    return P.run(argc, argv) ;    \
                                  \
  }
  

// --------------------------------------------------------------------------------
// Client code
// --------------------------------------------------------------------------------


class TheProgram : public BaseProgram {
public:

  virtual void defineOptions() {
  }

  virtual int main() {
    for (auto i : cmdLine() ) {
      cout << i << endl ;
    }
    return 0 ;
  }

};


ELEMENTS_PROG(TheProgram)

