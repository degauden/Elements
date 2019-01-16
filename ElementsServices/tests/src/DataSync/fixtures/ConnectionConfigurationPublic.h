/**
 * @file ConnectionConfigurationPublic.cpp
 *
 * @date Jan 16, 2019
 * @author Antoine Basset
 *
 * @copyright 2019
 *
 */

#ifndef ELEMENTSSERVICES_TESTS_SRC_DATASYNC_FIXTURES_CONNECTIONCONFIGURATIONPUBLIC_H_
#define ELEMENTSSERVICES_TESTS_SRC_DATASYNC_FIXTURES_CONNECTIONCONFIGURATIONPUBLIC_H_

#include "ElementsServices/DataSync/ConnectionConfiguration.h"

class ConnectionConfigurationPublic: public ElementsServices::DataSync::ConnectionConfiguration {
public:
  virtual ~ConnectionConfigurationPublic() = default;
  ConnectionConfigurationPublic();
  using ConnectionConfiguration::parseHost;
};

#endif  // ELEMENTSSERVICES_TESTS_SRC_DATASYNC_FIXTURES_CONNECTIONCONFIGURATIONPUBLIC_H_
