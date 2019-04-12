/**
 * @file Temporary.h
 * @brief Handling of temporary files, directories and environments
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

/**
 * @addtogroup ElementsKernel ElementsKernel
 * @{
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_TEMPORARY_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_TEMPORARY_H_

#include <string>
#include <boost/filesystem.hpp>

#include "ElementsKernel/Export.h"                 // ELEMENTS_API
#include "ElementsKernel/Environment.h"            // for Environment

namespace Elements {

/// The default environment variable name to keep the temporary object
const std::string DEFAULT_TMP_KEEP_VAR {"KEEPTEMPDIR"};
/// The default random creation motif
const std::string DEFAULT_TMP_MOTIF {"%%%%-%%%%-%%%%-%%%%"};

class ELEMENTS_API TempPath {
public:
  explicit TempPath(const std::string& motif = DEFAULT_TMP_MOTIF,
                    const std::string& keep_var = DEFAULT_TMP_KEEP_VAR);
  virtual ~TempPath();
  boost::filesystem::path path() const;
  std::string motif() const;
private:
  const std::string m_motif;
  boost::filesystem::path m_path;
  const std::string m_keep_var;
};


class ELEMENTS_API TempDir : public TempPath {
public:
  explicit TempDir(const std::string& motif = DEFAULT_TMP_MOTIF,
                   const std::string& keep_var = DEFAULT_TMP_KEEP_VAR);
  virtual ~TempDir();
};


class ELEMENTS_API TempFile : public TempPath {
public:
  explicit TempFile(const std::string&  motif = DEFAULT_TMP_MOTIF,
                    const std::string& keep_var = DEFAULT_TMP_KEEP_VAR);
  virtual ~TempFile();
};

using TempEnv = Environment;

}  // namespace Elements

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_TEMPORARY_H_

/**@}*/
