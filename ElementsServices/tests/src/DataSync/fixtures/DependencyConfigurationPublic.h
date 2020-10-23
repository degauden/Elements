#ifndef _ELEMENTSSERVICES_DATASYNC_DEPENDENCYCONFIGURATIONPUBLIC_H
#define _ELEMENTSSERVICES_DATASYNC_DEPENDENCYCONFIGURATIONPUBLIC_H

#include "ElementsServices/DataSync/DependencyConfiguration.h"

#include "ConfigFilesFixture.h"

class DependencyConfigurationPublic : public ElementsServices::DataSync::DependencyConfiguration {
public:
  virtual ~DependencyConfigurationPublic() = default;
  DependencyConfigurationPublic(ElementsServices::DataSync::path distantRoot,
                                ElementsServices::DataSync::path localRoot,
                                ElementsServices::DataSync::path configFile);
  using ElementsServices::DataSync::DependencyConfiguration::aliasSeparator;
  using ElementsServices::DataSync::DependencyConfiguration::lineHasAlias;
  using ElementsServices::DataSync::DependencyConfiguration::parseLineWithAlias;
  using ElementsServices::DataSync::DependencyConfiguration::parseLineWithoutAlias;
};

#endif
