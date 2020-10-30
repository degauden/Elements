/**
 * @file DependencyConfigurationPublic.cpp
 *
 * @date Jan 16, 2019
 * @author Antoine Basset
 *
 * @copyright 2019
 *
 */

#include "ElementsServices/DataSync/DependencyConfiguration.h"

#include "DependencyConfigurationPublic.h"

namespace DataSync = ElementsServices::DataSync;
using DataSync::path;

DependencyConfigurationPublic::DependencyConfigurationPublic(path distantRoot, path localRoot, path configFile)
    : DataSync::DependencyConfiguration(distantRoot, localRoot, configFile) {}
