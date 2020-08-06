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
#include "DataFormats/CTPPSReco/interface/TotemRPRecHit.h"
#include "DataFormats/CTPPSReco/interface/TotemRPUVPattern.h"
#include "DataFormats/CTPPSReco/interface/TotemRPLocalTrack.h"

#include "TFile.h"
#include "TH1I.h"

#include <map>
#include <set>
#include <string>

//----------------------------------------------------------------------------------------------------

struct StripInfo
{
  std::set<unsigned int> u_planes;
  std::set<unsigned int> v_planes;

  std::map<unsigned int, unsigned int> clustersPerPlane;

  bool u_tooFull = false;
  bool v_tooFull = false;

  unsigned int u_patterns = 0;
  unsigned int v_patterns = 0;

  bool hasTrack = false;

  bool pat_suff_or_tooFull = false;
};

//----------------------------------------------------------------------------------------------------

struct RPPlots
{
  TH1I *h_any, *h_suff, *h_suff_tr;

  RPPlots(unsigned int n_bins, double ts_min, double ts_max) :
    h_any(new TH1I("", "", n_bins, ts_min, ts_max)),
    h_suff(new TH1I("", "", n_bins, ts_min, ts_max)),
    h_suff_tr(new TH1I("", "", n_bins, ts_min, ts_max))
  {
  }

  void Write() const
  {
    h_any->Write("h_any");
    h_suff->Write("h_suff");
    h_suff_tr->Write("h_suff_tr");
  }
};

//----------------------------------------------------------------------------------------------------

struct ArmPlots
{
  TH1I *h_suff, *h_suff_tr_no, *h_suff_tr_N, *h_suff_tr_F, *h_suff_tr_NF;

  ArmPlots(unsigned int n_bins, double ts_min, double ts_max) :
    h_suff(new TH1I("", "", n_bins, ts_min, ts_max)),
    h_suff_tr_no(new TH1I("", "", n_bins, ts_min, ts_max)),
    h_suff_tr_N(new TH1I("", "", n_bins, ts_min, ts_max)),
    h_suff_tr_F(new TH1I("", "", n_bins, ts_min, ts_max)),
    h_suff_tr_NF(new TH1I("", "", n_bins, ts_min, ts_max))
  {
  }

  void Write() const
  {
    h_suff->Write("h_suff");
    h_suff_tr_no->Write("h_suff_tr_no");
    h_suff_tr_N->Write("h_suff_tr_N");
    h_suff_tr_F->Write("h_suff_tr_F");
    h_suff_tr_NF->Write("h_suff_tr_NF");
  }
};

//----------------------------------------------------------------------------------------------------

struct GlobalPlots
{
  TH1I *h_suff, *h_suff_tr_no, *h_suff_tr_L, *h_suff_tr_R, *h_suff_tr_LR;

  GlobalPlots(unsigned int n_bins, double ts_min, double ts_max) :
    h_suff(new TH1I("", "", n_bins, ts_min, ts_max)),
    h_suff_tr_no(new TH1I("", "", n_bins, ts_min, ts_max)),
    h_suff_tr_L(new TH1I("", "", n_bins, ts_min, ts_max)),
    h_suff_tr_R(new TH1I("", "", n_bins, ts_min, ts_max)),
    h_suff_tr_LR(new TH1I("", "", n_bins, ts_min, ts_max))
  {
  }

  void Write() const
  {
    h_suff->Write("h_suff");
    h_suff_tr_no->Write("h_suff_tr_no");
    h_suff_tr_L->Write("h_suff_tr_L");
    h_suff_tr_R->Write("h_suff_tr_R");
    h_suff_tr_LR->Write("h_suff_tr_LR");
  }
};

//----------------------------------------------------------------------------------------------------

class PPSStripEfficiencyEstimator : public edm::one::EDAnalyzer<>
{
  public:
    explicit PPSStripEfficiencyEstimator(const edm::ParameterSet&);

    ~PPSStripEfficiencyEstimator() override {}

  private:
    void analyze( const edm::Event&, const edm::EventSetup& ) override;
    void endJob() override;
    
    edm::EDGetTokenT<edm::DetSetVector<TotemRPRecHit>> tokenStripHits_;
    edm::EDGetTokenT<edm::DetSetVector<TotemRPUVPattern>> tokenStripPatterns_;
    edm::EDGetTokenT<edm::DetSetVector<TotemRPLocalTrack>> tokenStripTracks_;

    unsigned int rpId_45_N_, rpId_45_F_;
    unsigned int rpId_56_N_, rpId_56_F_;

    unsigned int timestamp_min_, timestamp_max_, timestamp_bs_;

    std::string outputFile_;

    std::map<unsigned int, std::pair<unsigned int, unsigned int>> armMap_;
    std::map<unsigned int, std::pair<unsigned int, unsigned int>> globalMap_;

    std::map<unsigned int, RPPlots> rpPlots_;
    std::map<unsigned int, ArmPlots> armPlots_;
    std::map<unsigned int, GlobalPlots> globalPlots_;
};

//----------------------------------------------------------------------------------------------------

