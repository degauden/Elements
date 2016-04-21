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

#include <string>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "ElementsKernel/Logging.h"
#include "ElementsKernel/Temporary.h"

namespace fs = boost::filesystem;

using std::string;

namespace Elements {

TempPath::TempPath(const string& motif) :
    m_motif(motif) {

  if (m_motif != "") {
    m_path = fs::temp_directory_path() / fs::unique_path(m_motif);
  } else {
    m_path = fs::temp_directory_path() / fs::unique_path();
  }

}

TempPath::~TempPath() {
}

fs::path TempPath::path() const {
  return m_path;
}

string TempPath::motif() const {
  return m_motif;
}

TempDir::TempDir(const string& motif) :
    TempPath(motif) {

  Logging logger = Logging::getLogger();
  logger.debug() << "Creation of the " << path() << " temporary directory";

  fs::create_directory(path());

}

TempDir::~TempDir() {

  Logging logger = Logging::getLogger();
  logger.debug() << "Automatic destruction of the " << path()
      << " temporary directory";

  fs::remove_all(path());

}

TempFile::TempFile(const string& motif) :
    TempPath(motif) {

  Logging logger = Logging::getLogger();
  logger.debug() << "Creation of the " << path() << " temporary file";

  fs::ofstream ofs(path());
  ofs.close();

}

TempFile::~TempFile() {

  Logging logger = Logging::getLogger();
  logger.debug() << "Automatic destruction of the " << path() << " file";

  fs::remove(path());

}

}  // namespace Elements
