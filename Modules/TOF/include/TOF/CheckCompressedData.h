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
/// \file   CheckCompressedData.h
/// \author Nicolo' Jacazio
/// \brief  Checker for the raw compressed data for TOF
///

#ifndef QC_MODULE_TOF_CHECKCOMPRESSEDDATA_H
#define QC_MODULE_TOF_CHECKCOMPRESSEDDATA_H

#include "QualityControl/CheckInterface.h"
#include "Base/MessagePad.h"

namespace o2::quality_control_modules::tof
{

/// \brief  Checker for the data produced by the TOF compressor (i.e. checking raw data)
///
/// \author Nicolo' Jacazio
class CheckCompressedData : public o2::quality_control::checker::CheckInterface
{
 public:
  /// Default constructor
  CheckCompressedData() = default;
  /// Destructor
  ~CheckCompressedData() override = default;

  // Override interface
  void configure(std::string name) override;
  Quality check(std::map<std::string, std::shared_ptr<MonitorObject>>* moMap) override;
  void beautify(std::shared_ptr<MonitorObject> mo, Quality checkResult) override;
  std::string getAcceptedType() override;

 private:
  float mDiagnosticThresholdPerSlot = 0;

  /// Messages to print on the output PAD
  MessagePad mShifterMessages;

  ClassDefOverride(CheckCompressedData, 1);
};

} // namespace o2::quality_control_modules::tof

#endif // QC_MODULE_TOF_CHECKCOMPRESSEDDATA_H
