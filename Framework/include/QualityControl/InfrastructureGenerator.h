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

///
/// \file   InfrastructureGenerator.h
/// \author Piotr Konopka
///

#ifndef QC_CORE_INFRASTRUCTUREGENERATOR_H
#define QC_CORE_INFRASTRUCTUREGENERATOR_H

#include <vector>
#include <string>

namespace o2::framework
{
class CompletionPolicy;
}
#include <Framework/WorkflowSpec.h>
#include <Framework/DataProcessorSpec.h>

namespace o2::quality_control
{
namespace core
{

struct InfrastructureSpec;

/// \brief A factory class which can generate QC topologies given a configuration file.
///
/// A factory class which can generate QC topologies given a configuration file (example in Framework/basic.json and
/// Framework/example-default.json). As QC topologies will be spread on both processing chain machines and dedicated
/// QC servers, a _local_ vs. _remote_ distinction was introduced. Tasks which are _local_ should have taskRunners
/// placed on FLP or EPN machines and their results should be merged and checked on QC servers. The 'remote' option
/// means, that full QC chain should be located on remote (QC) machines. For the laptop development, use
/// generateStandaloneInfrastructure() to obtain the full topology in one go.
///
/// \author Piotr Konopka
class InfrastructureGenerator
{
 public:
  InfrastructureGenerator() = delete;

  /// \brief Generates a standalone QC infrastructure.
  ///
  /// Generates a full QC infrastructure from a configuration file. This function is aimed to use for standalone setups
  /// and local development. It will create both local and remote QC tasks, and CheckRunners running associated Checks.
  ///
  /// \param configurationSource - full path to configuration file, preceded with the backend (e.g. "json://")
  /// \return generated standalone QC workflow
  static framework::WorkflowSpec generateStandaloneInfrastructure(std::string configurationSource);

  /// \brief Generates a standalone QC infrastructure.
  ///
  /// Generates a full QC infrastructure from a configuration file. This function is aimed to use for standalone setups
  /// and local development. It will create both local and remote QC tasks, and CheckRunners running associated Checks.
  ///
  /// \param workflow - existing workflow where QC infrastructure should be placed
  /// \param configurationSource - full path to configuration file, preceded with the backend (e.g. "json://")
  static void generateStandaloneInfrastructure(framework::WorkflowSpec& workflow, std::string configurationSource);

  /// \brief Generates the local part of the QC infrastructure for a specified host.
  ///
  /// Generates the local part of the QC infrastructure for a specified host - taskRunners which are declared in the
  /// configuration to be 'local'.
  ///
  /// \param configurationSource - full path to configuration file, preceded with the backend (e.g. "json://")
  /// \param targetHost - name of the machine
  /// \return generated local QC workflow
  static framework::WorkflowSpec generateLocalInfrastructure(std::string configurationSource, std::string targetHost);

  /// \brief Generates the local part of the QC infrastructure for a specified host.
  ///
  /// Generates the local part of the QC infrastructure for a specified host - taskRunners which are declared in the
  /// configuration to be 'local'.
  ///
  /// \param workflow - existing workflow where QC infrastructure should be placed
  /// \param configurationSource - full path to configuration file, preceded with the backend (e.g. "json://")
  /// \param host - name of the machine
  /// \return generated local QC workflow
  static void generateLocalInfrastructure(framework::WorkflowSpec& workflow, std::string configurationSource, std::string host);

  /// \brief Generates the remote part of the QC infrastructure.
  ///
  /// Generates the remote part of the QC infrastructure - mergers and checkers for 'local' tasks and full QC chain for
  /// 'remote' tasks.
  ///
  /// \param configurationSource - full path to configuration file, preceded with the backend (e.g. "json://")
  /// \return generated remote QC workflow
  static o2::framework::WorkflowSpec generateRemoteInfrastructure(std::string configurationSource);

