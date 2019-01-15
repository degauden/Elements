#ifndef _ELEMENTSSERVICES_DATASYNC_CONNECTIONCONFIGURATIONPUBLIC_H
#define _ELEMENTSSERVICES_DATASYNC_CONNECTIONCONFIGURATIONPUBLIC_H

#include "ElementsServices/DataSync/ConnectionConfiguration.h"
#include "ConfigFilesFixture.h"

using namespace ElementsServices::DataSync;

class ConnectionConfigurationPublic: public ConnectionConfiguration {
public:
  virtual ~ConnectionConfigurationPublic () = default;
  ConnectionConfigurationPublic () :
      ConnectionConfiguration(theWebdavFrConfig()) {
  }
  using ConnectionConfiguration::parseHost;
};

#endif
