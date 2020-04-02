/**
 * @file pybind11/callback.cpp
 *
 * @date Apr 1, 2020
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

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include <pyerrors.h>

#include "ElementsKernel/Export.h"
#include "ElementsExamples/callBackExample.h"

ELEMENTS_API PYBIND11_MODULE(testCallBack, mod) {
    mod.def("test", &Elements::Examples::testCallBack);
}
