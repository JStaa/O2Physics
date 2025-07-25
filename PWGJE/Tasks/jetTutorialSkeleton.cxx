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

// jet tutorial task for hands on tutorial session (16/11/2024)
//
/// \author Nima Zardoshti <nima.zardoshti@cern.ch>
//

#include "PWGJE/Core/JetDerivedDataUtilities.h"
#include "PWGJE/DataModel/Jet.h"
#include "PWGJE/DataModel/JetReducedData.h"

#include "Framework/ASoA.h"
#include "Framework/AnalysisTask.h"
#include "Framework/HistogramRegistry.h"
#include <Framework/Configurable.h>
#include <Framework/HistogramSpec.h>
#include <Framework/InitContext.h>
#include <Framework/runDataProcessing.h>

#include <string>
#include <vector>
using namespace o2;
using namespace o2::framework;
using namespace o2::framework::expressions;

struct JetTutorialSkeletonTask {
  HistogramRegistry registry{"registry",
                             {{"h_collisions", "event status;event status;entries", {HistType::kTH1F, {{4, 0.0, 4.0}}}},
                              {"h_track_pt", "track pT;#it{p}_{T,track} (GeV/#it{c});entries", {HistType::kTH1F, {{200, 0., 200.}}}},
                              {"h_track_eta", "track #eta;#eta_{track};entries", {HistType::kTH1F, {{100, -1.0, 1.0}}}},
                              {"h_track_phi", "track #varphi;#varphi_{track};entries", {HistType::kTH1F, {{80, -1.0, 7.}}}},
                              {"h_track_chi2PerCluster", "track #chi^{2} per cluste ;#chi^{2};entries", {HistType::kTH1F, {{100, 0, 40}}}},
                              {"h_jet_pt", "jet pT;#it{p}_{T,jet} (GeV/#it{c});entries", {HistType::kTH1F, {{200, 0., 200.}}}},
                              {"h_jet_eta", "jet #eta;#eta_{jet};entries", {HistType::kTH1F, {{100, -1.0, 1.0}}}},
                              {"h_jet_phi", "jet #phi;#phi_{jet};entries", {HistType::kTH1F, {{80, -1.0, 7.}}}},
                              {"h_jet_pt_rhosub", "jet pT bkg sub;#it{p}_{T,jet} (GeV/#it{c});entries", {HistType::kTH1F, {{200, 0., 200.}}}},
                              {"h_jet_pt_constsub", "jet pT bkg sub;#it{p}_{T,jet} (GeV/#it{c});entries", {HistType::kTH1F, {{200, 0., 200.}}}},
                              {"h_part_jet_pt", "particle level jet pT;#it{p}_{T,jet part} (GeV/#it{c});entries", {HistType::kTH1F, {{200, 0., 200.}}}},
                              {"h_part_jet_eta", "particle level jet #eta;#eta_{jet part};entries", {HistType::kTH1F, {{100, -1.0, 1.0}}}},
                              {"h_part_jet_phi", "particle level jet #phi;#phi_{jet part};entries", {HistType::kTH1F, {{80, -1.0, 7.}}}},
                              {"h_jet_ntracks", "jet N tracks;N_{jet tracks};entries", {HistType::kTH1F, {{40, -0.5, 39.5}}}},
                              {"h_jet_angularity", "jet angularity ;#lambda_{1};entries", {HistType::kTH1F, {{5, 0.0, 0.5}}}},
                              {"h_jet_angularity_constsub", "jet angularity bkg sub;#lambda_{1};entries", {HistType::kTH1F, {{5, 0.0, 0.5}}}},
                              {"h_full_jet_pt", "jet pT;#it{p}_{T,jet} (GeV/#it{c});entries", {HistType::kTH1F, {{200, 0., 200.}}}},
                              {"h_full_jet_eta", "jet #eta;#eta_{jet};entries", {HistType::kTH1F, {{100, -1.0, 1.0}}}},
                              {"h_full_jet_phi", "jet #phi;#phi_{jet};entries", {HistType::kTH1F, {{80, -1.0, 7.}}}},
                              {"h_full_jet_ntracks", "jet N tracks;N_{jet tracks};entries", {HistType::kTH1F, {{40, -0.5, 39.5}}}},
                              {"h_full_jet_nclusters", "jet N clusters;N_{jet clusters};entries", {HistType::kTH1F, {{40, -0.5, 39.5}}}},
                              {"h_full_jet_angularity", "jet angularity ;#lambda_{1};entries", {HistType::kTH1F, {{5, 0.0, 0.5}}}},
                              {"h_part_jet_angularity", "jet angularity ;#lambda_{1};entries", {HistType::kTH1F, {{5, 0.0, 0.5}}}},
                              {"h_recoil_jet_pt", "jet pT;#it{p}_{T,jet} (GeV/#it{c});entries", {HistType::kTH1F, {{200, 0., 200.}}}},
                              {"h_recoil_jet_eta", "jet #eta;#eta_{jet};entries", {HistType::kTH1F, {{100, -1.0, 1.0}}}},
                              {"h_recoil_jet_phi", "jet #phi;#phi_{jet};entries", {HistType::kTH1F, {{80, -1.0, 7.}}}},
                              {"h_recoil_jet_dphi", "hadron-jet #Delta#phi;#Delta#phi_{jet,trigger hadron};entries", {HistType::kTH1F, {{40, -2.0, 2.0}}}},
                              {"h_matched_jets_pt", "#it{p}_{T,jet part}; #it{p}_{T,jet det}", {HistType::kTH2F, {{100, 0., 20.}, {100, 0., 20.0}}}},
                              {"h_matched_jets_eta", "#eta_{jet part}; #eta_{jet det}", {HistType::kTH2F, {{100, -1.0, 1.0}, {100, -1.0, 1.0}}}},
                              {"h_matched_jets_phi", "#phi_{jet part}; #phi_{jet det}", {HistType::kTH2F, {{80, -1.0, 7.}, {80, -1.0, 7.}}}}}};

