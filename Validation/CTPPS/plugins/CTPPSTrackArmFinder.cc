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

#include "TFile.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TMath.h"

#include <map>

enum armMaps { deltaX, deltaY, sigmaX, sigmaY };

//----------------------------------------------------------------------------------------------------

class CTPPSTrackArmFinder : public edm::one::EDAnalyzer<>
{
  public:
    explicit CTPPSTrackArmFinder( const edm::ParameterSet& );
    ~CTPPSTrackArmFinder();

  private:
    virtual void beginJob() override;

    virtual void analyze( const edm::Event&, const edm::EventSetup& ) override;

    virtual void endJob() override;

    edm::EDGetTokenT< std::vector<CTPPSLocalTrackLite> > tracksToken_;

    std::string outputFile;
    std::string armMapFileName;
    double xSigmaMultiplierMin;
    double xSigmaMultiplierStepSize;
    double xSigmaMultiplierStepNumber;
    double ySigmaMultiplierMin;
    double ySigmaMultiplierStepSize;
    double ySigmaMultiplierStepNumber;
    double xSigmaTrackSinglePot;
    double ySigmaTrackSinglePot;

    std::map <int, std::map <armMaps,TH2D*> > armPotCorrelationsMaps_;
    
    struct RPPlots
    {
      TH2D *h2_y_vs_x;
      TProfile *p_y_vs_x;
      TH1D *h_x;

      void init()
      {
        h2_y_vs_x = new TH2D("", "", 300, -10., +50., 300, -30, +30.);
        p_y_vs_x = new TProfile("", "", 300, -10., +50.);
        h_x = new TH1D("", "", 600, -10., +50.);
      }

      void fill(double x, double y)
      {
        if (h2_y_vs_x == NULL)
          init();

        h2_y_vs_x->Fill(x, y);
        p_y_vs_x->Fill(x, y);
        h_x->Fill(x);
      }

      void write() const
      {
        h2_y_vs_x->Write("h2_y_vs_x");
        p_y_vs_x->Write("p_y_vs_x");
        h_x->Write("h_x");
      }
    };

    std::map<unsigned int, RPPlots> rpPlots;



    struct ArmPlots
    {
      std::map <std::pair<unsigned int,unsigned int> , TH2D*> h2_EfficiencyMap;
      std::map <std::pair<unsigned int,unsigned int> , TH2D*> h2_MultipleHitProbabilityMap;
      TH2D* h2_NormalizationMap;
      TH2D *h2_EfficiencyVsSigmaMultiplier;
      TH2D *h2_MultipleHitProbabilityVsSigmaMultiplier;
      TH2D *h2_NormalizationVsSigmaMultiplier;
      TH1D *h1_EfficiencyVsXsigmaMultiplier;
      TH1D *h1_MultipleHitProbabilityVsXsigmaMultiplier;
      TH1D *h1_NormalizationVsXsigmaMultiplier;
      TH1D *h1_EfficiencyVsYsigmaMultiplier;
      TH1D *h1_MultipleHitProbabilityVsYsigmaMultiplier;
      TH1D *h1_NormalizationVsYsigmaMultiplier;
      
      double xArmSigmaMultiplierMin;
      double xArmSigmaMultiplierStepSize;
      double xArmSigmaMultiplierStepNumber;
      double yArmSigmaMultiplierMin;
      double yArmSigmaMultiplierStepSize;
      double yArmSigmaMultiplierStepNumber;


