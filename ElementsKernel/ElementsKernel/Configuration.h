/**
 * @file ElementsKernel/Configuration.h
 *
 * @brief provide functions to retrieve configuration files
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
 * \addtogroup ElementsKernel ElementsKernel
 * @{
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_CONFIGURATION_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_CONFIGURATION_H_

#include <string>                   // for string
#include <boost/filesystem.hpp>     // for boost::filesystem

#include "ElementsKernel/Export.h"  // ELEMENTS_API

namespace Elements {

ELEMENTS_API std::string getConfigurationVariableName();

template <typename T>
ELEMENTS_API boost::filesystem::path getConfigurationPath(const T& file_name);

}  // Elements namespace

#include "ElementsKernel/_impl/Configuration.icpp"


#endif // ELEMENTSKERNEL_ELEMENTSKERNEL_CONFIGURATION_H_ 
/**@}*/