  Configurable<float> vertexZCut{"vertexZCut", 10.0f, "Accepted z-vertex range"};

  Configurable<float> jetPtMin{"jetPtMin", 5.0, "minimum jet pT cut"};
  Configurable<float> jetR{"jetR", 0.4, "jet resolution parameter"};

  Configurable<std::string> eventSelections{"eventSelections", "sel8", "choose event selection"};
  Configurable<std::string> trackSelections{"trackSelections", "globalTracks", "set track selections"};

  Configurable<float> kappa{"kappa", 1.0, "angularity kappa"};
  Configurable<float> alpha{"alpha", 1.0, "angularity alpha"};

  Configurable<std::string> triggerMasks{"triggerMasks", "", "possible JE Trigger masks: fJetChLowPt,fJetChHighPt,fTrackLowPt,fTrackHighPt,fJetD0ChLowPt,fJetD0ChHighPt,fJetLcChLowPt,fJetLcChHighPt,fEMCALReadout,fJetFullHighPt,fJetFullLowPt,fJetNeutralHighPt,fJetNeutralLowPt,fGammaVeryHighPtEMCAL,fGammaVeryHighPtDCAL,fGammaHighPtEMCAL,fGammaHighPtDCAL,fGammaLowPtEMCAL,fGammaLowPtDCAL,fGammaVeryLowPtEMCAL,fGammaVeryLowPtDCAL"};

  std::vector<int> eventSelectionBits;
  int trackSelection = -1;
  std::vector<int> triggerMaskBits;

  void init(o2::framework::InitContext&)
  {
    eventSelectionBits = jetderiveddatautilities::initialiseEventSelectionBits(static_cast<std::string>(eventSelections));
    trackSelection = jetderiveddatautilities::initialiseTrackSelection(static_cast<std::string>(trackSelections));
    triggerMaskBits = jetderiveddatautilities::initialiseTriggerMaskBits(triggerMasks);
  }

