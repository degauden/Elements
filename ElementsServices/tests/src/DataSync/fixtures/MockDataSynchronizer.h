/**
 * @file MockDataSynchronizer.cpp
 *
 * @date Jan 16, 2019
 * @author Antoine Basset
 *
 * @copyright 2019
 *
 */

#ifndef ELEMENTSSERVICES_TESTS_SRC_DATASYNC_FIXTURES_MOCKDATASYNCHRONIZER_H_
#define ELEMENTSSERVICES_TESTS_SRC_DATASYNC_FIXTURES_MOCKDATASYNCHRONIZER_H_

#include <string>
#include <map>

#include "ElementsServices/DataSync/DataSynchronizer.h"

#include "ConfigFilesFixture.h"

namespace ElementsServices {
namespace DataSync {

class MockDataSynchronizer : public DataSynchronizer {

public:

  virtual ~MockDataSynchronizer() = default;

  MockDataSynchronizer(
      path connection = theWebdavFrConfig(),
      path dependency = theDependencyConfig());

  std::string createDownloadCommand(
      path distantFile,
      path localFile) const override;

  std::map<path, path> fileMap();

};

}  // namespace DataSync
}  // namespace ElementsServices

#endif  // ELEMENTSSERVICES_TESTS_SRC_DATASYNC_FIXTURES_MOCKDATASYNCHRONIZER_H_