  /// \brief Generates the remote part of the QC infrastructure.
  ///
  /// Generates the remote part of the QC infrastructure - mergers and checkers for 'local' tasks and full QC chain for
  /// 'remote' tasks.
  ///
  /// \param workflow - existing workflow where QC infrastructure should be placed
  /// \param configurationSource - full path to configuration file, preceded with the backend (e.g. "json://")
  static void generateRemoteInfrastructure(framework::WorkflowSpec& workflow, std::string configurationSource);

  /// \brief Generates the local batch part of the QC infrastructure.
  ///
  /// Generates the local batch part of the QC infrastructure - tasks and a file sink/merger.
  ///
  /// \param workflow - existing workflow where QC infrastructure should be placed
  /// \param configurationSource - full path to configuration file, preceded with the backend (e.g. "json://")
  /// \param sinkFilePath - path to the output file
  static void generateLocalBatchInfrastructure(framework::WorkflowSpec& workflow, std::string configurationSource, std::string sinkFilePath);

  /// \brief Generates the local batch part of the QC infrastructure.
  ///
  /// Generates the local batch part of the QC infrastructure - tasks and a file sink/merger.
  ///
  /// \param configurationSource - full path to configuration file, preceded with the backend (f.e. "json://")
  /// \param sinkFilePath - path to the output file
  /// \return generated local QC workflow
  static framework::WorkflowSpec generateLocalBatchInfrastructure(std::string configurationSource, std::string sinkFilePath);

  /// \brief Generates the remote batch part of the QC infrastructure.
  ///
  /// Generates the remote batch part of the QC infrastructure - file reader, check runners, aggregator runners.
  ///
  /// \param workflow - existing workflow where QC infrastructure should be placed
  /// \param configurationSource - full path to configuration file, preceded with the backend (f.e. "json://")
  /// \param sourceFilePath - path to the input file
  static void generateRemoteBatchInfrastructure(framework::WorkflowSpec& workflow, std::string configurationSource, std::string sourceFilePath);

  /// \brief Generates the remote batch part of the QC infrastructure.
  ///
  /// Generates the remote batch part of the QC infrastructure - file reader, check runners, aggregator runners.
  ///
  /// \param configurationSource - full path to configuration file, preceded with the backend (f.e. "json://")
  /// \param sourceFilePath - path to the input file
  /// \return generated remote batch QC workflow
  static framework::WorkflowSpec generateRemoteBatchInfrastructure(std::string configurationSource, std::string sourceFilePath);

  /// \brief Provides necessary customization of the QC infrastructure.
  ///
  /// Provides necessary customization of the Completion Policies of the QC infrastructure. This is necessary to make
  /// the QC workflow work. Put it inside the following customize() function, before including <Framework/runDataProcessing.cxx>:
  /// \code{.cxx}
  /// void customize(std::vector<CompletionPolicy>& policies)
  /// {
  ///   quality_control::customizeInfrastructure(policies);
  /// }
  /// \endcode
  /// \param policies - completion policies vector
  static void customizeInfrastructure(std::vector<framework::CompletionPolicy>& policies);

  static void printVersion();

 private:
  // Dedicated methods for creating each QC component to hide implementation details.

