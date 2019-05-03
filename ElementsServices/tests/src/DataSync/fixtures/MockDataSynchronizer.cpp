/**
 * @file MockDataSynchronizer.cpp
 *
 * @date Jan 16, 2019
 * @author Antoine Basset
 *
 * @copyright 2019
 *
 */

#include <string>
#include <map>

#include "ElementsServices/DataSync/DataSynchronizer.h"

#include "MockDataSynchronizer.h"

namespace DataSync = ElementsServices::DataSync;

using std::string;
using DataSync::path;

MockDataSynchronizer::MockDataSynchronizer(
      DataSync::path connection,
      path dependency) : DataSync::DataSynchronizer(
                         DataSync::ConnectionConfiguration(connection),
                         DataSync::DependencyConfiguration(theWebdavFrDistantWorkspace(),
                                                           theLocalWorkspace(),
                                                           dependency)
        ) {}

string MockDataSynchronizer::createDownloadCommand(
      path distantFile,
      path localFile) const {
  string cmd = string("echo ") + distantFile.string() + "\t" + localFile.string();
  return cmd;
}

std::map<path, path> MockDataSynchronizer::fileMap() {
  return m_fileMap;
}



