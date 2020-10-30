/**
 * @file MockDataSynchronizer.cpp
 *
 * @date Jan 16, 2019
 * @author Antoine Basset
 *
 * @copyright 2019
 *
 */

#include <map>
#include <string>

#include "ElementsServices/DataSync/DataSynchronizer.h"

#include "MockDataSynchronizer.h"

namespace DataSync = ElementsServices::DataSync;

using DataSync::path;
using std::string;

MockDataSynchronizer::MockDataSynchronizer(DataSync::path connection, path dependency)
    : DataSync::DataSynchronizer(
          DataSync::ConnectionConfiguration(connection),
          DataSync::DependencyConfiguration(theWebdavFrDistantWorkspace(), theLocalWorkspace(), dependency)) {}

string MockDataSynchronizer::createDownloadCommand(path distantFile, path localFile) const {
  string cmd = string("echo ") + distantFile.string() + "\t" + localFile.string();
  return cmd;
}

std::map<path, path> MockDataSynchronizer::fileMap() {
  return m_fileMap;
}
