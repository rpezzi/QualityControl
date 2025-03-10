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
/// \file   TrackClusterCheck.cxx
/// \author Laura Serksnyte
///
#include <iostream>

#include "TPC/TrackClusterCheck.h"
#include "QualityControl/MonitorObject.h"
#include "QualityControl/Quality.h"

#include <fairlogger/Logger.h>
// ROOT
#include <TH1.h>
#include <TList.h>
#include <TPaveText.h>

using namespace std;

namespace o2::quality_control_modules::tpc
{

void TrackClusterCheck::configure(std::string) {}

Quality TrackClusterCheck::check(std::map<std::string, std::shared_ptr<MonitorObject>>* moMap)
{
  auto mo = moMap->begin()->second;
  Quality result = Quality::Null;

  // Check whether the cluster number for a track is smaller than 40 or 20 in Track task.
  if (mo->getName() == "hNClustersBeforeCuts") {
    auto* h = dynamic_cast<TH1F*>(mo->getObject());
    result = Quality::Good;
    for (int i = 0; i < h->GetNbinsX(); i++) {
      if (h->GetBinContent(i) > 0 && h->GetBinCenter(i) < 20) {
        result = Quality::Bad;
      } else if (h->GetBinContent(i) > 0 && h->GetBinCenter(i) < 40 && h->GetBinCenter(i) > 20 && result != Quality::Bad) {
        result = Quality::Medium;
      }
    }
  }

  return result;
}

std::string TrackClusterCheck::getAcceptedType() { return "TH1"; }

void TrackClusterCheck::beautify(std::shared_ptr<MonitorObject> mo, Quality checkResult)
{
  auto* h = dynamic_cast<TH1F*>(mo->getObject());

  TPaveText* msg = new TPaveText(0.5, 0.5, 0.9, 0.75, "NDC");
  h->GetListOfFunctions()->Add(msg);
  msg->SetName(Form("%s_msg", mo->GetName()));

  if (checkResult == Quality::Good) {
    h->SetFillColor(kGreen);
    msg->Clear();
    msg->AddText("Quality::Good");
    msg->AddText("This is a fake test case.");
    msg->AddText("PLEASE IGNORE.");
    msg->SetFillColor(kGreen);
  } else if (checkResult == Quality::Bad) {
    LOG(info) << "Quality::Bad, setting to red";
    h->SetFillColor(kRed);
    msg->Clear();
    msg->AddText("Quality::Bad");
    msg->AddText("This is a fake test case.");
    msg->AddText("PLEASE IGNORE.");
    msg->SetFillColor(kRed);
  } else if (checkResult == Quality::Medium) {
    LOG(info) << "Quality::medium, setting to orange";
    h->SetFillColor(kOrange);
    msg->Clear();
    msg->AddText("Quality::Medium");
    msg->AddText("This is a fake test case.");
    msg->AddText("PLEASE IGNORE.");
    msg->SetFillColor(kOrange);
  } else if (checkResult == Quality::Null) {
    h->SetFillColor(0);
  }
  h->SetLineColor(kBlack);
}

} // namespace o2::quality_control_modules::tpc
