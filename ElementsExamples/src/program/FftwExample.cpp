/**
 * @file FftwExample.cpp
 * @date January 6th, 2015
 * @author Pierre Dubath
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

#include <cstdio>
#include <cmath>                            // for cos

#include <boost/program_options.hpp>        // for program options from configuration file of command line arguments
#include <boost/format.hpp>                 // for format

#include <fftw3.h>

#include "ElementsKernel/ProgramHeaders.h"  // for including all Program/related headers
#include "ElementsKernel/Unused.h"          // for ELEMENTS_UNUSED
#include "ElementsKernel/MathConstants.h"   // for pi

namespace Elements {
namespace Examples {

namespace po = boost::program_options;

using namespace std;

constexpr size_t N = 32;

class FftwExample: public Elements::Program {

public:


  Elements::ExitCode mainMethod(ELEMENTS_UNUSED map<string, po::variable_value>& args) override {

    Elements::Logging logger = Elements::Logging::getLogger("FftwExample");

    fftw_complex in[N], out[N], in2[N]; /* double [2] */
    fftw_plan p, q;

    /* prepare a cosine wave */
    for (size_t i = 0; i < N; i++) {
      in[i][0] = cos(3 * 2*Elements::Units::pi*i/N);
      in[i][1] = 0;
    }

    /* forward Fourier transform, save the result in 'out' */
    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);
    for (size_t i = 0; i<N; i++) {
      logger.info() << boost::format("freq: %3d %+9.5f %+9.5f I") % i % out[i][0] % out[i][1];
    }
    fftw_destroy_plan(p);

    /* backward Fourier transform, save the result in 'in2' */
    printf("\nInverse transform:\n");
    q = fftw_plan_dft_1d(N, out, in2, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(q);
    /* normalize */
    for (size_t i = 0; i<N; i++) {
      in2[i][0] *= 1./N;
      in2[i][1] *= 1./N;
    }
    for (size_t i = 0; i<N; i++) {
      logger.info() << boost::format("recover: %3d %+9.5f %+9.5f I vs. %+9.5f %+9.5f I") % i % in[i][0] % in[i][1] % in2[i][0] % in2[i][1];
    }
    fftw_destroy_plan(q);

    fftw_cleanup();

    logger.info() << "This is the end of the test";

    return Elements::ExitCode::OK;

  }

};

} // namespace ElementsExamples
} // namespace Elements


/**
 * Implementation of a main using a base class macro
 * This must be present in all Elements programs
 */
MAIN_FOR(Elements::Examples::FftwExample)

