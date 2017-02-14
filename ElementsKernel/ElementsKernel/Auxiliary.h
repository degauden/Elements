/**
 * @file ElementsKernel/Auxiliary.h
 *
 * @brief provide functions to retrieve auxiliary files
 * @date Feb 8, 2017
 * @author Hubert Degaudenzi
 *
 * @copyright 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation; either version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this
 * library; if not, write to the
 *
 * Free Software Foundation, Inc.,
 * 51 Franklin Street,
 * Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 *
 */
#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_AUXILIARY_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_AUXILIARY_H_

#include <string>                   // for string
#include <boost/filesystem.hpp>     // for boost::filesystem

#include "ElementsKernel/Export.h"  // ELEMENTS_API

namespace Elements {

ELEMENTS_API std::string getAuxiliaryVariableName();

template <typename T>
ELEMENTS_API boost::filesystem::path getAuxiliaryPath(const T& file_name);

}  // Elements namespace

#include "ElementsKernel/_impl/Auxiliary.icpp"


#endif // ELEMENTSKERNEL_ELEMENTSKERNEL_AUXILIARY_H_