      void setSigmaSteps(double xTmpSigmaMultiplierMin, double xTmpSigmaMultiplierStepSize, unsigned int xTmpSigmaMultiplierStepNumber, double yTmpSigmaMultiplierMin, double yTmpSigmaMultiplierStepSize, unsigned int yTmpSigmaMultiplierStepNumber ){
        xArmSigmaMultiplierMin = xTmpSigmaMultiplierMin;
        xArmSigmaMultiplierStepSize = xTmpSigmaMultiplierStepSize;
        xArmSigmaMultiplierStepNumber = xTmpSigmaMultiplierStepNumber;
        yArmSigmaMultiplierMin = yTmpSigmaMultiplierMin;
        yArmSigmaMultiplierStepSize = yTmpSigmaMultiplierStepSize;
        yArmSigmaMultiplierStepNumber = yTmpSigmaMultiplierStepNumber;

        h2_EfficiencyVsSigmaMultiplier = new TH2D("EfficiencyVsSigmaMultiplier","EfficiencyVsSigmaMultiplier",xArmSigmaMultiplierStepNumber,xArmSigmaMultiplierMin-xArmSigmaMultiplierStepSize*0.5,xArmSigmaMultiplierMin+xArmSigmaMultiplierStepSize*(xArmSigmaMultiplierStepNumber-0.5),yArmSigmaMultiplierStepNumber,yArmSigmaMultiplierMin-yArmSigmaMultiplierStepSize*0.5,yArmSigmaMultiplierMin+yArmSigmaMultiplierStepSize*(yArmSigmaMultiplierStepNumber-0.5));
        h2_MultipleHitProbabilityVsSigmaMultiplier = new TH2D("MultipleHitProbabilityVsSigmaMultiplier","MultipleHitProbabilityVsSigmaMultiplier",xArmSigmaMultiplierStepNumber,xArmSigmaMultiplierMin-xArmSigmaMultiplierStepSize*0.5,xArmSigmaMultiplierMin+xArmSigmaMultiplierStepSize*(xArmSigmaMultiplierStepNumber-0.5),yArmSigmaMultiplierStepNumber,yArmSigmaMultiplierMin-yArmSigmaMultiplierStepSize*0.5,yArmSigmaMultiplierMin+yArmSigmaMultiplierStepSize*(yArmSigmaMultiplierStepNumber-0.5));
        h2_NormalizationVsSigmaMultiplier = new TH2D("NormalizationVsSigmaMultiplier","NormalizationVsSigmaMultiplier",xArmSigmaMultiplierStepNumber,xArmSigmaMultiplierMin-xArmSigmaMultiplierStepSize*0.5,xArmSigmaMultiplierMin+xArmSigmaMultiplierStepSize*(xArmSigmaMultiplierStepNumber-0.5),yArmSigmaMultiplierStepNumber,yArmSigmaMultiplierMin-yArmSigmaMultiplierStepSize*0.5,yArmSigmaMultiplierMin+yArmSigmaMultiplierStepSize*(yArmSigmaMultiplierStepNumber-0.5));
        h2_NormalizationMap = new TH2D("NormalizationMap", "NormalizationMap", 300, -10., +50., 300, -30, +30.);

        h1_EfficiencyVsXsigmaMultiplier = new TH1D("h1_EfficiencyVsXsigmaMultiplier","h1_EfficiencyVsXsigmaMultiplier",xArmSigmaMultiplierStepNumber,xArmSigmaMultiplierMin-xArmSigmaMultiplierStepSize*0.5,xArmSigmaMultiplierMin+xArmSigmaMultiplierStepSize*(xArmSigmaMultiplierStepNumber-0.5));
        h1_MultipleHitProbabilityVsXsigmaMultiplier = new TH1D("h1_MultipleHitProbabilityVsXsigmaMultiplier","h1_MultipleHitProbabilityVsXsigmaMultiplier",xArmSigmaMultiplierStepNumber,xArmSigmaMultiplierMin-xArmSigmaMultiplierStepSize*0.5,xArmSigmaMultiplierMin+xArmSigmaMultiplierStepSize*(xArmSigmaMultiplierStepNumber-0.5));
        h1_NormalizationVsXsigmaMultiplier = new TH1D("h1_NormalizationVsXsigmaMultiplier","h1_NormalizationVsXsigmaMultiplier",xArmSigmaMultiplierStepNumber,xArmSigmaMultiplierMin-xArmSigmaMultiplierStepSize*0.5,xArmSigmaMultiplierMin+xArmSigmaMultiplierStepSize*(xArmSigmaMultiplierStepNumber-0.5));
        h1_EfficiencyVsYsigmaMultiplier = new TH1D("h1_EfficiencyVsYsigmaMultiplier","h1_EfficiencyVsYsigmaMultiplier",yArmSigmaMultiplierStepNumber,yArmSigmaMultiplierMin-yArmSigmaMultiplierStepSize*0.5,yArmSigmaMultiplierMin+yArmSigmaMultiplierStepSize*(yArmSigmaMultiplierStepNumber-0.5));
        h1_MultipleHitProbabilityVsYsigmaMultiplier = new TH1D("h1_MultipleHitProbabilityVsYsigmaMultiplier","h1_MultipleHitProbabilityVsYsigmaMultiplier",yArmSigmaMultiplierStepNumber,yArmSigmaMultiplierMin-yArmSigmaMultiplierStepSize*0.5,yArmSigmaMultiplierMin+yArmSigmaMultiplierStepSize*(yArmSigmaMultiplierStepNumber-0.5));
        h1_NormalizationVsYsigmaMultiplier = new TH1D("h1_NormalizationVsYsigmaMultiplier","h1_NormalizationVsYsigmaMultiplier",yArmSigmaMultiplierStepNumber,yArmSigmaMultiplierMin-yArmSigmaMultiplierStepSize*0.5,yArmSigmaMultiplierMin+yArmSigmaMultiplierStepSize*(yArmSigmaMultiplierStepNumber-0.5));
      

        for(unsigned int xSigmaStep = 0; xSigmaStep<xArmSigmaMultiplierStepNumber; ++xSigmaStep){
          double xSigmaMult=xArmSigmaMultiplierMin+xSigmaStep*xArmSigmaMultiplierStepSize;
          h2_EfficiencyVsSigmaMultiplier->GetXaxis()->SetBinLabel(xSigmaStep+1, Form("%.3f",xSigmaMult));
          h2_MultipleHitProbabilityVsSigmaMultiplier->GetXaxis()->SetBinLabel(xSigmaStep+1, Form("%.3f",xSigmaMult));
          h2_NormalizationVsSigmaMultiplier->GetXaxis()->SetBinLabel(xSigmaStep+1, Form("%.3f",xSigmaMult));
          for(unsigned int ySigmaStep = 0; ySigmaStep<yArmSigmaMultiplierStepNumber; ++ySigmaStep){
            double ySigmaMult=yArmSigmaMultiplierMin+ySigmaStep*yArmSigmaMultiplierStepSize;
            if(xSigmaStep == 0){
              h2_EfficiencyVsSigmaMultiplier->GetYaxis()->SetBinLabel(ySigmaStep+1, Form("%.3f",ySigmaMult));
              h2_MultipleHitProbabilityVsSigmaMultiplier->GetYaxis()->SetBinLabel(ySigmaStep+1, Form("%.3f",ySigmaMult));
              h2_NormalizationVsSigmaMultiplier->GetYaxis()->SetBinLabel(ySigmaStep+1, Form("%.3f",ySigmaMult));
            }
            h2_EfficiencyMap[std::pair<unsigned int,unsigned int>(xSigmaStep,ySigmaStep)] = new TH2D(Form("EfficiencyMap_sigmaX_%.2f_sigmaY_%.2f",xSigmaMult,ySigmaMult), Form("EfficiencyMap sigmaX*%.2f -- sigmaY*%.2f",xSigmaMult,ySigmaMult), 300, -10., +50., 300, -30, +30.);
            h2_MultipleHitProbabilityMap[std::pair<unsigned int,unsigned int>(xSigmaStep,ySigmaStep)] = new TH2D(Form("MultipleHitProbabilityMap_sigmaX_%.2f_sigmaY_%.2f",xSigmaMult,ySigmaMult), Form("MultipleHitProbabilityMap sigmaX*%.2f -- sigmaY*%.2f",xSigmaMult,ySigmaMult), 300, -10., +50., 300, -30, +30.);
          }
        }
 
      }


