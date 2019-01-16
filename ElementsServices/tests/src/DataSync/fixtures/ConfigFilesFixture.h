/**
 * @file ConfigFilesFixture.cpp
 *
 * @date Jan 16, 2019
 * @author Antoine Basset
 *
 * @copyright 2019
 *
 */


#ifndef ELEMENTSSERVICES_TESTS_SRC_DATASYNC_FIXTURES_CONFIGFILESFIXTURE_H_
#define ELEMENTSSERVICES_TESTS_SRC_DATASYNC_FIXTURES_CONFIGFILESFIXTURE_H_

#include <vector>

#include "ElementsServices/DataSync/DataSyncUtils.h"

ElementsServices::DataSync::path theDependencyConfig();

std::vector<ElementsServices::DataSync::path> theLocalFiles();

ElementsServices::DataSync::path theLocalWorkspace();

ElementsServices::DataSync::path absPath(ElementsServices::DataSync::path relPath);

ElementsServices::DataSync::path thePrefixedLocalWorkspace();

ElementsServices::DataSync::path theInvalidDependencyConfig();

ElementsServices::DataSync::path theIrodsFrConfig();

ElementsServices::DataSync::path theWebdavFrConfig();

ElementsServices::DataSync::path theWebdavFrDistantWorkspace();

ElementsServices::DataSync::path theWebdavEsConfig();

ElementsServices::DataSync::path theNoOverwriteConfig();

ElementsServices::DataSync::path aBadConnectionConfig();

#endif  // ELEMENTSSERVICES_TESTS_SRC_DATASYNC_FIXTURES_CONFIGFILESFIXTURE_H_
