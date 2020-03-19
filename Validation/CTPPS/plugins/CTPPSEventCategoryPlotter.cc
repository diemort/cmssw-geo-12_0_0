/****************************************************************************
 *
 * Authors:
 *   Jan Kašpar
 *
 ****************************************************************************/

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"
#include "DataFormats/CTPPSDetId/interface/TotemRPDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLite.h"
#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLiteFwd.h"
#include "DataFormats/CTPPSReco/interface/TotemRPRecHit.h"
#include "DataFormats/CTPPSReco/interface/TotemRPUVPattern.h"

#include "DataFormats/ProtonReco/interface/ForwardProton.h"
#include "DataFormats/ProtonReco/interface/ForwardProtonFwd.h"

#include "TFile.h"
#include "TGraph.h"

#include <map>
#include <set>
#include <string>

//----------------------------------------------------------------------------------------------------

class CTPPSEventCategoryPlotter : public edm::one::EDAnalyzer<>
{
  public:
    explicit CTPPSEventCategoryPlotter(const edm::ParameterSet&);

    ~CTPPSEventCategoryPlotter() override {}

  private:
    void analyze( const edm::Event&, const edm::EventSetup& ) override;
    void endJob() override;
    
    edm::EDGetTokenT<edm::DetSetVector<TotemRPRecHit>> tokenStripHits_;
    edm::EDGetTokenT<edm::DetSetVector<TotemRPUVPattern>> tokenStripPatterns_;

    edm::EDGetTokenT<CTPPSLocalTrackLiteCollection> tokenTracks_;
    edm::EDGetTokenT<reco::ForwardProtonCollection> tokenRecoProtonsMultiRP_;

    unsigned int rpId_45_N_, rpId_45_F_;
    unsigned int rpId_56_N_, rpId_56_F_;

    std::string outputFile_;

    unsigned int denominator_;
    std::map<std::string, unsigned int> counter_;
};

//----------------------------------------------------------------------------------------------------

CTPPSEventCategoryPlotter::CTPPSEventCategoryPlotter( const edm::ParameterSet& iConfig ) :
  tokenStripHits_( consumes<edm::DetSetVector<TotemRPRecHit>>(iConfig.getParameter<edm::InputTag>("tagStripHits")) ),
  tokenStripPatterns_( consumes<edm::DetSetVector<TotemRPUVPattern>>(iConfig.getParameter<edm::InputTag>("tagStripPatterns")) ),
  tokenTracks_( consumes<CTPPSLocalTrackLiteCollection>( iConfig.getParameter<edm::InputTag>( "tagTracks" ) ) ),
  tokenRecoProtonsMultiRP_ (consumes<reco::ForwardProtonCollection>(iConfig.getParameter<edm::InputTag>("tagRecoProtonsMultiRP"))),

  rpId_45_N_(iConfig.getParameter<unsigned int>("rpId_45_N")),
  rpId_45_F_(iConfig.getParameter<unsigned int>("rpId_45_F")),
  rpId_56_N_(iConfig.getParameter<unsigned int>("rpId_56_N")),
  rpId_56_F_(iConfig.getParameter<unsigned int>("rpId_56_F")),

  outputFile_( iConfig.getParameter<std::string>("outputFile") ),

  denominator_(0)
{
}

//----------------------------------------------------------------------------------------------------

