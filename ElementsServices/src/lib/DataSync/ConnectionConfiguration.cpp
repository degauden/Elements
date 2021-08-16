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
#include <string>
#include <vector>

#include "ElementsServices/DataSync/ConnectionConfiguration.h"
#include "ElementsServices/DataSync/DataSyncUtils.h"

namespace Elements {
inline namespace Services {
namespace DataSync {

using std::string;

ConnectionConfiguration::ConnectionConfiguration(const path& filename) {
  parseConfigurationFile(filename);
}

bool ConnectionConfiguration::overwritingAllowed() const {
  return overwritingPolicy == OverwritingPolicy::OVERWRITE;
}

void ConnectionConfiguration::parseConfigurationFile(const path& filename) {
  // @TODO clean function

  namespace po = boost::program_options;

  /* Declare options */
  po::options_description options{};
  options.add_options()("host", po::value<string>(), "Hosting solution: iRODS or WebDAV (case insensitive)")(
      "host-url", po::value<string>()->default_value(""),
      "Host URL if needed")("user", po::value<string>()->default_value(""), "User name if needed")(
      "password", po::value<string>()->default_value(""), "Password if needed")(
      "overwrite", po::value<string>()->default_value("no"), "Allow overwriting local files if they already exist")(
      "distant-workspace", po::value<string>(), "Path to distant repository workspace")(
      "local-workspace", po::value<string>(),
      "Path to local repository workspace")("tries", po::value<int>()->default_value(4), "Number of download tries");

  /* Get config file path */
  path abs_path = confFilePath(filename);

  /* Read config file */
  po::variables_map vm;
  try {
    po::store(po::parse_config_file<char>(abs_path.c_str(), options), vm);
    po::notify(vm);
  } catch (std::exception& e) {
    throw e.what();
  }

  /* Configure object */
  parseHost(vm["host"].as<string>());
  hostUrl  = vm["host-url"].as<string>();
  user     = vm["user"].as<string>();
  password = vm["password"].as<string>();
  parseOverwritingPolicy(vm["overwrite"].as<string>());
  distantRoot = vm["distant-workspace"].as<string>();
  localRoot   = localWorkspacePrefix() / vm["local-workspace"].as<string>();
  tries       = static_cast<size_t>(vm["tries"].as<int>());
}

void ConnectionConfiguration::parseHost(const string& name) {
  const string uncased = lower(name);
  if (uncased == "irods") {
    host = DataHost::IRODS;
  } else if (uncased == "webdav") {
    host = DataHost::WEBDAV;
  } else {
    throw UnknownHost(name);
  }
}

void ConnectionConfiguration::parseOverwritingPolicy(const string& policy) {

  using std::vector;

  const vector<string> overwrite_allowed_options   = {"true", "yes", "y"};
  const vector<string> overwrite_forbidden_options = {"false", "no", "n"};

  string uncased = lower(policy);
  if (valueIsListed(uncased, overwrite_allowed_options)) {
    overwritingPolicy = OverwritingPolicy::OVERWRITE;
  } else if (valueIsListed(uncased, overwrite_forbidden_options)) {
    overwritingPolicy = OverwritingPolicy::ABORT;
  } else {
    throw std::runtime_error("I don't know this overwriting policy: " + policy);
  }
}

}  // namespace DataSync
}  // namespace Services
}  // namespace Elements
