/**
 * @file SimpleProgram.h
 *
 * @date Aug 27, 2014
 * @author Hubert Degaudenzi
 *
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
 *
 */

/**
 * @addtogroup ElementsKernel ElementsKernel
 * @{
 */

#ifndef ELEMENTSKERNEL_SIMPLEPROGRAM_H_
#define ELEMENTSKERNEL_SIMPLEPROGRAM_H_

#include <boost/filesystem/path.hpp>

#include "ElementsKernel/Export.h"
#include "ElementsKernel/Main.h"

namespace Elements {

// Forward declaration. We don't need the definition right now.
enum class ExitCode;

class ELEMENTS_API SimpleProgram {

public:

  ELEMENTS_API ExitCode run(int argc, char** argv) noexcept;
  ELEMENTS_API const boost::filesystem::path& getProgramPath() const;
  ELEMENTS_API const boost::filesystem::path& getProgramName() const;


protected:

  SimpleProgram() = default;
  virtual ~SimpleProgram();

  virtual ExitCode main() = 0;
  virtual void defineOptions() = 0;

private:

  void setup(int argc, char** argv);

private:

  boost::filesystem::path m_program_name;
  boost::filesystem::path m_program_path;

};

} // namespace Elements


#endif // ELEMENTSKERNEL_SIMPLEPROGRAM_H_

/**@}*/
