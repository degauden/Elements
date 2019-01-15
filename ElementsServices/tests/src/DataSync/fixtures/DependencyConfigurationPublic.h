#ifndef _ELEMENTSSERVICES_DATASYNC_DEPENDENCYCONFIGURATIONPUBLIC_H
#define _ELEMENTSSERVICES_DATASYNC_DEPENDENCYCONFIGURATIONPUBLIC_H

#include "ElementsServices/DataSync/DependencyConfiguration.h"
#include "ConfigFilesFixture.h"

using namespace ElementsServices::DataSync;

class DependencyConfigurationPublic: public DependencyConfiguration {
public:
  virtual ~DependencyConfigurationPublic () = default;
  DependencyConfigurationPublic (
      path distantRoot,
      path localRoot,
      path configFile) :
      DependencyConfiguration(distantRoot, localRoot, configFile) {
  }
  using DependencyConfiguration::aliasSeparator;
  using DependencyConfiguration::lineHasAlias;
  using DependencyConfiguration::parseLineWithAlias;
  using DependencyConfiguration::parseLineWithoutAlias;
};

# endif
