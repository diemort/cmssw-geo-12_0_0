/****************************************************************************
 *
 * This is a part of CTPPS validation software
 * Authors:
 *   Jan Kašpar
 *   Laurent Forthomme
 *
 ****************************************************************************/


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLite.h"
#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLiteFwd.h"

#include "TFile.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TProfile2D.h"

#include <map>

//----------------------------------------------------------------------------------------------------

class CTPPSTrackDistributionPlotter : public edm::one::EDAnalyzer<>
{
  public:
    explicit CTPPSTrackDistributionPlotter(const edm::ParameterSet&);

    ~CTPPSTrackDistributionPlotter() override {}

  private:
    void analyze( const edm::Event&, const edm::EventSetup& ) override;
    void endJob() override;

    edm::EDGetTokenT<CTPPSLocalTrackLiteCollection> tracksToken_;

    std::string outputFile_;

    struct RPPlots
    {
      std::unique_ptr<TH2D> h2_y_vs_x;
      std::unique_ptr<TProfile> p_y_vs_x;
      std::unique_ptr<TH1D> h_x;
      std::unique_ptr<TH1D> h_y;

      RPPlots() :
        h2_y_vs_x(new TH2D("", "", 300, -10., +70., 300, -30, +30.)),
        p_y_vs_x(new TProfile("", "", 300, -10., +70.)),
        h_x(new TH1D("", "", 800, -10., +70.)),
        h_y(new TH1D("", "", 300, -15., +15.))
      {}

      void fill(double x, double y)
      {
        h2_y_vs_x->Fill(x, y);
        p_y_vs_x->Fill(x, y);
        h_x->Fill(x);
        h_y->Fill(y);
      }

      void write() const
      {
        h2_y_vs_x->Write("h2_y_vs_x");
        p_y_vs_x->Write("p_y_vs_x");
        h_x->Write("h_x");
        h_y->Write("h_y");
      }
    };

    std::map<unsigned int, RPPlots> rpPlots;


    struct ArmPlots
    {
      std::unique_ptr<TH1D> h_de_x, h_de_y;
      std::unique_ptr<TProfile> p_de_x_vs_x, p_de_y_vs_x;
      std::unique_ptr<TProfile2D> p2_de_x_vs_x_y, p2_de_y_vs_x_y;

      ArmPlots() :
        h_de_x(new TH1D("", ";x^{F} - x^{N}", 100, -1., +1.)),
        h_de_y(new TH1D("", ";y^{F} - y^{N}", 100, -1., +1.)),
        p_de_x_vs_x(new TProfile("", ";x^{N};x^{F} - x^{N}", 40, 0., 40.)),
        p_de_y_vs_x(new TProfile("", ";x^{N};y^{F} - y^{N}", 40, 0., 40.)),
        p2_de_x_vs_x_y(new TProfile2D("", ";x;y", 40, 0., 40., 40, -20., +20.)),
        p2_de_y_vs_x_y(new TProfile2D("", ";x;y", 40, 0., 40., 40, -20., +20.))
      {}

      void fill(double x_N, double y_N, double x_F, double y_F)
      {
        h_de_x->Fill(x_F - x_N);
        h_de_y->Fill(y_F - y_N);

        p_de_x_vs_x->Fill(x_N, x_F - x_N);
        p_de_y_vs_x->Fill(x_N, y_F - y_N);

        p2_de_x_vs_x_y->Fill(x_N, y_N, x_F - x_N);
        p2_de_y_vs_x_y->Fill(x_N, y_N, y_F - y_N);
      }

      void write() const
      {
        h_de_x->Write("h_de_x");
        h_de_y->Write("h_de_y");

        p_de_x_vs_x->Write("p_de_x_vs_x");
        p_de_y_vs_x->Write("p_de_y_vs_x");

        p2_de_x_vs_x_y->Write("p2_de_x_vs_x_y");
        p2_de_y_vs_x_y->Write("p2_de_y_vs_x_y");
      }
    };

    std::map<unsigned int, ArmPlots> armPlots;
};

//----------------------------------------------------------------------------------------------------

CTPPSTrackDistributionPlotter::CTPPSTrackDistributionPlotter( const edm::ParameterSet& iConfig ) :
  tracksToken_( consumes<CTPPSLocalTrackLiteCollection>( iConfig.getParameter<edm::InputTag>( "tagTracks" ) ) ),
  outputFile_( iConfig.getParameter<std::string>("outputFile") )
{
}

//----------------------------------------------------------------------------------------------------

void CTPPSTrackDistributionPlotter::analyze( const edm::Event& iEvent, const edm::EventSetup& )
{
  // get input
  edm::Handle<CTPPSLocalTrackLiteCollection> tracks;
  iEvent.getByToken( tracksToken_, tracks );

  // process tracks
  for (const auto& trk : *tracks) {
    CTPPSDetId rpId(trk.getRPId());
    unsigned int rpDecId = rpId.arm()*100 + rpId.station()*10 + rpId.rp();
    rpPlots[rpDecId].fill(trk.getX(), trk.getY());
  }

  for (const auto& t1 : *tracks) {
    CTPPSDetId rpId1(t1.getRPId());

    for (const auto& t2 : *tracks) {
      CTPPSDetId rpId2(t2.getRPId());

      if (rpId1.arm() != rpId2.arm())
        continue;

      if (rpId1.station() == 0 && rpId2.station() == 2)
        armPlots[rpId1.arm()].fill(t1.getX(), t1.getY(), t2.getX(), t2.getY());
    }
  }
}

//----------------------------------------------------------------------------------------------------

void CTPPSTrackDistributionPlotter::endJob()
{
  auto f_out = std::make_unique<TFile>(outputFile_.c_str(), "recreate");

  for (const auto& it : rpPlots) {
    gDirectory = f_out->mkdir(Form("RP %u", it.first));
    it.second.write();
  }

  for (const auto& it : armPlots) {
    gDirectory = f_out->mkdir(Form("arm %u", it.first));
    it.second.write();
  }
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE( CTPPSTrackDistributionPlotter );