      void fill(double x, double y, int numberOfMatchingTracks, int xSigmaStep, int ySigmaStep)
      {

        if(numberOfMatchingTracks>=1){
          if(xSigmaStep>=0 && ySigmaStep>=0){
            h2_EfficiencyVsSigmaMultiplier->Fill(xArmSigmaMultiplierMin+xArmSigmaMultiplierStepSize*xSigmaStep,yArmSigmaMultiplierMin+yArmSigmaMultiplierStepSize*ySigmaStep);
            h2_EfficiencyMap[std::pair<unsigned int,unsigned int>(xSigmaStep,ySigmaStep)]->Fill(x,y);
          }
          if(xSigmaStep>=0  && ySigmaStep==-1){
            h1_EfficiencyVsXsigmaMultiplier->Fill(xArmSigmaMultiplierMin+xArmSigmaMultiplierStepSize*xSigmaStep);
          }
          if(xSigmaStep==-1 && ySigmaStep>=0 ){
            h1_EfficiencyVsYsigmaMultiplier->Fill(yArmSigmaMultiplierMin+yArmSigmaMultiplierStepSize*ySigmaStep);
          }
        }
        if(numberOfMatchingTracks>=2){
          if(xSigmaStep>=0 && ySigmaStep>=0){
            h2_MultipleHitProbabilityVsSigmaMultiplier->Fill(xArmSigmaMultiplierMin+xArmSigmaMultiplierStepSize*xSigmaStep,yArmSigmaMultiplierMin+yArmSigmaMultiplierStepSize*ySigmaStep);
            h2_MultipleHitProbabilityMap[std::pair<unsigned int,unsigned int>(xSigmaStep,ySigmaStep)]->Fill(x,y);
          }
          if(xSigmaStep>=0  && ySigmaStep==-1){
            h1_MultipleHitProbabilityVsXsigmaMultiplier->Fill(xArmSigmaMultiplierMin+xArmSigmaMultiplierStepSize*xSigmaStep);
          }
          if(xSigmaStep==-1 && ySigmaStep>=0 ){
            h1_MultipleHitProbabilityVsYsigmaMultiplier->Fill(yArmSigmaMultiplierMin+yArmSigmaMultiplierStepSize*ySigmaStep);
          }
        }
        h2_NormalizationVsSigmaMultiplier->Fill(xArmSigmaMultiplierMin+xArmSigmaMultiplierStepSize*xSigmaStep,yArmSigmaMultiplierMin+yArmSigmaMultiplierStepSize*ySigmaStep);
        if(xSigmaStep == 0 && ySigmaStep == 0) h2_NormalizationMap->Fill(x,y);
        if(xSigmaStep>=0  && ySigmaStep==-1) h1_NormalizationVsXsigmaMultiplier->Fill(xArmSigmaMultiplierMin+xArmSigmaMultiplierStepSize*xSigmaStep);
        if(xSigmaStep==-1 && ySigmaStep>=0 ) h1_NormalizationVsYsigmaMultiplier->Fill(yArmSigmaMultiplierMin+yArmSigmaMultiplierStepSize*ySigmaStep);
      }

