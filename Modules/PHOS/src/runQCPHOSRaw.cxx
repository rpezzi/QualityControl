// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include <string>
#include <TH1.h>

#include <DataSampling/DataSampling.h>
#include <PHOSWorkflow/ReaderSpec.h>
#include "QualityControl/InfrastructureGenerator.h"
#include "QualityControl/CheckRunner.h"
#include "QualityControl/CheckRunnerFactory.h"
#include "QualityControl/QcInfoLogger.h"

using namespace o2::utilities;

void customize(std::vector<o2::framework::CompletionPolicy>& policies)
{
  o2::utilities::DataSampling::CustomizeInfrastructure(policies);
  o2::quality_control::customizeInfrastructure(policies);
}

void customize(std::vector<o2::framework::ChannelConfigurationPolicy>& policies)
{
  o2::utilities::DataSampling::CustomizeInfrastructure(policies);
}

void customize(std::vector<o2::framework::ConfigParamSpec>& workflowOptions)
{
  workflowOptions.push_back(
    o2::framework::ConfigParamSpec{ "pedestal", o2::framework::VariantType::Bool, false, { "Runs QC of pedestal runs" } });
  workflowOptions.push_back(
    o2::framework::ConfigParamSpec{ "config-path", o2::framework::VariantType::String, "", { "Path to the config file. Overwrite the default paths. Do not use with no-data-sampling." } });
  workflowOptions.push_back(
    o2::framework::ConfigParamSpec{ "no-data-sampling", o2::framework::VariantType::Bool, false, { "Skips data sampling, connects directly the task to the producer." } });
  workflowOptions.push_back(
    o2::framework::ConfigParamSpec{ "local", o2::framework::VariantType::Bool, false, { "Creates only the local part of the QC topology." } });
  workflowOptions.push_back(
    o2::framework::ConfigParamSpec{ "remote", o2::framework::VariantType::Bool, false, { "Creates only the remote part of the QC topology." } });
  workflowOptions.push_back(
    o2::framework::ConfigParamSpec{ "host", o2::framework::VariantType::String, "", { "Name of the host of the local part of the QC topology."
                                                                                      "Necessary to specify when creating topologies on multiple"
                                                                                      " machines, can be omitted for the local development" } });
}

#include <Framework/runDataProcessing.h>
#include <Configuration/ConfigurationFactory.h>
#include <Configuration/ConfigurationInterface.h>

using namespace o2::configuration;

std::string getConfigPath(const o2::framework::ConfigContext& config);

o2::framework::WorkflowSpec defineDataProcessing(o2::framework::ConfigContext const& config)
{
  o2::framework::WorkflowSpec specs;

  // Path to the config file
  std::string qcConfigurationSource = getConfigPath(config);
  LOG(info) << "Using config file=== '" << qcConfigurationSource << "'";

  if (config.options().get<bool>("local") && config.options().get<bool>("remote")) {
    ILOG(Info, Support) << "To create both local and remote QC topologies, one does not have to add any of '--local' or '--remote' flags." << ENDM;
  }

  if (config.options().get<bool>("local") || !config.options().get<bool>("remote")) {

    LOG(info) << "Local GenerateInfrastructure";
    // Generation of Data Sampling infrastructure
    auto configInterface = ConfigurationFactory::getConfiguration(qcConfigurationSource);
    auto dataSamplingTree = configInterface->getRecursive("dataSamplingPolicies");
    DataSampling::GenerateInfrastructure(specs, dataSamplingTree);

    LOG(info) << "Local: generateLocalInfrastructure";
    // Generation of the local QC topology (local QC tasks)
    o2::quality_control::generateLocalInfrastructure(specs, qcConfigurationSource, config.options().get<std::string>("host"));
  }
  if (config.options().get<bool>("remote") || !config.options().get<bool>("local")) {
    // Generation of the remote QC topology (task for QC servers, mergers and all checkers)
    o2::quality_control::generateRemoteInfrastructure(specs, qcConfigurationSource);
  }
  LOG(info) << "Done ";

  return specs;
}

std::string getConfigPath(const o2::framework::ConfigContext& config)
{
  std::string userConfigPath = config.options().get<std::string>("config-path");
  std::string defaultConfigPath = getenv("QUALITYCONTROL_ROOT") != nullptr ? std::string(getenv("QUALITYCONTROL_ROOT")) + "/Modules/PHOS/etc/phosRaw.json" : "$QUALITYCONTROL_ROOT undefined";
  std::string path = userConfigPath == "" ? defaultConfigPath : userConfigPath;
  const std::string qcConfigurationSource = std::string("json:/") + path;
  return qcConfigurationSource;
}
