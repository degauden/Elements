/**
 * @file Temporary.cpp
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
 *
 */

#include "ElementsKernel/Temporary.h"

#include <string>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "ElementsKernel/Logging.h"
#include "ElementsKernel/Environment.h"

using std::string;

namespace Elements {

namespace {
  auto log = Logging::getLogger();
}

TempPath::TempPath(const string& motif, const string& keep_var) :
    m_motif(motif), m_keep_var(keep_var) {

  using boost::filesystem::temp_directory_path;
  using boost::filesystem::unique_path;

  if (m_motif.find('%') == string::npos) {
    log.warn() << "The '" << m_motif << "' motif is not random";
  }

  if (m_motif != "") {
    m_path = temp_directory_path() / unique_path(m_motif);
  } else {
    m_path = temp_directory_path() / unique_path();
  }
}

TempPath::~TempPath() {

  Environment current;

  if (not current.hasKey(m_keep_var)) {
    log.debug() << "Automatic destruction of the " << path()
                   << " temporary path";
    boost::filesystem::remove_all(m_path);
  } else {
    log.info() << m_keep_var << " set: I do not remove the "
                  << m_path.string() << " temporary path";
  }

}

boost::filesystem::path TempPath::path() const {
  return m_path;
}

string TempPath::motif() const {
  return m_motif;
}

TempDir::TempDir(const string& motif, const string& keep_var) :
    TempPath(motif, keep_var) {

  log.debug() << "Creation of the " << path() << " temporary directory";

  boost::filesystem::create_directory(path());

}

TempDir::~TempDir() {
}

TempFile::TempFile(const string& motif, const string& keep_var) :
    TempPath(motif, keep_var) {

  log.debug() << "Creation of the " << path() << " temporary file";

  boost::filesystem::ofstream ofs(path());
  ofs.close();

}

TempFile::~TempFile() {
}

}  // namespace Elements