      void write() const
      {

        h2_EfficiencyVsSigmaMultiplier->Divide(h2_NormalizationVsSigmaMultiplier);
        h2_EfficiencyVsSigmaMultiplier->SetStats(0);
        h2_EfficiencyVsSigmaMultiplier->SetMinimum(0.);
        h2_EfficiencyVsSigmaMultiplier->SetMaximum(1.);
        h2_EfficiencyVsSigmaMultiplier->Write();
        h2_MultipleHitProbabilityVsSigmaMultiplier->Divide(h2_NormalizationVsSigmaMultiplier);
        h2_MultipleHitProbabilityVsSigmaMultiplier->SetStats(0);
        h2_MultipleHitProbabilityVsSigmaMultiplier->SetMinimum(0.);
        h2_MultipleHitProbabilityVsSigmaMultiplier->SetMaximum(1.);
        h2_MultipleHitProbabilityVsSigmaMultiplier->Write();
        h2_NormalizationVsSigmaMultiplier->SetStats(0);
        h2_NormalizationVsSigmaMultiplier->Write();
        h2_NormalizationMap->SetStats(0);
        h2_NormalizationMap->Write();
        
        h1_EfficiencyVsXsigmaMultiplier->Divide(h1_NormalizationVsXsigmaMultiplier);
        h1_EfficiencyVsXsigmaMultiplier->SetMinimum(0.);
        h1_EfficiencyVsXsigmaMultiplier->SetMaximum(1.);
        h1_EfficiencyVsXsigmaMultiplier->Write();
        h1_MultipleHitProbabilityVsXsigmaMultiplier->Divide(h1_NormalizationVsXsigmaMultiplier);
        h1_MultipleHitProbabilityVsXsigmaMultiplier->SetMinimum(0.);
        h1_MultipleHitProbabilityVsXsigmaMultiplier->SetMaximum(1.);
        h1_MultipleHitProbabilityVsXsigmaMultiplier->Write();
        h1_NormalizationVsXsigmaMultiplier->Write();

        h1_EfficiencyVsYsigmaMultiplier->Divide(h1_NormalizationVsYsigmaMultiplier);
        h1_EfficiencyVsYsigmaMultiplier->SetMinimum(0.);
        h1_EfficiencyVsYsigmaMultiplier->SetMaximum(1.);
        h1_EfficiencyVsYsigmaMultiplier->Write();
        h1_MultipleHitProbabilityVsYsigmaMultiplier->Divide(h1_NormalizationVsYsigmaMultiplier);
        h1_MultipleHitProbabilityVsYsigmaMultiplier->SetMinimum(0.);
        h1_MultipleHitProbabilityVsYsigmaMultiplier->SetMaximum(1.);
        h1_MultipleHitProbabilityVsYsigmaMultiplier->Write();
        h1_NormalizationVsYsigmaMultiplier->Write();


        for(int xBin=1; xBin<=300; ++xBin){
          for(int yBin=1; yBin<=300; ++yBin){
            if(h2_NormalizationMap->GetBinContent(xBin,yBin)==0) h2_NormalizationMap->SetBinContent(xBin,yBin,-1);
          }
        }
        


        for(const auto & h2Map : h2_EfficiencyMap){

          for(int xBin=1; xBin<=300; ++xBin){
            for(int yBin=1; yBin<=300; ++yBin){
              if(h2_NormalizationMap->GetBinContent(xBin,yBin)==-1) h2Map.second->SetBinContent(xBin,yBin,1);
            }
          }

          h2Map.second->Divide(h2_NormalizationMap);
          h2Map.second->SetStats(0);
          h2Map.second->SetMinimum(-0.01);
          h2Map.second->SetMaximum(1.);
          h2Map.second->Write();
          h2_MultipleHitProbabilityMap.at(h2Map.first)->Divide(h2_NormalizationMap);
          h2_MultipleHitProbabilityMap.at(h2Map.first)->SetStats(0);
          h2_MultipleHitProbabilityMap.at(h2Map.first)->SetMinimum(0.);
          h2_MultipleHitProbabilityMap.at(h2Map.first)->SetMaximum(1.);
          h2_MultipleHitProbabilityMap.at(h2Map.first)->Write();
        }
        
      }
    };

