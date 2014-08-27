/**
 * @file SimpleProgram.h
 *
 * @date Aug 27, 2014
 * @author Hubert Degaudenzi
 */

#ifndef ELEMENTSKERNEL_SIMPLEPROGRAM_H_
#define ELEMENTSKERNEL_SIMPLEPROGRAM_H_

#include <boost/filesystem/path.hpp>

#include "ElementsKernel/Main.h"

namespace Elements {

// Forward declaration. We don't need the definition right now.
enum class ExitCode;

class SimpleProgram {

public:

  ExitCode run(int argc, char** argv) noexcept;
  const boost::filesystem::path& getProgramPath() const;
  const boost::filesystem::path& getProgramName() const;


protected:

  SimpleProgram() = default;
  virtual ~SimpleProgram() = default;

  virtual ExitCode main() = 0;
  virtual void defineOptions() = 0;

private :

  void setup(int argc, char** argv);

private:

  boost::filesystem::path m_program_name;
  boost::filesystem::path m_program_path;

};

} // namespace Elements


#endif // ELEMENTSKERNEL_SIMPLEPROGRAM_H_
