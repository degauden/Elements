/**
 * @file OpenMPExample.cpp
 * @date March 17th, 2020
 * @author Hubert Degaudenzi
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

#include <complex>  // for complex
#include <cstdio>   // for size_t
#include <map>      // for map
#include <string>   // for string

#include <boost/current_function.hpp>  // for BOOST_CURRENT_FUNCTION

#include "ElementsKernel/ProgramHeaders.h"  // for including all Program/related headers

using std::map;
using std::size_t;
using std::string;
using complex = std::complex<double>;

namespace Elements {
namespace Examples {

static constexpr char CHARSET[] = ".,c8M@jawrpogOQEPGJ";

class OpenMP : public Program {

public:
  ExitCode mainMethod(map<string, VariableValue>& /*args*/) override {

    auto log = Logging::getLogger("ProgramExample");

    const int width      = 78;
    const int height     = 44;
    const int num_pixels = width * height;

    const complex center(-.7, 0);
    const complex span(2.7, -(4 / 3.0) * 2.7 * height / width);
    const complex begin   = center - span / 2.0;  //, end = center+span/2.0;
    const int     maxiter = 100000;

#pragma omp parallel for ordered schedule(dynamic)
    for (int pix = 0; pix < num_pixels; ++pix) {

      const int x = pix % width;
      const int y = pix / width;

      complex c = begin + complex(x * span.real() / (width + 1.0), y * span.imag() / (height + 1.0));

      size_t n = mandelbrotCalculate(c, maxiter);
      if (n == maxiter) {
        n = 0;
      }

#pragma omp ordered
      {
        char c2 = ' ';
        if (n > 0) {
          c2 = CHARSET[n % (sizeof(CHARSET) - 1)];
        }
        std::putchar(c2);
        if (x + 1 == width) {
          std::puts("|");
        }
      }
    }

    log.info() << "done with test program! ";

    return ExitCode::OK;
  }

private:
  static size_t mandelbrotCalculate(const complex c, const size_t maxiter) {
    // iterates z = z + c until |z| >= 2 or maxiter is reached,
    // returns the number of iterations.
    complex z = c;
    size_t  n = 0;
    for (; n < maxiter; ++n) {
      if (std::abs(z) >= 2.0) {
        break;
      }
      z = z * z + c;
    }
    return n;
  }
};

}  // namespace Examples
}  // namespace Elements

/**
 * Implementation of a main using a base class macro
 * This must be present in all Elements programs
 */
MAIN_FOR(Elements::Examples::OpenMP)