    std::map<unsigned int, ArmPlots> armPlots;

};

//----------------------------------------------------------------------------------------------------

CTPPSTrackArmFinder::CTPPSTrackArmFinder( const edm::ParameterSet& iConfig ) :
  tracksToken_( consumes< std::vector<CTPPSLocalTrackLite> >( iConfig.getParameter<edm::InputTag>( "tracksTag" ) ) ),
  outputFile( iConfig.getParameter<std::string>("outputFile") ),
  armMapFileName( iConfig.getParameter<std::string>("armMapFileName") ),
  xSigmaMultiplierMin( iConfig.getParameter<double>("xSigmaMultiplierMin") ),
  xSigmaMultiplierStepSize( iConfig.getParameter<double>("xSigmaMultiplierStepSize") ),
  xSigmaMultiplierStepNumber( iConfig.getParameter<unsigned int>("xSigmaMultiplierStepNumber") ),
  ySigmaMultiplierMin( iConfig.getParameter<double>("ySigmaMultiplierMin") ),
  ySigmaMultiplierStepSize( iConfig.getParameter<double>("ySigmaMultiplierStepSize") ),
  ySigmaMultiplierStepNumber( iConfig.getParameter<unsigned int>("ySigmaMultiplierStepNumber") ),
  xSigmaTrackSinglePot( iConfig.getParameter<double>("xSigmaTrackSinglePot") ),
  ySigmaTrackSinglePot( iConfig.getParameter<double>("ySigmaTrackSinglePot") )
{
}

