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

#include <map>
#include <string>

#include "ElementsServices/DataSync/DataSynchronizer.h"

#include "ConfigFilesFixture.h"

struct MockDataSynchronizer : public ElementsServices::DataSync::DataSynchronizer, WorkspaceFixture {

  virtual ~MockDataSynchronizer() = default;

  MockDataSynchronizer(ElementsServices::DataSync::path connection = theWebdavFrConfig(),
                       ElementsServices::DataSync::path dependency = theDependencyConfig());

  std::string createDownloadCommand(ElementsServices::DataSync::path distantFile,
                                    ElementsServices::DataSync::path localFile) const override;

  std::map<ElementsServices::DataSync::path, ElementsServices::DataSync::path> fileMap();
};

#endif  // ELEMENTSSERVICES_TESTS_SRC_DATASYNC_FIXTURES_MOCKDATASYNCHRONIZER_H_
