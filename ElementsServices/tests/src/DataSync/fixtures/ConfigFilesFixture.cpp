/**
 * @file ConfigFilesFixture.cpp
 *
 * @date Jan 16, 2019
 * @author Antoine Basset
 *
 * @copyright 2019
 *
 */

#include <vector>

#include "ElementsServices/DataSync/DataSyncUtils.h"

#include "ConfigFilesFixture.h"

using ElementsServices::DataSync::path;
using ElementsServices::DataSync::localWorkspacePrefix;

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

path absPath(path relPath) {
  return localWorkspacePrefix() / theLocalWorkspace() / relPath;
}

path thePrefixedLocalWorkspace() {
  return localWorkspacePrefix() / theLocalWorkspace();
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