//----------------------------------------------------------------------------------------------------

CTPPSTrackArmFinder::~CTPPSTrackArmFinder()
{
}

//----------------------------------------------------------------------------------------------------

void CTPPSTrackArmFinder::analyze( const edm::Event& iEvent, const edm::EventSetup& )
{
   // get input
  edm::Handle< std::vector<CTPPSLocalTrackLite> > tracks;
  iEvent.getByToken( tracksToken_, tracks );

  // process tracks
  std::map<unsigned int, std::vector<CTPPSLocalTrackLite> > tracksForMatching;
  for (const auto& trk : *tracks)
  {
    CTPPSDetId rpId(trk.getRPId());
    unsigned int rpDecId = rpId.arm()*100 + rpId.station()*10 + rpId.rp();
    rpPlots[rpDecId].fill(trk.getX(), trk.getY());

    if (tracksForMatching.find(rpDecId)==tracksForMatching.end()) tracksForMatching[rpDecId] = std::vector<CTPPSLocalTrackLite>();
    tracksForMatching[rpDecId].push_back(trk);
  }

  for(unsigned int arm=0; arm<=1; ++arm){
    if(armPlots.find(arm)==armPlots.end()){
      armPlots[arm].setSigmaSteps( xSigmaMultiplierMin, xSigmaMultiplierStepSize, xSigmaMultiplierStepNumber, ySigmaMultiplierMin, ySigmaMultiplierStepSize, ySigmaMultiplierStepNumber);
    }
    if(tracksForMatching[arm*100+3].size()!=tracksForMatching[arm*100+3].size()) continue;
    for(unsigned int xSigmaStep = 0; xSigmaStep<xSigmaMultiplierStepNumber; ++xSigmaStep){
      double xSigmaMult=xSigmaMultiplierMin+xSigmaStep*xSigmaMultiplierStepSize;    
      for(unsigned int ySigmaStep = 0; ySigmaStep<ySigmaMultiplierStepNumber; ++ySigmaStep){
        double ySigmaMult=ySigmaMultiplierMin+ySigmaStep*ySigmaMultiplierStepSize;
        for(const auto & trackNear : tracksForMatching[arm*100+3]){
          int numberOfMatchingTracks = 0;
          int numberOfMatchingTracksInX = 0;
          int numberOfMatchingTracksInY = 0;
          double deltaXValue = armPotCorrelationsMaps_[arm][deltaX]->GetBinContent(armPotCorrelationsMaps_[arm][deltaX]->GetXaxis()->FindBin(trackNear.getX()),armPotCorrelationsMaps_[arm][deltaX]->GetYaxis()->FindBin(trackNear.getY()));
          double deltaYValue = armPotCorrelationsMaps_[arm][deltaY]->GetBinContent(armPotCorrelationsMaps_[arm][deltaY]->GetXaxis()->FindBin(trackNear.getX()),armPotCorrelationsMaps_[arm][deltaY]->GetYaxis()->FindBin(trackNear.getY()));
          double sigmaXValue = armPotCorrelationsMaps_[arm][sigmaX]->GetBinContent(armPotCorrelationsMaps_[arm][sigmaX]->GetXaxis()->FindBin(trackNear.getX()),armPotCorrelationsMaps_[arm][sigmaX]->GetYaxis()->FindBin(trackNear.getY()));
          double windowXSize = TMath::Sqrt(sigmaXValue*sigmaXValue*xSigmaMult*xSigmaMult+2.*xSigmaTrackSinglePot*xSigmaTrackSinglePot);
          double sigmaYValue = armPotCorrelationsMaps_[arm][sigmaY]->GetBinContent(armPotCorrelationsMaps_[arm][sigmaY]->GetXaxis()->FindBin(trackNear.getX()),armPotCorrelationsMaps_[arm][sigmaY]->GetYaxis()->FindBin(trackNear.getY()));
          double windowYSize = TMath::Sqrt(sigmaYValue*sigmaYValue*ySigmaMult*ySigmaMult+2.*ySigmaTrackSinglePot*ySigmaTrackSinglePot);
          
          double projectedFarX = trackNear.getX()+deltaXValue;
          double projectedFarY = trackNear.getY()+deltaYValue;
          for(const auto & trackFar : tracksForMatching[arm*100+23]){
            if( trackFar.getX() >= projectedFarX - windowXSize && trackFar.getX() <= projectedFarX + windowXSize  &&  trackFar.getY() >= projectedFarY - windowYSize && trackFar.getY() <= projectedFarY + windowYSize ){
              ++numberOfMatchingTracks;
            }
            if(ySigmaStep == 0 && trackFar.getX() >= projectedFarX - windowXSize && trackFar.getX() <= projectedFarX + windowXSize ){
              ++numberOfMatchingTracksInX;
            }
            if(xSigmaStep == 0 && trackFar.getY() >= projectedFarY - windowYSize && trackFar.getY() <= projectedFarY + windowYSize ){
              ++numberOfMatchingTracksInY;
            }
          }
          armPlots[arm].fill(trackNear.getX(),trackNear.getY(),numberOfMatchingTracks,xSigmaStep,ySigmaStep);
          if(xSigmaStep == 0) {
            armPlots[arm].fill(trackNear.getX(),trackNear.getY(),numberOfMatchingTracksInY,-1,ySigmaStep);
          }
          if(ySigmaStep == 0){
            armPlots[arm].fill(trackNear.getX(),trackNear.getY(),numberOfMatchingTracksInX,xSigmaStep,-1);
          }
        }
      }
    }
  }


}

