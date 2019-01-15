/*
 * Copyright (C) 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <boost/program_options.hpp>

#include "ElementsServices/DataSync/ConnectionConfiguration.h"
#include "ElementsServices/DataSync/DataSyncUtils.h"

namespace ElementsServices {
namespace DataSync {

ConnectionConfiguration::ConnectionConfiguration (path filename) {
  parseConfigurationFile(filename);
}

bool ConnectionConfiguration::overwritingAllowed () const {
  return overwritingPolicy == OverwritingPolicy::OVERWRITE;
}

void ConnectionConfiguration::parseConfigurationFile (path filename) {
  //TODO clean function

  namespace po = boost::program_options;

  /* Declare options */
  po::options_description options { };
  options.add_options()
      ("host", po::value<std::string>(),
          "Hosting solution: iRODS or WebDAV (case insensitive)")
      ("host-url", po::value<std::string>()->default_value(""),
          "Host URL if needed")
      ("user", po::value<std::string>()->default_value(""),
          "User name if needed")
      ("password", po::value<std::string>()->default_value(""),
          "Password if needed")
      ("overwrite", po::value<std::string>()->default_value("no"),
          "Allow overwriting local files if they already exist")
      ("distant-workspace", po::value<std::string>(),
          "Path to distant repository workspace")
      ("local-workspace", po::value<std::string>(),
          "Path to local repository workspace")
      ("tries", po::value<int>()->default_value(4),
          "Number of download tries");

  /* Get config file path */
  path absPath = confFilePath(filename);

  /* Read config file */
  po::variables_map vm;
  try {
    po::store(po::parse_config_file<char>(absPath.c_str(), options), vm);
    po::notify(vm);
  } catch (std::exception &e) {
    throw e.what();
  }

  /* Configure object */
  parseHost(vm["host"].as<std::string>());
  hostUrl = vm["host-url"].as<std::string>();
  user = vm["user"].as<std::string>();
  password = vm["password"].as<std::string>();
  parseOverwritingPolicy(vm["overwrite"].as<std::string>());
  distantRoot = vm["distant-workspace"].as<std::string>();
  localRoot = localWorkspacePrefix() / vm["local-workspace"].as<std::string>();
  tries = vm["tries"].as<int>();

}

void ConnectionConfiguration::parseHost (std::string name) {
  const std::string uncased = lower(name);
  if (uncased == "irods")
    host = DataHost::IRODS;
  else if (uncased == "webdav")
    host = DataHost::WEBDAV;
  else if (uncased == "dss")
    host = DataHost::DSS;
  else
    throw UnknownHost(name);
}

void ConnectionConfiguration::parseOverwritingPolicy (std::string policy) {
  const std::vector<std::string> overwriteAllowedOptions = { "true", "yes", "y" };
  const std::vector<std::string> overwriteForbiddenOptions = { "false", "no", "n" };
  std::string uncased = lower(policy);
  if (valueIsListed(uncased, overwriteAllowedOptions))
    overwritingPolicy = OverwritingPolicy::OVERWRITE;
  else if (valueIsListed(uncased, overwriteForbiddenOptions))
    overwritingPolicy = OverwritingPolicy::ABORT;
  else
    throw std::runtime_error("I don't know this overwriting policy: " + policy);
}

}
}
