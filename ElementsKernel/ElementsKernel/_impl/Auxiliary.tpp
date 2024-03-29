/**
 * @file ElementsKernel/_impl/Auxiliary.tpp
 * @brief implementation of the templates declared in ElementsKernel/Auxiliary.h
 * @date Feb 8, 2017
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
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_AUXILIARY_IMPL_
#error "This file should not be included directly! Use ElementsKernel/Auxiliary.h instead"
#else

#include "ElementsKernel/Exception.h"  // for Exception
#include "ElementsKernel/Path.h"       // for Path::VARIABLE, Path::Type, Path::Item
                                       // getPathFromLocations
namespace Elements {
inline namespace Kernel {

template <typename T>
Path::Item getAuxiliaryPath(const T& file_name, bool raise_exception) {

  auto location_list = getAuxiliaryLocations();

  auto result = Path::getPathFromLocations(file_name, location_list);

  if (result.empty() and raise_exception) {
    throw Exception() << "The auxiliary path \"" << file_name << "\" cannot be found!";
  }

  return result;
}

namespace Auxiliary {

template <typename T>
Path::Item getPath(const T& file_name, bool raise_exception) {
  return getAuxiliaryPath(file_name, raise_exception);
}

}  // namespace Auxiliary

}  // namespace Kernel
}  // namespace Elements

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_AUXILIARY_IMPL_