//----------------------------------------------------------------------------------------------------

void CTPPSTrackArmFinder::beginJob()
{
  TFile *armMapFile = TFile::Open(armMapFileName.c_str());
  if(armMapFile==NULL){
    std::cout<<"File "<<armMapFileName<<" does not exist\n";
    exit(0);
  }

  std::map <int ,std::map<armMaps,std::string> > armMapPaths;
  armMapPaths[0][deltaX] = "Arm 0/h2_dXmeanExtendedSmooth3";
  armMapPaths[0][deltaY] = "Arm 0/h2_dYmeanExtendedSmooth3";
  armMapPaths[0][sigmaX] = "Arm 0/h2_dXsigmaExtendedSmooth3";
  armMapPaths[0][sigmaY] = "Arm 0/h2_dYsigmaExtendedSmooth3";
  armMapPaths[1][deltaX] = "Arm 1/h2_dXmeanExtendedSmooth3";
  armMapPaths[1][deltaY] = "Arm 1/h2_dYmeanExtendedSmooth3";
  armMapPaths[1][sigmaX] = "Arm 1/h2_dXsigmaExtendedSmooth3";
  armMapPaths[1][sigmaY] = "Arm 1/h2_dYsigmaExtendedSmooth3";
  

  for(auto & arm : armMapPaths){
    for(auto & hName : arm.second){
      armPotCorrelationsMaps_[arm.first][hName.first] = (TH2D*)armMapFile->Get(hName.second.c_str());
      if(armPotCorrelationsMaps_[arm.first][hName.first]==NULL){
        std::cout<<"Map "<<hName.second<<" not found in file "<<armMapFileName<<std::endl;
        exit(0);
      }
    }
  }


}

//----------------------------------------------------------------------------------------------------

void CTPPSTrackArmFinder::endJob()
{
  TFile *f_out = TFile::Open(outputFile.c_str(), "recreate");
  
  for (const auto it : rpPlots)
  {
    gDirectory = f_out->mkdir(Form("RP %u", it.first));
    it.second.write();
  }


  for (const auto it : armPlots)
  {
    gDirectory = f_out->mkdir(Form("Arm %u", it.first));
    it.second.write();
  }
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE( CTPPSTrackArmFinder );