PPSStripEfficiencyEstimator::PPSStripEfficiencyEstimator( const edm::ParameterSet& iConfig ) :
  tokenStripHits_( consumes<edm::DetSetVector<TotemRPRecHit>>(iConfig.getParameter<edm::InputTag>("tagStripHits")) ),
  tokenStripPatterns_( consumes<edm::DetSetVector<TotemRPUVPattern>>(iConfig.getParameter<edm::InputTag>("tagStripPatterns")) ),
  tokenStripTracks_( consumes<edm::DetSetVector<TotemRPLocalTrack>>(iConfig.getParameter<edm::InputTag>("tagStripTracks")) ),

  rpId_45_N_(iConfig.getParameter<unsigned int>("rpId_45_N")),
  rpId_45_F_(iConfig.getParameter<unsigned int>("rpId_45_F")),
  rpId_56_N_(iConfig.getParameter<unsigned int>("rpId_56_N")),
  rpId_56_F_(iConfig.getParameter<unsigned int>("rpId_56_F")),

  timestamp_min_(iConfig.getParameter<unsigned int>("timestamp_min")),
  timestamp_max_(iConfig.getParameter<unsigned int>("timestamp_max")),
  timestamp_bs_(iConfig.getParameter<unsigned int>("timestamp_bs")),

  outputFile_( iConfig.getParameter<std::string>("outputFile") )
{
  // prepare maps
  armMap_[0] = {rpId_45_N_, rpId_45_F_};
  armMap_[1] = {rpId_56_N_, rpId_56_F_};

  globalMap_[0] = {rpId_45_N_, rpId_56_N_};
  globalMap_[1] = {rpId_45_F_, rpId_56_F_};

  // book plots
  double ts_min = timestamp_min_;
  double ts_max = timestamp_max_;
  double ts_w = timestamp_bs_; // s
  unsigned int n_bins = ceil((ts_max - ts_min) / ts_w);
  ts_max = ts_min + n_bins * ts_w;

  for (const auto rpId : { rpId_45_N_, rpId_45_F_, rpId_56_N_, rpId_56_F_ })
    rpPlots_.insert({rpId, RPPlots(n_bins, ts_min, ts_max)});

  for (const auto &ai : armMap_)
    armPlots_.insert({ai.first, ArmPlots(n_bins, ts_min, ts_max)});

  for (const auto &ai : globalMap_)
    globalPlots_.insert({ai.first, GlobalPlots(n_bins, ts_min, ts_max)});
}

//----------------------------------------------------------------------------------------------------

