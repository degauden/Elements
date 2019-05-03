/**
 * @file ConfigFilesFixture.cpp
 *
 * @date Jan 16, 2019
 * @author Antoine Basset
 *
 * @copyright 2019
 *
 */

#include "ConfigFilesFixture.h"

#include <vector>

#include "ElementsKernel/Temporary.h"
#include "ElementsServices/DataSync/DataSyncUtils.h"


using ElementsServices::DataSync::path;
using ElementsServices::DataSync::localWorkspacePrefix;


WorkspaceFixture::WorkspaceFixture() {
  m_env["WORKSPACE"] = (m_top_dir.path() / "workspace").string();
}

path theDependencyConfig() {
  return path("ElementsServices/testdata/test_file_list.txt");
}

std::vector<path> theLocalFiles() {
  return std::vector<path>( {
      path("file1.txt"),
      path("file2.txt"),
      path("dir/file3.txt"),
      path("dir/file4.txt"),
      path("file5.txt")
  });
}

path theLocalWorkspace() {
  return path("/tmp");
}

path thePrefixedLocalWorkspace() {
  return localWorkspacePrefix() / theLocalWorkspace();
}

path absPath(path relPath) {
  return thePrefixedLocalWorkspace() / relPath;
}

path theInvalidDependencyConfig() {
  return path("ElementsServices/testdata/invalid_test_file_list.txt");
}

path theIrodsFrConfig() {
  return path("ElementsServices/testdata/sync_fr_irods.conf");
}

path theWebdavFrConfig() {
  return path("ElementsServices/testdata/sync_fr_webdav.conf");
}

path theWebdavFrDistantWorkspace() {
  return path("/euclid-fr/ct/mock_test_data");
}

path theWebdavEsConfig() {
  return path("ElementsServices/testdata/sync_es_webdav.conf");
}

path theNoOverwriteConfig() {
  return path("ElementsServices/testdata/sync_fr_webdav_nooverwrite.conf");
}

path aBadConnectionConfig() {
  return path("ElementsServices/testdata/sync_wrong.conf");
}
