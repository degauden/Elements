/**
 * @file ConnectionConfigurationPublic.cpp
 *
 * @date Jan 16, 2019
 * @author Hubert Degaudenzi
 *
 * @copyright 2019
 *
 */

#include "ElementsServices/DataSync/ConnectionConfiguration.h"

#include "ConfigFilesFixture.h"
#include "ConnectionConfigurationPublic.h"

using ElementsServices::DataSync::ConnectionConfiguration;

ConnectionConfigurationPublic::ConnectionConfigurationPublic() :
          ConnectionConfiguration(theWebdavFrConfig()) {

}