  Filter jetCuts = aod::jet::pt > jetPtMin&& aod::jet::r == nround(jetR.node() * 100.0f);
  Filter collisionFilter = nabs(aod::jcollision::posZ) < vertexZCut;
  Filter mcCollisionFilter = nabs(aod::jmccollision::posZ) < vertexZCut;

  Preslice<soa::Filtered<aod::ChargedMCParticleLevelJets>> perMcCollisionJets = aod::jet::mcCollisionId;

  void processDummy(aod::JDummys const&)
  {
  }
  PROCESS_SWITCH(JetTutorialSkeletonTask, processDummy, "dummy process", false);

  /*
    void processCollisions(aod::JetCollision const& collision, aod::JetTracks const& tracks)
    {

    }
    PROCESS_SWITCH(JetTutorialSkeletonTask, processCollisions, "process JE collisions", false);

    void processCollisionsWithExternalTracks(soa::Filtered<aod::JetCollisions>::iterator const& collision, soa::Join<aod::JetTracks, aod::JTrackPIs> const& tracks, soa::Join<aod::Tracks, aod::TracksExtra, aod::TracksDCA, aod::TrackSelection> const&)
    {

    }
    PROCESS_SWITCH(JetTutorialSkeletonTask, processCollisionsWithExternalTracks, "process JE collisions with access to the original track table", false);

    void processDataCharged(soa::Filtered<aod::JetCollisions>::iterator const& collision, soa::Filtered<aod::ChargedJets> const& jets)
    {

    }
    PROCESS_SWITCH(JetTutorialSkeletonTask, processDataCharged, "charged jets in data", false);

    void processMCDetectorLevelCharged(soa::Filtered<aod::JetCollisions>::iterator const& collision, soa::Filtered<aod::ChargedMCDetectorLevelJets> const& jets)
    {

    }
    PROCESS_SWITCH(JetTutorialSkeletonTask, processMCDetectorLevelCharged, "charged jets in detector level MC", false);

    void processMCDetectorLevelWeightedCharged(soa::Filtered<aod::JetCollisionsMCD>::iterator const& collision, aod::JetMcCollisions const& ,soa::Filtered<aod::ChargedMCDetectorLevelJets> const& jets)
    {

    }
    PROCESS_SWITCH(JetTutorialSkeletonTask, processMCDetectorLevelWeightedCharged, "charged jets in weighted detector level MC", false);

    void processMCParticleLevelCharged(soa::Filtered<aod::JetMcCollisions>::iterator const& mcCollision, soa::Filtered<aod::ChargedMCParticleLevelJets> const& jets)
    {

    }
    PROCESS_SWITCH(JetTutorialSkeletonTask, processMCParticleLevelCharged, "charged jets in particle level MC", false);

    void processMCCharged(soa::Filtered<aod::JetCollisionsMCD>::iterator const& collision, aod::JetMcCollisions const& , soa::Filtered<aod::ChargedMCDetectorLevelJets> const& mcdjets, soa::Filtered<aod::ChargedMCParticleLevelJets> const& mcpjets)
    {

    }
    PROCESS_SWITCH(JetTutorialSkeletonTask, processMCCharged, "charged jets in detector and particle level MC", false);


    using JetMCPTable = soa::Filtered<soa::Join<aod::ChargedMCParticleLevelJets, aod::ChargedMCParticleLevelJetConstituents, aod::ChargedMCParticleLevelJetsMatchedToChargedMCDetectorLevelJets>>;
    void processMCMatchedCharged(soa::Filtered<aod::JetCollisionsMCD>::iterator const& collision,
                                 soa::Filtered<soa::Join<aod::ChargedMCDetectorLevelJets, aod::ChargedMCDetectorLevelJetConstituents, aod::ChargedMCDetectorLevelJetsMatchedToChargedMCParticleLevelJets>> const& mcdjets,
                                 JetMCPTable const&,
                                 aod::JetTracks const&,
                                 aod::JetParticles const&)
    {

    }
    PROCESS_SWITCH(JetTutorialSkeletonTask, processMCMatchedCharged, "matched detector and particle level charged jets", false);

    void processDataSubstructureCharged(soa::Filtered<aod::JetCollisions>::iterator const& collision, soa::Filtered<soa::Join<aod::ChargedJets, aod::ChargedJetConstituents>>const& jets, aod::JetTracks const&)
    {

    }
    PROCESS_SWITCH(JetTutorialSkeletonTask, processDataSubstructureCharged, "charged jet substructure", false);

  void processDataFull(soa::Filtered<aod::JetCollisions>::iterator const& , soa::Filtered<aod::FullJets> const& jets)
    {

    }
    PROCESS_SWITCH(JetTutorialSkeletonTask, processDataFull, "full jets in data", false);

  void processDataSubstructureFull(soa::Filtered<aod::JetCollisions>::iterator const& , soa::Filtered<soa::Join<aod::FullJets, aod::FullJetConstituents>> const& jets, aod::JetTracks const&, aod::JetClusters const&)
    {

    }
    PROCESS_SWITCH(JetTutorialSkeletonTask, processDataSubstructureFull, "full jet substructure", false);


    void processMCParticleLevelSubstructureFull(soa::Filtered<aod::JetMcCollisions>::iterator const& mcCollision, soa::Filtered<soa::Join<aod::FullMCParticleLevelJets, aod::FullMCParticleLevelJetConstituents>> const& jets, aod::JetParticles const&)
    {

    }
    PROCESS_SWITCH(JetTutorialSkeletonTask, processMCParticleLevelSubstructureFull, "full particle level jet substructure", false);


    void processRecoilDataCharged(soa::Filtered<aod::JetCollisions>::iterator const& collision, soa::Filtered<aod::ChargedJets> const& jets, aod::JetTracks const& tracks)
    {

    }
    PROCESS_SWITCH(JetTutorialSkeletonTask, processRecoilDataCharged, "hadron-recoil charged jets", false);

    void processDataRhoAreaSubtractedCharged(soa::Filtered<soa::Join<aod::JetCollisions, aod::BkgChargedRhos>>::iterator const& collision, soa::Filtered<aod::ChargedJets> const& jets)
    {

    }
    PROCESS_SWITCH(JetTutorialSkeletonTask, processDataRhoAreaSubtractedCharged, "charged rho-area  subtracted jets", false);

    void processDataConstituentSubtractedCharged(soa::Filtered<aod::JetCollisions>::iterator const& collision, soa::Filtered<aod::ChargedEventWiseSubtractedJets> const& jets)
    {

    }
    PROCESS_SWITCH(JetTutorialSkeletonTask, processDataConstituentSubtractedCharged, "charged constituent subtracted jets", false);


    void processDataConstituentSubtractedSubstructureCharged(soa::Filtered<aod::JetCollisions>::iterator const& collision, soa::Filtered<soa::Join<aod::ChargedEventWiseSubtractedJets, aod::ChargedEventWiseSubtractedJetConstituents>>const& jets, aod::JetTracksSub const&)
    {

    }
    PROCESS_SWITCH(JetTutorialSkeletonTask, processDataConstituentSubtractedSubstructureCharged, "charged constituent subtracted jet substructure", false);

    void processDataTriggered(soa::Filtered<aod::JetCollisions>::iterator const& collision, soa::Filtered<aod::ChargedJets> const& jets)
    {

    }
    PROCESS_SWITCH(JetTutorialSkeletonTask, processDataTriggered, "jets triggered", false);
  */
};

WorkflowSpec defineDataProcessing(ConfigContext const& cfgc) { return WorkflowSpec{adaptAnalysisTask<JetTutorialSkeletonTask>(cfgc, TaskName{"jet-tutorial-skeleton"})}; }
