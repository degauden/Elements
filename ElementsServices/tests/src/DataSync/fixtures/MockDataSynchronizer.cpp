/**
 * @file MockDataSynchronizer.cpp
 *
 * @date Jan 16, 2019
 * @author Hubert Degaudenzi
 *
 * @copyright 2019
 *
 */

#include <string>
#include <map>

#include "ElementsServices/DataSync/DataSynchronizer.h"

#include "MockDataSynchronizer.h"

namespace ElementsServices {
namespace DataSync {


MockDataSynchronizer::MockDataSynchronizer(
      path connection,
      path dependency) :
        DataSynchronizer(
            ConnectionConfiguration(connection),
            DependencyConfiguration(theWebdavFrDistantWorkspace(), theLocalWorkspace(), dependency)
        ) {}

std::string MockDataSynchronizer::createDownloadCommand(
      path distantFile,
      path localFile) const {
    std::string cmd = std::string("echo ") + distantFile.string() + "\t" + localFile.string();
    return cmd;
  }

  std::map<path, path> MockDataSynchronizer::fileMap() {
    return m_fileMap;
  }


}  // namespace DataSync
}  // namespace ElementsServices

