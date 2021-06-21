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

/**
 * @addtogroup ElementsKernel ElementsKernel
 * @{
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_AUXILIARY_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_AUXILIARY_H_

#include <string>   // for string
#include <utility>  // for forward
#include <vector>   // for vector

#include "ElementsKernel/Export.h"  // ELEMENTS_API
#include "ElementsKernel/Path.h"    // for Path::Item

namespace Elements {
inline namespace Kernel {

/**
 * @brief retrieve the variable name used for the auxiliary file lookup
 * @ingroup ElementsKernel
 * @return
 *   the standard variable name: ELEMENTS_AUX_PATH
 */
ELEMENTS_API std::string getAuxiliaryVariableName();

template <typename T>
ELEMENTS_API Path::Item getAuxiliaryPath(const T& file_name, bool raise_exception = true);

// instantiation of the most expected types
extern template ELEMENTS_API Path::Item getAuxiliaryPath(const Path::Item& file_name, bool raise_exception);
extern template ELEMENTS_API Path::Item getAuxiliaryPath(const std::string& file_name, bool raise_exception);

ELEMENTS_API std::vector<Path::Item> getAuxiliaryLocations(bool exist_only = false);

namespace Auxiliary {

/**
 * @brief alias for the getAuxiliaryVariableName function
 * @ingroup ElementsKernel
 * @return same as getAuxiliaryVariableName
 */
auto& getVariableName = getAuxiliaryVariableName;

/**
 * @brief alias for the getAuxiliaryPath function
 * @ingroup ElementsKernel
 * @param args
 *   Forward arguments
 * @return same as getAuxiliaryPath
 */
template <typename T>
ELEMENTS_API Path::Item getPath(const T& file_name, bool raise_exception = true);

// instantiation of the most expected types
extern template ELEMENTS_API Path::Item getPath(const Path::Item& file_name, bool raise_exception);
extern template ELEMENTS_API Path::Item getPath(const std::string& file_name, bool raise_exception);

/**
 * @brief alias for the getAuxiliaryLocations function
 * @ingroup ElementsKernel
 * @return same as getAuxiliaryLocations
 */
auto& getLocations = getAuxiliaryLocations;

}  // namespace Auxiliary

/** @example ElementsExamples/src/program/CCfitsExample.cpp
 * This is an example of how to use the getAuxiliaryPath function.
 */

}  // namespace Kernel
}  // namespace Elements

#define ELEMENTSKERNEL_ELEMENTSKERNEL_AUXILIARY_IMPL_
#include "ElementsKernel/_impl/Auxiliary.icpp"
#undef ELEMENTSKERNEL_ELEMENTSKERNEL_AUXILIARY_IMPL_

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_AUXILIARY_H_

/**@}*/