void CTPPSEventCategoryPlotter::analyze( const edm::Event& iEvent, const edm::EventSetup& )
{
  // get input
  edm::Handle<edm::DetSetVector<TotemRPRecHit>> hStripHits;
  iEvent.getByToken(tokenStripHits_, hStripHits);

  edm::Handle<edm::DetSetVector<TotemRPUVPattern>> hStripPatterns;
  iEvent.getByToken(tokenStripPatterns_, hStripPatterns);

  edm::Handle<CTPPSLocalTrackLiteCollection> tracks;
  iEvent.getByToken( tokenTracks_, tracks );

  edm::Handle<reco::ForwardProtonCollection> hRecoProtonsMultiRP;
  iEvent.getByToken(tokenRecoProtonsMultiRP_, hRecoProtonsMultiRP);

  // process strip data
  struct StripInfo
  {
      std::map<unsigned int, unsigned int> clustersPerPlane;

      unsigned int u_patterns = 0;
      unsigned int v_patterns = 0;

      bool u_tooFull = false;
      bool v_tooFull = false;

      bool suff = false;
  };

  std::map<unsigned int, StripInfo> stripInfo;

  for (const auto &dsRecHits : *hStripHits)
  {
    TotemRPDetId planeId(dsRecHits.detId());
    unsigned int rpDecId = planeId.arm()*100 + planeId.station()*10 + planeId.rp();
    unsigned int planeIdx = planeId.plane();

    stripInfo[rpDecId].clustersPerPlane[planeIdx] = dsRecHits.size();

    if (dsRecHits.size() == 0)
      continue;
  }

  for (const auto &dsPatterns : *hStripPatterns)
  {
    TotemRPDetId rpId(dsPatterns.detId());
    unsigned int rpDecId = rpId.arm()*100 + rpId.station()*10 + rpId.rp();

    for (const auto &pat : dsPatterns)
    {
      if (! pat.getFittable())
        continue;

      if (pat.getProjection() == TotemRPUVPattern::projU)
        stripInfo[rpDecId].u_patterns++;
      if (pat.getProjection() == TotemRPUVPattern::projV)
        stripInfo[rpDecId].v_patterns++;
    }
  }

  for (auto &p : stripInfo)
  {
    auto &rpInfo = p.second;

    unsigned int n_too_full_u = 0, n_too_full_v = 0;

    for (const auto &clP : rpInfo.clustersPerPlane)
    {
      if (clP.second <= 5)
        continue;

      if ((clP.first % 2) == 1)
        n_too_full_u++;
      else
        n_too_full_v++;
    }

    rpInfo.u_tooFull = (n_too_full_u >= 3);
    rpInfo.v_tooFull = (n_too_full_v >= 3);

    rpInfo.suff = (rpInfo.u_patterns >= 1 || rpInfo.u_tooFull) && (rpInfo.v_patterns >= 1 || rpInfo.v_tooFull);
  }

  // process tracks
  std::map<unsigned int, unsigned int> trackMultiplicity;

  for (const auto& trk : *tracks)
  {
    CTPPSDetId rpId(trk.getRPId());
    unsigned int rpDecId = rpId.arm()*100 + rpId.station()*10 + rpId.rp();
    //bool rpPixel = (rpId.subdetId() == CTPPSDetId::sdTrackingPixel);
    trackMultiplicity[rpDecId]++;
  }

  // process protons
  std::map<unsigned int, unsigned int> multiRPMultiplicity;

  for (const auto &proton : *hRecoProtonsMultiRP)
  {
    CTPPSDetId rpId((*proton.contributingLocalTracks().begin())->getRPId());
    unsigned int armId = rpId.arm();

    if (proton.validFit())
      multiRPMultiplicity[armId]++;
  }

  // evaluate flags
  const bool suff_45_N = (trackMultiplicity[rpId_45_N_] > 0) || stripInfo[rpId_45_N_].suff;
  const bool suff_45_F = (trackMultiplicity[rpId_45_F_] > 0) || stripInfo[rpId_45_F_].suff;
  const bool suff_56_N = (trackMultiplicity[rpId_56_N_] > 0) || stripInfo[rpId_56_N_].suff;
  const bool suff_56_F = (trackMultiplicity[rpId_56_F_] > 0) || stripInfo[rpId_56_F_].suff;

  const bool suff_45 = suff_45_N && suff_45_F;
  const bool suff_56 = suff_56_N && suff_56_F;

  enum { aNone, a0, a1, a2pl } act_45 = aNone, act_56 = aNone;

  if (multiRPMultiplicity[0] > 1)
    act_45 = a2pl;
  else if (multiRPMultiplicity[0] == 1)
    act_45 = a1;
  else act_45 = (suff_45) ? a2pl : a0; // multiplicity 0

  if (multiRPMultiplicity[1] > 1)
    act_56 = a2pl;
  else if (multiRPMultiplicity[1] == 1)
    act_56 = a1;
  else act_56 = (suff_56) ? a2pl : a0; // multiplicity 0

  // increase counters
  denominator_++;

  counter_["total"]++;

  if (act_45 == a0) counter_["45_0"]++;
  if (act_45 == a1) counter_["45_1"]++;
  if (act_45 == a2pl) counter_["45_2pl"]++;

  if (act_56 == a0) counter_["56_0"]++;
  if (act_56 == a1) counter_["56_1"]++;
  if (act_56 == a2pl) counter_["56_2pl"]++;

  if (act_45 == a0 && act_56 == a0) counter_["45_0_56_0"]++;
  if (act_45 == a0 && act_56 == a1) counter_["45_0_56_1"]++;
  if (act_45 == a1 && act_56 == a0) counter_["45_1_56_0"]++;
  if (act_45 == a1 && act_56 == a1) counter_["45_1_56_1"]++;
}

//----------------------------------------------------------------------------------------------------

void CTPPSEventCategoryPlotter::endJob()
{
  auto f_out = std::make_unique<TFile>(outputFile_.c_str(), "recreate");

  printf(">> CTPPSEventCategoryPlotter::endJob\n");

  for (const auto &p : counter_)
  {
    const double r = double(p.second) / denominator_;

    printf("    %s: %0.3f\n", p.first.c_str(), r);

    TGraph *g = new TGraph();

    g->SetPoint(0, 0., r);

    g->Write(("g_" + p.first).c_str());
  }
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE( CTPPSEventCategoryPlotter );