  static void generateDataSamplingPolicyLocalProxyBind(framework::WorkflowSpec& workflow,
                                                       const std::string& policyName,
                                                       const framework::Inputs& inputSpecs,
                                                       const std::string& localMachine,
                                                       const std::string& localPort,
                                                       const std::string& control);
  static void generateDataSamplingPolicyRemoteProxyConnect(framework::WorkflowSpec& workflow,
                                                           const std::string& policyName,
                                                           const framework::Outputs& outputSpecs,
                                                           const std::string& localMachine,
                                                           const std::string& localPort,
                                                           const std::string& control);
  static void generateDataSamplingPolicyLocalProxyConnect(framework::WorkflowSpec& workflow,
                                                          const std::string& policyName,
                                                          const framework::Inputs& inputSpecs,
                                                          const std::string& remoteMachine,
                                                          const std::string& remotePort,
                                                          const std::string& control);
  static void generateDataSamplingPolicyRemoteProxyBind(framework::WorkflowSpec& workflow,
                                                        const std::string& policyName,
                                                        const framework::Outputs& outputSpecs,
                                                        const std::string& remotePort,
                                                        const std::string& control);
  static void generateLocalTaskLocalProxy(framework::WorkflowSpec& workflow,
                                          size_t id,
                                          std::string taskName,
                                          std::string remoteHost,
                                          std::string remotePort,
                                          const std::string& control);
  static void generateLocalTaskRemoteProxy(framework::WorkflowSpec& workflow,
                                           std::string taskName,
                                           size_t numberOfLocalMachines,
                                           std::string remotePort,
                                           const std::string& control);
  static void generateMergers(framework::WorkflowSpec& workflow,
                              std::string taskName,
                              size_t numberOfLocalMachines,
                              double cycleDurationSeconds,
                              std::string mergingMode,
                              size_t resetAfterCycles,
                              std::string monitoringUrl,
                              std::string detectorName);
  static void generateCheckRunners(framework::WorkflowSpec& workflow, const InfrastructureSpec& infrastructureSpec);
  static void generateAggregator(framework::WorkflowSpec& workflow, const InfrastructureSpec& infrastructureSpec);
  static void generatePostProcessing(framework::WorkflowSpec& workflow, const InfrastructureSpec& infrastructureSpec);
};

} // namespace core

// exposing the class above as a main QC interface, syntactic sugar

inline framework::WorkflowSpec generateStandaloneInfrastructure(std::string configurationSource)
{
  return core::InfrastructureGenerator::generateStandaloneInfrastructure(configurationSource);
}

inline void generateStandaloneInfrastructure(framework::WorkflowSpec& workflow, std::string configurationSource)
{
  core::InfrastructureGenerator::generateStandaloneInfrastructure(workflow, configurationSource);
}

inline framework::WorkflowSpec generateLocalInfrastructure(std::string configurationSource, std::string host)
{
  return core::InfrastructureGenerator::generateLocalInfrastructure(configurationSource, host);
}

inline void generateLocalInfrastructure(framework::WorkflowSpec& workflow, std::string configurationSource, std::string host)
{
  core::InfrastructureGenerator::generateLocalInfrastructure(workflow, configurationSource, host);
}

inline framework::WorkflowSpec generateRemoteInfrastructure(std::string configurationSource)
{
  return core::InfrastructureGenerator::generateRemoteInfrastructure(configurationSource);
}

inline framework::WorkflowSpec generateLocalBatchInfrastructure(std::string configurationSource, std::string sinkFilePath)
{
  return core::InfrastructureGenerator::generateLocalBatchInfrastructure(configurationSource, std::move(sinkFilePath));
}

inline void generateLocalBatchInfrastructure(framework::WorkflowSpec& workflow, std::string configurationSource, std::string sinkFilePath)
{
  core::InfrastructureGenerator::generateLocalBatchInfrastructure(workflow, configurationSource, std::move(sinkFilePath));
}

inline framework::WorkflowSpec generateRemoteBatchInfrastructure(std::string configurationSource, std::string sourceFilePath)
{
  return core::InfrastructureGenerator::generateRemoteBatchInfrastructure(configurationSource, std::move(sourceFilePath));
}

inline void generateRemoteBatchInfrastructure(framework::WorkflowSpec& workflow, std::string configurationSource, std::string sourceFilePath)
{
  core::InfrastructureGenerator::generateRemoteBatchInfrastructure(workflow, configurationSource, std::move(sourceFilePath));
}

inline void generateRemoteInfrastructure(framework::WorkflowSpec& workflow, std::string configurationSource)
{
  core::InfrastructureGenerator::generateRemoteInfrastructure(workflow, configurationSource);
}

inline void customizeInfrastructure(std::vector<framework::CompletionPolicy>& policies)
{
  core::InfrastructureGenerator::customizeInfrastructure(policies);
}

} // namespace o2::quality_control

#endif //QC_CORE_INFRASTRUCTUREGENERATOR_H
