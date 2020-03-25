/****************************************************************************
 * Authors:
 *   Jan Kašpar
 ****************************************************************************/

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "CondFormats/RunInfo/interface/LHCInfo.h"
#include "CondFormats/DataRecord/interface/LHCInfoRcd.h"

//----------------------------------------------------------------------------------------------------

class XangleBetaStarFilter : public edm::EDFilter
{
  public:
    explicit XangleBetaStarFilter(const edm::ParameterSet&);

  private:
    std::string lhcInfoLabel_;

    double xangle_min_;
    double xangle_max_;

    double beta_star_min_;
    double beta_star_max_;

    virtual bool filter(edm::Event &, const edm::EventSetup &) override;
};

//----------------------------------------------------------------------------------------------------

using namespace std;
using namespace edm;

//----------------------------------------------------------------------------------------------------

XangleBetaStarFilter::XangleBetaStarFilter(const edm::ParameterSet& iConfig) :
  lhcInfoLabel_(iConfig.getParameter<std::string>("lhcInfoLabel")),

  xangle_min_(iConfig.getParameter<double>("xangle_min")),
  xangle_max_(iConfig.getParameter<double>("xangle_max")),
  beta_star_min_(iConfig.getParameter<double>("beta_star_min")),
  beta_star_max_(iConfig.getParameter<double>("beta_star_max"))
{
}

//----------------------------------------------------------------------------------------------------

bool XangleBetaStarFilter::filter(edm::Event& /*iEvent*/, const edm::EventSetup& iSetup)
{
  edm::ESHandle<LHCInfo> hLHCInfo;
  iSetup.get<LHCInfoRcd>().get(lhcInfoLabel_, hLHCInfo);

  return (xangle_min_ <= hLHCInfo->crossingAngle() && hLHCInfo->crossingAngle() < xangle_max_)
    && (beta_star_min_ <= hLHCInfo->betaStar() && hLHCInfo->betaStar() < beta_star_max_);
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE(XangleBetaStarFilter);
