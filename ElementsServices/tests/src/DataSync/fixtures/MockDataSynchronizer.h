#ifndef _ELEMENTSSERVICES_DATASYNC_MOCKDATASYNCHRONIZER_H
#define _ELEMENTSSERVICES_DATASYNC_MOCKDATASYNCHRONIZER_H

#include "ElementsServices/DataSync/DataSynchronizer.h"

namespace ElementsServices {
namespace DataSync {

class MockDataSynchronizer : public DataSynchronizer {

public:

  virtual ~MockDataSynchronizer () = default;

  MockDataSynchronizer (
      path connection=theWebdavFrConfig(),
      path dependency=theDependencyConfig()) :
        DataSynchronizer(
            ConnectionConfiguration(connection),
            DependencyConfiguration(theWebdavFrDistantWorkspace(), theLocalWorkspace(), dependency)
        ) {}

  virtual std::string createDownloadCommand (
      path distantFile,
      path localFile) const override {
    std::string cmd = std::string("echo ") + distantFile.string() + "\t" + localFile.string();
    return cmd;
  }
  
  std::map<path, path> fileMap() {
    return m_fileMap;
  }

};

}
}

#endif

