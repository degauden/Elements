/**
 * @file ElementsKernel/_impl/Path.tpp
 * @brief implementation of the templates declared in ElementsKernel/Path.h
 * @date May 17, 2016
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

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_PATH_IMPL_
#error "This file should not be included directly! Use ElementsKernel/Path.h instead"
#else

#include <algorithm>      // for find_if, transform, for_each
#include <string>         // for string
#include <unordered_set>  // for unordered_set
#include <vector>         // for vector

#include <boost/algorithm/string/join.hpp>  // for join
#include <boost/filesystem/operations.hpp>  // for exists

namespace Elements {
inline namespace Kernel {
namespace Path {

template <typename T, typename U>
Item getPathFromLocations(const T& file_name, const std::vector<U>& locations) {

  Item found_path{};
  Item file_path{file_name};

  auto found_pos = std::find_if(locations.cbegin(), locations.cend(), [file_path](const U& l) {
    return boost::filesystem::exists(Item{l} / file_path);
  });

  if (found_pos != locations.cend()) {
    found_path = Item{*found_pos} / file_path;
  }

  return found_path;
}

template <typename T, typename U>
std::vector<Item> getAllPathFromLocations(const T& file_name, const std::vector<U>& locations) {

  std::vector<Item> file_list(locations.size());
  Item              file_path{file_name};

  std::transform(locations.cbegin(), locations.cend(), file_list.begin(), [file_path](const U& l) {
    return Item{l} / file_path;
  });

  auto found_pos = std::remove_if(file_list.begin(), file_list.end(), [](const Item& p) {
    return not boost::filesystem::exists(p);
  });

  file_list.erase(found_pos, file_list.end());

  return removeDuplicates(file_list);
}

template <typename T>
Item getPathFromEnvVariable(const T& file_name, const std::string& path_variable) {

  using std::vector;

  vector<Item> location_list = getLocationsFromEnv(path_variable);

  return getPathFromLocations(file_name, location_list);
}

template <typename T>
std::string joinPath(const std::vector<T>& path_list) {

  using std::string;
  using std::vector;

  vector<string> elems(path_list.size());

  std::transform(path_list.cbegin(), path_list.cend(), elems.begin(), [](const T& s) {
    return Item{s}.string();
  });

  std::string result = boost::algorithm::join(elems, PATH_SEP);

  return result;
}

template <typename... Args>
auto join(Args&&... args) -> decltype(joinPath(std::forward<Args>(args)...)) {
  return joinPath(std::forward<Args>(args)...);
}

template <typename... Args>
auto split(Args&&... args) -> decltype(splitPath(std::forward<Args>(args)...)) {
  return splitPath(std::forward<Args>(args)...);
}

template <typename T, typename U>
std::vector<Item> multiPathAppend(const std::vector<T>& initial_locations, const std::vector<U>& suffixes) {

  using std::vector;

  vector<Item> result(initial_locations.size() * suffixes.size());

  auto pos = result.begin();

  std::for_each(initial_locations.cbegin(), initial_locations.cend(), [&pos, &suffixes](const T& l) {
    std::transform(suffixes.cbegin(), suffixes.cend(), pos, [l](const U& s) {
      return Item{l} / s;
    });
    pos += static_cast<std::ptrdiff_t>(suffixes.size());
  });

  return result;
}

template <typename T>
std::vector<Item> removeDuplicates(const std::vector<T>& path_list) {

  std::unordered_set<std::string> s;

  std::vector<Item> output(path_list.size());

  auto end = copy_if(path_list.cbegin(), path_list.cend(), output.begin(), [&s](const T& i) {
    return s.insert(Item{i}.string()).second;
  });

  output.erase(end, output.end());

  return output;
}

}  // namespace Path
}  // namespace Kernel
}  // namespace Elements

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_PATH_IMPL_
