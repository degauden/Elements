/**
 * @file Temporary.h
 *
 * @date May 27, 2014
 * @author hubert degaudenzi
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

#ifndef ELEMENTSKERNEL_TEMPORARY_H_
#define ELEMENTSKERNEL_TEMPORARY_H_


#include <boost/filesystem.hpp>

#include "ElementsKernel/Export.h"                 // ELEMENTS_API
#include "ElementsKernel/Environment.h"            // for Environment

namespace Elements {

class TempPath {
public:
  explicit TempPath(const std::string& motif);
  virtual ~TempPath();
  ELEMENTS_API boost::filesystem::path path() const;
  ELEMENTS_API std::string motif() const;
private:
  const std::string m_motif;
  boost::filesystem::path m_path;
};


class TempDir : public TempPath {
public:
  ELEMENTS_API explicit TempDir(const std::string& motif="");
  ELEMENTS_API ~TempDir();
};


class TempFile : public TempPath {
public:
  ELEMENTS_API explicit TempFile(const std::string&  motif="");
  ELEMENTS_API ~TempFile();
};

using TempEnv = Environment;

} // namespace Elements

#endif // ELEMENTSKERNEL_TEMPORARY_H_
