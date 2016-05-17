/**
 * @file Path.cpp
 *
 * @date May 13, 2016
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
 *
 */

#include "ElementsKernel/Path.h"

#include <string>                      // for string
#include <boost/filesystem.hpp>        // for boost::filesystem

using namespace std;
namespace fs = boost::filesystem;


namespace Elements {


// Template instanciation for the most common types
template fs::path getFileFromPath<fs::path>(const fs::path& file_name, const string& path_variable);
template fs::path getFileFromPath<string>(const string& file_name, const string& path_variable);


} // Elements namespace
