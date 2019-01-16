/**
 * @file DependencyConfigurationPublic.cpp
 *
 * @date Jan 16, 2019
 * @author hubert
 *
 * @copyright 2019
 *
 */

#include "ElementsServices/DataSync/DependencyConfiguration.h"

#include "DependencyConfigurationPublic.h"

namespace DataSync = ElementsServices::DataSync;
using DataSync::path;
using DataSync::DependencyConfiguration;

DependencyConfigurationPublic::DependencyConfigurationPublic(
                path distantRoot,
                path localRoot,
                path configFile) :
                DependencyConfiguration(distantRoot, localRoot, configFile) {
}