void PPSStripEfficiencyEstimator::analyze(const edm::Event& iEvent, const edm::EventSetup&)
{
  // get input
  edm::Handle<edm::DetSetVector<TotemRPRecHit>> hStripRecHits;
  iEvent.getByToken(tokenStripHits_, hStripRecHits);

  edm::Handle<edm::DetSetVector<TotemRPUVPattern>> hStripPatterns;
  iEvent.getByToken(tokenStripPatterns_, hStripPatterns);

  edm::Handle<edm::DetSetVector<TotemRPLocalTrack>> hStripTracks;
  iEvent.getByToken(tokenStripTracks_, hStripTracks);

  unsigned int timestamp = iEvent.time().unixTime();

  // process strip input
  std::map<unsigned int, StripInfo> stripInfo;

  for (const auto &dsRecHits : *hStripRecHits)
  {
    TotemRPDetId planeId(dsRecHits.detId());
    unsigned int rpDecId = planeId.arm()*100 + planeId.station()*10 + planeId.rp();
    unsigned int planeIdx = planeId.plane();

    stripInfo[rpDecId].clustersPerPlane[planeIdx] = dsRecHits.size();

    if (dsRecHits.size() == 0)
      continue;

    if ((planeIdx % 2) == 0)
      stripInfo[rpDecId].v_planes.insert(planeIdx);
    else
      stripInfo[rpDecId].u_planes.insert(planeIdx);
  }

  for (const auto &dsPatterns : *hStripPatterns)
  {
    TotemRPDetId rpId(dsPatterns.detId());
    unsigned int rpDecId = rpId.arm()*100 + rpId.station()*10 + rpId.rp();

    //printf("%u --> %lu\n", rpDecId, dsPatterns.size());

    for (const auto &pat : dsPatterns)
    {
      //printf("    - fittable = %u, projection = %u, proj U = %u, proj V = %u\n", pat.getFittable(), pat.getProjection(), pat.getProjection() == TotemRPUVPattern::projU, pat.getProjection() == TotemRPUVPattern::projV);

      if (! pat.getFittable())
        continue;

      if (pat.getProjection() == TotemRPUVPattern::projU)
        stripInfo[rpDecId].u_patterns++;
      if (pat.getProjection() == TotemRPUVPattern::projV)
        stripInfo[rpDecId].v_patterns++;
    }
  }

  for (const auto &dsTracks : *hStripTracks)
  {
    TotemRPDetId rpId(dsTracks.detId());
    unsigned int rpDecId = rpId.arm()*100 + rpId.station()*10 + rpId.rp();

    for (const auto &tr : dsTracks)
    {
      if (tr.isValid())
        stripInfo[rpDecId].hasTrack = true;
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

    //bool pl_suff = (rpInfo.u_planes.size() >= 3 && rpInfo.v_planes.size() >= 3);
    //bool pat_suff = (rpInfo.u_patterns >= 1 && rpInfo.v_patterns >= 1);
    rpInfo.pat_suff_or_tooFull = (rpInfo.u_patterns >= 1 || rpInfo.u_tooFull) && (rpInfo.v_patterns >= 1 || rpInfo.v_tooFull);

    //bool destr_suff = (rpInfo.u_patterns >= 1 || rpInfo.u_tooFull) || (rpInfo.v_patterns >= 1 || rpInfo.v_tooFull);
  }

  if (false)
  {
    printf("--------------------------\n");
    printf("%u:%llu\n", iEvent.id().run(), iEvent.id().event());
    printf("%u\n", iEvent.time().unixTime());
    for (const auto &p : stripInfo)
    {
      printf("RP %u\n", p.first);
      printf("    u_planes: %lu, v_planes: %lu\n", p.second.u_planes.size(), p.second.v_planes.size());
      printf("    u_tooFull: %u, v_tooFull: %u\n", p.second.u_tooFull, p.second.v_tooFull);
      printf("    u_patterns: %u, v_patterns: %u\n", p.second.u_patterns, p.second.v_patterns);
      printf("    hasTrack: %u\n", p.second.hasTrack);
    }
  }

  // increment RP counters
  for (const auto &p : stripInfo)
  {
    const unsigned int &rpDecId = p.first;
    const StripInfo &rpInfo = p.second;

    const auto pit = rpPlots_.find(rpDecId);
    if (pit == rpPlots_.end())
      continue;

    pit->second.h_any->Fill(timestamp);
    if (rpInfo.pat_suff_or_tooFull) pit->second.h_suff->Fill(timestamp);
    if (rpInfo.pat_suff_or_tooFull && rpInfo.hasTrack) pit->second.h_suff_tr->Fill(timestamp);
  }

  // increment arm counters
  for (const auto &ai : armMap_)
  {
    const auto it_N = stripInfo.find(ai.second.first);
    const auto it_F = stripInfo.find(ai.second.second);

    if (it_N == stripInfo.end() || it_F == stripInfo.end())
      continue;

    const auto &si_N = it_N->second;
    const auto &si_F = it_F->second;

    const bool suff = si_N.pat_suff_or_tooFull && si_F.pat_suff_or_tooFull;

    auto &pl = armPlots_.find(ai.first)->second;

    if (suff) pl.h_suff->Fill(timestamp);

    if (suff && !si_N.hasTrack && !si_F.hasTrack) pl.h_suff_tr_no->Fill(timestamp);
    if (suff && si_N.hasTrack && !si_F.hasTrack) pl.h_suff_tr_N->Fill(timestamp);
    if (suff && !si_N.hasTrack && si_F.hasTrack) pl.h_suff_tr_F->Fill(timestamp);
    if (suff && si_N.hasTrack && si_F.hasTrack) pl.h_suff_tr_NF->Fill(timestamp);
  }

  // increment global counters
  for (const auto &gi : globalMap_)
  {
    const auto it_L = stripInfo.find(gi.second.first);
    const auto it_R = stripInfo.find(gi.second.second);

    if (it_L == stripInfo.end() || it_R == stripInfo.end())
      continue;

    const auto &si_L = it_L->second;
    const auto &si_R = it_R->second;

    const bool suff = si_L.pat_suff_or_tooFull && si_R.pat_suff_or_tooFull;

    auto &pl = globalPlots_.find(gi.first)->second;

    if (suff) pl.h_suff->Fill(timestamp);

    if (suff && !si_L.hasTrack && !si_R.hasTrack) pl.h_suff_tr_no->Fill(timestamp);
    if (suff && si_L.hasTrack && !si_R.hasTrack) pl.h_suff_tr_L->Fill(timestamp);
    if (suff && !si_L.hasTrack && si_R.hasTrack) pl.h_suff_tr_R->Fill(timestamp);
    if (suff && si_L.hasTrack && si_R.hasTrack) pl.h_suff_tr_LR->Fill(timestamp);
  }
}

//----------------------------------------------------------------------------------------------------

void PPSStripEfficiencyEstimator::endJob()
{
  auto f_out = std::make_unique<TFile>(outputFile_.c_str(), "recreate");

  for (const auto p : rpPlots_)
  {
    char buf[100];
    sprintf(buf, "RP %u", p.first);
    gDirectory = f_out->mkdir(buf);
    p.second.Write();
  }

  for (const auto p : armPlots_)
  {
    char buf[100];
    sprintf(buf, "arm %u", p.first);
    gDirectory = f_out->mkdir(buf);
    p.second.Write();
  }

  for (const auto p : globalPlots_)
  {
    char buf[100];
    sprintf(buf, "global %u", p.first);
    gDirectory = f_out->mkdir(buf);
    p.second.Write();
  }
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE(PPSStripEfficiencyEstimator);
