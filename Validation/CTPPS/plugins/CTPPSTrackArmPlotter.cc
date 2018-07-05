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
#include "TF1.h"
#include "TGraphErrors.h"
#include "TFitResult.h"

#include <map>

enum slope { slopePosNear, slopePosFar, slopeNegNear, slopeNegFar };


std::map<slope,double> getSlopes(TH1D *normProjection, TH1D *meanProjectionCut, double hitShapeCenter, double hitShapeSigma, int minNumberOfHits, double nSigmaMinNear, double nSigmaMaxNear, double nSigmaMinFar){

  std::map<slope,double> slopes;

  TF1 *straightLine = new TF1("straightLine","pol1",-30.,30.);
  straightLine->SetLineColor(kBlack);

  straightLine->SetParameter(0,0.);
  straightLine->SetParameter(1,0.);
  int pointsForFit = 0;
  bool fitIsDone = false;
  double minBin =  normProjection->FindBin(hitShapeCenter+nSigmaMinNear*hitShapeSigma);
  if (normProjection->GetBinCenter(minBin)<hitShapeCenter+nSigmaMinNear*hitShapeSigma) ++minBin;
  double maxBin =  normProjection->FindBin(hitShapeCenter+nSigmaMaxNear*hitShapeSigma);
  if (normProjection->GetBinCenter(maxBin)>hitShapeCenter+nSigmaMaxNear*hitShapeSigma) --maxBin;
  for(int yBin = minBin; yBin <= maxBin; ++yBin){
    if(normProjection->GetBinContent(yBin)>=minNumberOfHits) ++pointsForFit;
    if(pointsForFit>=6) {
      meanProjectionCut->Fit(straightLine,"Q+","",hitShapeCenter+nSigmaMinNear*hitShapeSigma,hitShapeCenter+nSigmaMaxNear*hitShapeSigma);
      fitIsDone = true;
      break;
    }
  }
  if( !fitIsDone ){
    for(int yBin = maxBin+1; yBin<=300; ++yBin){
      if(normProjection->GetBinContent(yBin)>=minNumberOfHits) ++pointsForFit;
      if(pointsForFit>=6) {
        meanProjectionCut->Fit(straightLine,"Q+","",hitShapeCenter+nSigmaMinNear*hitShapeSigma,normProjection->GetBinCenter(yBin));
        break;
      }
    }
  }
  slopes[slopePosNear] = straightLine->GetParameter(1);
  
  straightLine->SetParameter(0,0.);
  straightLine->SetParameter(1,0.);
  pointsForFit = 0;
  minBin =  normProjection->FindBin(hitShapeCenter+nSigmaMinFar*hitShapeSigma);
  if (normProjection->GetBinCenter(minBin)<hitShapeCenter+nSigmaMinFar*hitShapeSigma) ++minBin;
  for(int yBin = minBin; yBin <= 300; ++yBin){
    if(normProjection->GetBinContent(yBin)>=minNumberOfHits) ++pointsForFit;
    if(pointsForFit>=6) {
      meanProjectionCut->Fit(straightLine,"Q+","",hitShapeCenter+nSigmaMinFar*hitShapeSigma,30.);
      break;
    }
  }
 slopes[slopePosFar] = straightLine->GetParameter(1);
  

  
  straightLine->SetParameter(0,0.);
  straightLine->SetParameter(1,0.);
  pointsForFit = 0;
  fitIsDone = false;
  minBin =  normProjection->FindBin(hitShapeCenter-nSigmaMaxNear*hitShapeSigma);
  if (normProjection->GetBinCenter(minBin)<hitShapeCenter-nSigmaMaxNear*hitShapeSigma) ++minBin;
  maxBin =  normProjection->FindBin(hitShapeCenter-nSigmaMinNear*hitShapeSigma);
  if (normProjection->GetBinCenter(maxBin)>hitShapeCenter-nSigmaMinNear*hitShapeSigma) --maxBin;
  for(int yBin = minBin; yBin <= maxBin; ++yBin){
    if(normProjection->GetBinContent(yBin)>=minNumberOfHits) ++pointsForFit;
    if(pointsForFit>=6) {
      meanProjectionCut->Fit(straightLine,"Q+","",hitShapeCenter-nSigmaMaxNear*hitShapeSigma,hitShapeCenter-nSigmaMinNear*hitShapeSigma);
      fitIsDone = true;
      break;
    }
  }
  if( !fitIsDone ){
    for(int yBin = minBin-1; yBin>=1; --yBin){
      if(normProjection->GetBinContent(yBin)>=minNumberOfHits) ++pointsForFit;
      if(pointsForFit>=6) {
        meanProjectionCut->Fit(straightLine,"Q+","",normProjection->GetBinCenter(yBin),hitShapeCenter-nSigmaMinNear*hitShapeSigma);
        break;
      }
    }
  }
  slopes[slopeNegNear] = straightLine->GetParameter(1);
  
 
  straightLine->SetParameter(0,0.);
  straightLine->SetParameter(1,0.);
  pointsForFit = 0;
  fitIsDone = false;
  maxBin =  normProjection->FindBin(hitShapeCenter-nSigmaMinFar*hitShapeSigma);
  if (normProjection->GetBinCenter(maxBin)>hitShapeCenter-nSigmaMinFar*hitShapeSigma) --maxBin;
  for(int yBin = 1; yBin <= maxBin; ++yBin){
    if(normProjection->GetBinContent(yBin)>=minNumberOfHits) ++pointsForFit;
    if(pointsForFit>=6) {
      meanProjectionCut->Fit(straightLine,"Q+","",-30.,hitShapeCenter-nSigmaMinFar*hitShapeSigma);
      break;
    }
  }
  slopes[slopeNegFar] = straightLine->GetParameter(1);

  return slopes;

}


// //----------------------------------------------------------------------------------------------------

double deltaYvsY(double *xx, double *par){

  double x0 = xx[0]-par[0];
  
  if(par[1]>=0 && x0<=par[1]){
    TF1::RejectPoint();
    return 0.;
  }
  
  else if(par[1]<0 && x0>=par[1]){
    TF1::RejectPoint();
    return 0.;
  }

  else {
    return TMath::Log(par[2]*TMath::Abs(xx[0])+par[3])*par[5] + par[4];
  }

}

//----------------------------------------------------------------------------------------------------

double deltaXvsY(double *xx, double *par){

  double x0 = xx[0]-par[0];
  
  if(x0>-par[1] && x0<par[1] ){
    TF1::RejectPoint();
    return 0.;
  }

  else {
    return TMath::Log(par[2]*TMath::Abs(xx[0])+par[3])*par[5] + par[4];
  }


}

//----------------------------------------------------------------------------------------------------

double varianceYvsY(double *xx, double *par){

    return par[0]*TMath::Power(xx[0]-par[1],2)+par[2];

}

//----------------------------------------------------------------------------------------------------


class CTPPSTrackArmPlotter : public edm::one::EDAnalyzer<>
{
  public:
    explicit CTPPSTrackArmPlotter( const edm::ParameterSet& );
    ~CTPPSTrackArmPlotter();

  private:
    virtual void beginJob() override;

    virtual void analyze( const edm::Event&, const edm::EventSetup& ) override;

    virtual void endJob() override;

    edm::EDGetTokenT< std::vector<CTPPSLocalTrackLite> > tracksToken_;

    std::string outputFile;


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
      TH2D *h2_dXmean;
      TH2D *h2_dXmeanExtended;
      TH2D *h2_dXsigma;
      TH2D *h2_dXsigmaExtended;
      TH2D *h2_dXvariance;
      TH2D *h2_dYmean;
      TH2D *h2_dYmeanExtended;
      TH2D *h2_dYsigma;
      TH2D *h2_dYsigmaExtended;
      TH2D *h2_dYvariance;
      TH2D *h2_dXmeanSquare;
      TH2D *h2_dYmeanSquare;
      TH2D *h2_Normalization;
      TGraphErrors *g_hitShapeCenter;
      TGraphErrors *g_hitShapeSigma;
      double minNumberOfHits;
      

      void init()
      {
        h2_dXmean = new TH2D("", "", 300, -10., +50., 300, -30, +30.);
        h2_dXmeanExtended = new TH2D("", "", 300, -10., +50., 300, -30, +30.);
        h2_dXmeanSquare = new TH2D("", "", 300, -10., +50., 300, -30, +30.);
        h2_dYmean = new TH2D("", "", 300, -10., +50., 300, -30, +30.);
        h2_dYmeanExtended = new TH2D("", "", 300, -10., +50., 300, -30, +30.);
        h2_dYmeanSquare = new TH2D("", "", 300, -10., +50., 300, -30, +30.);
        h2_Normalization = new TH2D("", "", 300, -10., +50., 300, -30, +30.);
        h2_dXsigma = new TH2D("", "", 300, -10., +50., 300, -30, +30.);
        h2_dXsigmaExtended = new TH2D("", "", 300, -10., +50., 300, -30, +30.);
        h2_dYsigma = new TH2D("", "", 300, -10., +50., 300, -30, +30.);
        h2_dYsigmaExtended = new TH2D("", "", 300, -10., +50., 300, -30, +30.);
        h2_dXvariance = new TH2D("", "", 300, -10., +50., 300, -30, +30.);
        h2_dYvariance = new TH2D("", "", 300, -10., +50., 300, -30, +30.);
        g_hitShapeCenter = new TGraphErrors();
        g_hitShapeSigma = new TGraphErrors();
        minNumberOfHits=5;
        
      }

      void fill(double xRpNear, double yRpNear, double xRpFar, double yRpFar)
      {
        if (h2_dXmean == NULL || h2_dXmeanSquare == NULL || h2_dYmean == NULL || h2_dYmeanSquare == NULL)
          init();

        h2_dXmean->Fill(xRpNear,yRpNear,xRpFar-xRpNear);
        h2_dXmeanSquare->Fill(xRpNear,yRpNear,(xRpFar-xRpNear)*(xRpFar-xRpNear));
        h2_dYmean->Fill(xRpNear,yRpNear,yRpFar-yRpNear);
        h2_dYmeanSquare->Fill(xRpNear,yRpNear,(yRpFar-yRpNear)*(yRpFar-yRpNear));
        h2_Normalization->Fill(xRpNear,yRpNear);

      }

      void write() const
      {
        h2_dXmean->Divide(h2_Normalization);
        h2_dYmean->Divide(h2_Normalization);
        h2_dXmeanSquare->Divide(h2_Normalization);
        h2_dYmeanSquare->Divide(h2_Normalization);

        h2_dXvariance->Multiply(h2_dXmean,h2_dXmean,-1);
        h2_dXvariance->Add(h2_dXmeanSquare);

        h2_dYvariance->Multiply(h2_dYmean,h2_dYmean,-1);
        h2_dYvariance->Add(h2_dYmeanSquare);

        for(int xBin=1; xBin<=300; ++xBin){
          for(int yBin=1; yBin<=300; ++yBin){
            h2_dXsigma->SetBinContent(xBin,yBin,TMath::Sqrt(h2_dXmeanSquare->GetBinContent(xBin,yBin) - TMath::Power(h2_dXmean->GetBinContent(xBin,yBin),2)));
            h2_dYsigma->SetBinContent(xBin,yBin,TMath::Sqrt(h2_dYmeanSquare->GetBinContent(xBin,yBin) - TMath::Power(h2_dYmean->GetBinContent(xBin,yBin),2)));
          }
        }

        h2_dXmean->Write("h2_dXmean");
        h2_dXsigma->Write("h2_dXsigma");
        h2_dXvariance->Write("h2_dXvariance");
        h2_dYmean->Write("h2_dYmean");
        h2_dYsigma->Write("h2_dYsigma");
        h2_dYvariance->Write("h2_dYvariance");
        h2_Normalization->Write("h2_Normalization");

        int counter=0;
        for(int xBin=1; xBin<=300; ++xBin){
          TH1D *normProjection = h2_Normalization->ProjectionY(Form("normProjection_%i",xBin),xBin, xBin);
          if(normProjection->GetEntries()>=10){
            TF1 *fGaus = new TF1("fGaus","gaus",normProjection->GetMean()-normProjection->GetRMS(),normProjection->GetMean()+normProjection->GetRMS());
            normProjection->Fit(fGaus,"QNR");
            double binCenter = h2_Normalization->GetXaxis()->GetBinCenter(xBin);
            double binWidth = h2_Normalization->GetXaxis()->GetBinWidth(xBin);
            double hitShapeCenter = fGaus->GetParameter(1);
            double hitShapeCenterError = fGaus->GetParError(1);
            double hitShapeSigma = fGaus->GetParameter(2);
            double hitShapeSigmaError = fGaus->GetParError(2);

            g_hitShapeCenter->SetPoint(counter, binCenter, hitShapeCenter);
            g_hitShapeCenter->SetPointError(counter, 0., hitShapeCenterError);

            g_hitShapeSigma->SetPoint(counter, binCenter, hitShapeSigma);
            g_hitShapeSigma->SetPointError(counter, binWidth, hitShapeSigmaError);

            TH1D *xMeanProjection = h2_dXmean->ProjectionY(Form("xMeanProjection_%i",xBin),xBin, xBin);
            TH1D *yMeanProjection = h2_dYmean->ProjectionY(Form("yMeanProjection_%i",xBin),xBin, xBin);
            TH1D *xMeanProjectionCut = new TH1D(Form("xMeanProjectionCut_%i",xBin), Form("xMeanProjectionCut_%i",xBin), 300, -30, +30.);
            TH1D *yMeanProjectionCut = new TH1D(Form("yMeanProjectionCut_%i",xBin), Form("yMeanProjectionCut_%i",xBin), 300, -30, +30.);
            
            TH1D *xVarianceProjection = h2_dXvariance->ProjectionY(Form("xVarianceProjection_%i",xBin),xBin, xBin);
            TH1D *yVarianceProjection = h2_dYvariance->ProjectionY(Form("yVarianceProjection_%i",xBin),xBin, xBin);
            TH1D *xVarianceProjectionCut = new TH1D(Form("xVarianceProjectionCut_%i",xBin), Form("xVarianceProjectionCut_%i",xBin), 300, -30, +30.);
            TH1D *yVarianceProjectionCut = new TH1D(Form("yVarianceProjectionCut_%i",xBin), Form("yVarianceProjectionCut_%i",xBin), 300, -30, +30.);
            
            for(int yBin=1; yBin<=300; ++yBin){
              if(normProjection->GetBinContent(yBin)>=minNumberOfHits){
                xMeanProjectionCut->SetBinContent(yBin,xMeanProjection->GetBinContent(yBin));
                xMeanProjectionCut->SetBinError(yBin,xMeanProjection->GetBinError(yBin));
                yMeanProjectionCut->SetBinContent(yBin,yMeanProjection->GetBinContent(yBin));
                yMeanProjectionCut->SetBinError(yBin,yMeanProjection->GetBinError(yBin));

                xVarianceProjectionCut->SetBinContent(yBin,xVarianceProjection->GetBinContent(yBin));
                xVarianceProjectionCut->SetBinError(yBin,xVarianceProjection->GetBinError(yBin));
                yVarianceProjectionCut->SetBinContent(yBin,yVarianceProjection->GetBinContent(yBin));
                yVarianceProjectionCut->SetBinError(yBin,yVarianceProjection->GetBinError(yBin));
              }
            }

            double sigmaMultFactorDelta = 1.;

            // ----------------------- deltaXvsY fit ---------------------------------------------------------------------- //
            std::map<slope,double> xSlopes = getSlopes(normProjection, xMeanProjectionCut,hitShapeCenter,hitShapeSigma,minNumberOfHits,0.5,2.5,1.5);

            TF1 *fDeltaXvsYPos = new TF1("fDeltaXvsYPos",deltaXvsY,hitShapeCenter+sigmaMultFactorDelta*hitShapeSigma,30.,6);
            fDeltaXvsYPos->FixParameter(0,hitShapeCenter);
            fDeltaXvsYPos->FixParameter(1,hitShapeSigma);
            fDeltaXvsYPos->SetParLimits(2,0.,100.);
            fDeltaXvsYPos->SetParLimits(3,0.,100.);
            if(xSlopes[slopePosFar]>=0.) fDeltaXvsYPos->SetParLimits(5,0.,100.);
            else fDeltaXvsYPos->SetParLimits(5,-100.,0.);
            TFitResultPtr fitResultDeltaXvsYPos = xMeanProjectionCut->Fit(fDeltaXvsYPos,"QR+S");
            
            TF1 *fDeltaXvsYNeg = new TF1("fDeltaXvsYNeg",deltaXvsY,-30,hitShapeCenter-sigmaMultFactorDelta*hitShapeSigma,6);
            fDeltaXvsYNeg->FixParameter(0,hitShapeCenter);
            fDeltaXvsYNeg->FixParameter(1,hitShapeSigma);
            fDeltaXvsYNeg->SetParLimits(2,0.,100.);
            fDeltaXvsYNeg->SetParLimits(3,0.,100.);
            if(xSlopes[slopeNegFar]<=0.) fDeltaXvsYNeg->SetParLimits(5,0.,100.);
            else fDeltaXvsYNeg->SetParLimits(5,-100.,0.);
            TFitResultPtr fitResultDeltaXvsYNeg = xMeanProjectionCut->Fit(fDeltaXvsYNeg,"QR+S");
            xMeanProjectionCut->Write();

            // ----------------------- deltaYvsY fit ---------------------------------------------------------------------- //
 
            std::map<slope,double> ySlopes = getSlopes(normProjection, yMeanProjectionCut,hitShapeCenter,hitShapeSigma,minNumberOfHits,0.5,2.5,2.5);

            TF1 *fDeltaYvsYPos;
            if(ySlopes[slopePosNear]*ySlopes[slopePosFar]>=0.){
              fDeltaYvsYPos = new TF1("fDeltaYvsYPos",deltaYvsY,hitShapeCenter+hitShapeSigma*sigmaMultFactorDelta,30.,6);
              fDeltaYvsYPos->FixParameter(0,hitShapeCenter);
              fDeltaYvsYPos->FixParameter(1,hitShapeSigma*sigmaMultFactorDelta);
              fDeltaYvsYPos->SetParLimits(2, 0., 10000.);
              fDeltaYvsYPos->SetParLimits(3, 0., 10000.);
              if(ySlopes[slopePosNear]>=0.) fDeltaYvsYPos->SetParLimits(5, 0., 100.);
              else fDeltaYvsYPos->SetParLimits(5, -100., 0.);
            }
            else{
              fDeltaYvsYPos = new TF1("fDeltaYvsYPosPol2","pol2",hitShapeCenter+hitShapeSigma*sigmaMultFactorDelta,30.);
            }
            TFitResultPtr fitResultDeltaYvsYPos = yMeanProjectionCut->Fit(fDeltaYvsYPos,"QR+S");
           
            TF1 *fDeltaYvsYNeg;
            if(ySlopes[slopeNegNear]*ySlopes[slopeNegFar]>=0.){
              fDeltaYvsYNeg = new TF1("fDeltaYvsYNeg",deltaYvsY,-30.,hitShapeCenter-hitShapeSigma*sigmaMultFactorDelta,6);
              fDeltaYvsYNeg->FixParameter(0,hitShapeCenter);
              fDeltaYvsYNeg->FixParameter(1,-hitShapeSigma*sigmaMultFactorDelta);
              fDeltaYvsYNeg->SetParLimits(2, 0., 10000.);
              fDeltaYvsYNeg->SetParLimits(3, 0., 10000.);
              if(ySlopes[slopeNegNear]<=0.) fDeltaYvsYNeg->SetParLimits(5, 0., 100.);
              else fDeltaYvsYNeg->SetParLimits(5, -100., 0.);
            }
            else{
              fDeltaYvsYNeg = new TF1("fDeltaYvsYNegPol2","pol2",-30.,hitShapeCenter-hitShapeSigma*sigmaMultFactorDelta);
            }
            TFitResultPtr fitResultDeltaYvsYNeg = yMeanProjectionCut->Fit(fDeltaYvsYNeg,"QR+S");
            yMeanProjectionCut->Write();

            // ----------------------- varianceDeltaXvsY fit -------------------------------------------------------------- //

            double sigmaMultFactorVariance = 1.5;
            TF1 *fVarianceXvsY = new TF1("fVarianceXvsY","pol0", -30.,30.);
            TFitResultPtr fitResultVarianceXvsY;
            if(xVarianceProjectionCut->GetEntries()>=3) fitResultVarianceXvsY = xVarianceProjectionCut->Fit(fVarianceXvsY,"QR+S");
            xVarianceProjectionCut->Write();

            // ----------------------- fVarianceYvsY fit -------------------------------------------------------------- //

            TF1 *fVarianceYvsY = new TF1("fVarianceYvsY","pol2", hitShapeCenter-hitShapeSigma*sigmaMultFactorVariance,hitShapeCenter+hitShapeSigma*sigmaMultFactorVariance);
            TFitResultPtr fitResultVarianceYvsY;
            if(yVarianceProjectionCut->GetEntries()>=4){
              int pointsForFit = 0;
              bool fitIsDone = false;
              double minBin =  normProjection->FindBin(hitShapeCenter+sigmaMultFactorVariance*hitShapeSigma);
              if (normProjection->GetBinCenter(minBin)<hitShapeCenter+sigmaMultFactorVariance*hitShapeSigma) ++minBin;
              double maxBin =  normProjection->FindBin(hitShapeCenter+sigmaMultFactorVariance*hitShapeSigma);
              if (normProjection->GetBinCenter(maxBin)>hitShapeCenter+sigmaMultFactorVariance*hitShapeSigma) --maxBin;
              for(int yBin = minBin; yBin <= maxBin; ++yBin){
                if(normProjection->GetBinContent(yBin)>=minNumberOfHits) ++pointsForFit;
                if(pointsForFit>=4) {
                  fitResultVarianceYvsY = yVarianceProjectionCut->Fit(fVarianceYvsY,"Q+S","",hitShapeCenter+sigmaMultFactorVariance*hitShapeSigma,hitShapeCenter+sigmaMultFactorVariance*hitShapeSigma);
                  fitIsDone = true;
                  break;
                }
              }
              if( !fitIsDone ){
                for(int plusBin = 1; plusBin<=300; ++plusBin){
                  if(normProjection->GetBinContent(minBin-plusBin)>=minNumberOfHits) ++pointsForFit;
                  if(normProjection->GetBinContent(maxBin+plusBin)>=minNumberOfHits) ++pointsForFit;
                  if(pointsForFit>=4) {
                    fitResultVarianceYvsY = yVarianceProjectionCut->Fit(fVarianceYvsY,"Q+S","",normProjection->GetBinCenter(minBin-plusBin),normProjection->GetBinCenter(maxBin+plusBin));
                    break;
                  }
                }
              }
            }
            yVarianceProjectionCut->Write();

            // ----------------------- filling extended maps -------------------------------------------------------------- //

            for(int yBin = 1; yBin <= 300; ++yBin){
              if(normProjection->GetBinContent(yBin)<minNumberOfHits){
                double binCenter = normProjection->GetBinCenter(yBin);
                if(binCenter>=hitShapeCenter){
                  double dXmean = fDeltaXvsYPos->Eval(binCenter);
                  double dYmean = fDeltaYvsYPos->Eval(binCenter);

                  if(!isnan(dXmean) && !isinf(dXmean) && dXmean!=0.){
                    h2_dXmeanExtended->SetBinContent(xBin,yBin,dXmean);
                    double xVal[1] = {binCenter};
                    double err[1] = {999.};
                    fitResultDeltaXvsYPos->GetConfidenceIntervals(1, 1, 1, xVal, err, 0.683, false);
                    h2_dXmeanExtended-> SetBinError(xBin,yBin,err[0]);
                  }

                  if(!isnan(dYmean) && !isinf(dYmean) && dYmean!=0.){
                    h2_dYmeanExtended->SetBinContent(xBin,yBin,dYmean);
                    double xVal[1] = {binCenter};
                    double err[1] = {999.};
                    fitResultDeltaYvsYPos->GetConfidenceIntervals(1, 1, 1, xVal, err, 0.683, false);
                    h2_dYmeanExtended-> SetBinError(xBin,yBin,err[0]);
                  }

                }
                else{
                // else if((int)(fitResultDeltaYvsYNeg)==0 && fDeltaYvsYNeg!=NULL){
                  double dXmean = fDeltaXvsYNeg->Eval(binCenter);
                  double dYmean = fDeltaYvsYNeg->Eval(binCenter);


                  if(!isnan(dXmean) && !isinf(dXmean) && dXmean!=0.){
                    h2_dXmeanExtended->SetBinContent(xBin,yBin,dXmean);
                    double xVal[1] = {binCenter};
                    double err[1] = {999.};
                    fitResultDeltaXvsYNeg->GetConfidenceIntervals(1, 1, 1, xVal, err, 0.683, false);
                    h2_dXmeanExtended->SetBinError(xBin,yBin,err[0]);
                  }


                  if(!isnan(dYmean) && !isinf(dYmean) && dYmean!=0.){
                    h2_dYmeanExtended->SetBinContent(xBin,yBin,dYmean);
                    double xVal[1] = {binCenter};
                    double err[1] = {999.};
                    fitResultDeltaYvsYNeg->GetConfidenceIntervals(1, 1, 1, xVal, err, 0.683, false);
                    h2_dYmeanExtended->SetBinError(xBin,yBin,err[0]);
                  }
                }

                double dXsigma = TMath::Sqrt(fVarianceXvsY->Eval(binCenter));
                double dYsigma = TMath::Sqrt(fVarianceYvsY->Eval(binCenter));

                if(!isnan(dXsigma) && !isinf(dXsigma) && dXsigma!=0.){
                  h2_dXsigmaExtended->SetBinContent(xBin,yBin,dXsigma);
                  double xVal[1] = {binCenter};
                  double err[1] = {999.};
                  fitResultVarianceXvsY->GetConfidenceIntervals(1, 1, 1, xVal, err, 0.683, false);
                  h2_dXsigmaExtended->SetBinError(xBin,yBin,err[0]/(2.*dXsigma));
                }
                
                if(!isnan(dYsigma) && !isinf(dYsigma) && dYsigma!=0.){
                  h2_dYsigmaExtended->SetBinContent(xBin,yBin,dYsigma);
                  double xVal[1] = {binCenter};
                  double err[1] = {999.};
                  fitResultVarianceYvsY->GetConfidenceIntervals(1, 1, 1, xVal, err, 0.683, false);
                  h2_dYsigmaExtended->SetBinError(xBin,yBin,err[0]/(2.*dYsigma));
                }            

              }
              else{
                h2_dXmeanExtended->SetBinContent(xBin,yBin,h2_dXmean->GetBinContent(xBin,yBin));
                h2_dXmeanExtended->SetBinError(xBin,yBin,h2_dXmean->GetBinError(xBin,yBin));
                h2_dYmeanExtended->SetBinContent(xBin,yBin,h2_dYmean->GetBinContent(xBin,yBin));
                h2_dYmeanExtended->SetBinError(xBin,yBin,h2_dYmean->GetBinError(xBin,yBin));

                h2_dXsigmaExtended->SetBinContent(xBin,yBin,h2_dXsigma->GetBinContent(xBin,yBin));
                h2_dXsigmaExtended->SetBinError(xBin,yBin,h2_dXsigma->GetBinError(xBin,yBin));
                h2_dYsigmaExtended->SetBinContent(xBin,yBin,h2_dYsigma->GetBinContent(xBin,yBin));
                h2_dYsigmaExtended->SetBinError(xBin,yBin,h2_dYsigma->GetBinError(xBin,yBin));                
                
              }
            }

            ++counter;
          }

        }

        g_hitShapeCenter->Write("g_hitShapeCenter");
        g_hitShapeSigma->Write("g_hitShapeSigma");

        h2_dXmeanExtended->SetMinimum(-3.);
        h2_dXmeanExtended->SetMaximum(1.);
        h2_dXmeanExtended->Write("h2_dXmeanExtended");
        TH2D *h2_dXmeanExtendedSmooth1 = (TH2D*)h2_dXmeanExtended->Clone("h2_dXmeanExtendedSmooth1");
        h2_dXmeanExtendedSmooth1->Smooth(1,"k3a");
        h2_dXmeanExtendedSmooth1->SetMinimum(-3.);
        h2_dXmeanExtendedSmooth1->SetMaximum(1.);
        h2_dXmeanExtendedSmooth1->Write();
        TH2D *h2_dXmeanExtendedSmooth1Norm = (TH2D*)h2_dXmeanExtendedSmooth1->Clone("h2_dXmeanExtendedSmooth1Norm");
        h2_dXmeanExtendedSmooth1Norm->Add(h2_dXmean,-1.);
        h2_dXmeanExtendedSmooth1Norm->SetMinimum(-1.);
        h2_dXmeanExtendedSmooth1Norm->SetMaximum(+1.);
        h2_dXmeanExtendedSmooth1Norm->Write();
        TH2D *h2_dXmeanExtendedSmooth2 = (TH2D*)h2_dXmeanExtendedSmooth1->Clone("h2_dXmeanExtendedSmooth2");
        h2_dXmeanExtendedSmooth2->Smooth(1,"k3a");
        h2_dXmeanExtendedSmooth2->SetMinimum(-3.);
        h2_dXmeanExtendedSmooth2->SetMaximum(1.);
        h2_dXmeanExtendedSmooth2->Write();
        TH2D *h2_dXmeanExtendedSmooth2Norm = (TH2D*)h2_dXmeanExtendedSmooth2->Clone("h2_dXmeanExtendedSmooth2Norm");
        h2_dXmeanExtendedSmooth2Norm->Add(h2_dXmean,-1.);
        h2_dXmeanExtendedSmooth2Norm->SetMinimum(-1.);
        h2_dXmeanExtendedSmooth2Norm->SetMaximum(+1.);
        h2_dXmeanExtendedSmooth2Norm->Write();
        TH2D *h2_dXmeanExtendedSmooth3 = (TH2D*)h2_dXmeanExtendedSmooth2->Clone("h2_dXmeanExtendedSmooth3");
        h2_dXmeanExtendedSmooth3->Smooth(1,"k3a");
        h2_dXmeanExtendedSmooth3->SetMinimum(-3.);
        h2_dXmeanExtendedSmooth3->SetMaximum(1.);
        h2_dXmeanExtendedSmooth3->Write();
        TH2D *h2_dXmeanExtendedSmooth3Norm = (TH2D*)h2_dXmeanExtendedSmooth3->Clone("h2_dXmeanExtendedSmooth3Norm");
        h2_dXmeanExtendedSmooth3Norm->Add(h2_dXmean,-1.);
        h2_dXmeanExtendedSmooth3Norm->SetMinimum(-1.);
        h2_dXmeanExtendedSmooth3Norm->SetMaximum(+1.);
        h2_dXmeanExtendedSmooth3Norm->Write();

        h2_dYmeanExtended->SetMinimum(-3.);
        h2_dYmeanExtended->SetMaximum(3.);
        h2_dYmeanExtended->Write("h2_dYmeanExtended");
        TH2D *h2_dYmeanExtendedSmooth1 = (TH2D*)h2_dYmeanExtended->Clone("h2_dYmeanExtendedSmooth1");
        h2_dYmeanExtendedSmooth1->Smooth(1,"k3a");
        h2_dYmeanExtendedSmooth1->SetMinimum(-3.);
        h2_dYmeanExtendedSmooth1->SetMaximum(3.);
        h2_dYmeanExtendedSmooth1->Write();
        TH2D *h2_dYmeanExtendedSmooth1Norm = (TH2D*)h2_dYmeanExtendedSmooth1->Clone("h2_dYmeanExtendedSmooth1Norm");
        h2_dYmeanExtendedSmooth1Norm->Add(h2_dYmean,-1.);
        h2_dYmeanExtendedSmooth1Norm->SetMinimum(-1.);
        h2_dYmeanExtendedSmooth1Norm->SetMaximum(+1.);
        h2_dYmeanExtendedSmooth1Norm->Write();
        TH2D *h2_dYmeanExtendedSmooth2 = (TH2D*)h2_dYmeanExtendedSmooth1->Clone("h2_dYmeanExtendedSmooth2");
        h2_dYmeanExtendedSmooth2->Smooth(1,"k3a");
        h2_dYmeanExtendedSmooth2->SetMinimum(-3.);
        h2_dYmeanExtendedSmooth2->SetMaximum(3.);
        h2_dYmeanExtendedSmooth2->Write();
        TH2D *h2_dYmeanExtendedSmooth2Norm = (TH2D*)h2_dYmeanExtendedSmooth2->Clone("h2_dYmeanExtendedSmooth2Norm");
        h2_dYmeanExtendedSmooth2Norm->Add(h2_dYmean,-1.);
        h2_dYmeanExtendedSmooth2Norm->SetMinimum(-1.);
        h2_dYmeanExtendedSmooth2Norm->SetMaximum(+1.);
        h2_dYmeanExtendedSmooth2Norm->Write();
        TH2D *h2_dYmeanExtendedSmooth3 = (TH2D*)h2_dYmeanExtendedSmooth2->Clone("h2_dYmeanExtendedSmooth3");
        h2_dYmeanExtendedSmooth3->Smooth(1,"k3a");
        h2_dYmeanExtendedSmooth3->SetMinimum(-3.);
        h2_dYmeanExtendedSmooth3->SetMaximum(3.);
        h2_dYmeanExtendedSmooth3->Write();
        TH2D *h2_dYmeanExtendedSmooth3Norm = (TH2D*)h2_dYmeanExtendedSmooth3->Clone("h2_dYmeanExtendedSmooth3Norm");
        h2_dYmeanExtendedSmooth3Norm->Add(h2_dYmean,-1.);
        h2_dYmeanExtendedSmooth3Norm->SetMinimum(-1.);
        h2_dYmeanExtendedSmooth3Norm->SetMaximum(+1.);
        h2_dYmeanExtendedSmooth3Norm->Write();

        h2_dXsigmaExtended->SetMinimum(0.);
        h2_dXsigmaExtended->SetMaximum(1.);
        h2_dXsigmaExtended->Write("h2_dXsigmaExtended");
        TH2D *h2_dXsigmaExtendedSmooth1 = (TH2D*)h2_dXsigmaExtended->Clone("h2_dXsigmaExtendedSmooth1");
        h2_dXsigmaExtendedSmooth1->Smooth(1,"k3a");
        h2_dXsigmaExtendedSmooth1->SetMinimum(0.);
        h2_dXsigmaExtendedSmooth1->SetMaximum(1.);
        h2_dXsigmaExtendedSmooth1->Write();
        TH2D *h2_dXsigmaExtendedSmooth1Norm = (TH2D*)h2_dXsigmaExtendedSmooth1->Clone("h2_dXsigmaExtendedSmooth1Norm");
        h2_dXsigmaExtendedSmooth1Norm->Add(h2_dXsigma,-1.);
        h2_dXsigmaExtendedSmooth1Norm->SetMinimum(-0.3);
        h2_dXsigmaExtendedSmooth1Norm->SetMaximum(+0.3);
        h2_dXsigmaExtendedSmooth1Norm->Write();
        TH2D *h2_dXsigmaExtendedSmooth2 = (TH2D*)h2_dXsigmaExtendedSmooth1->Clone("h2_dXsigmaExtendedSmooth2");
        h2_dXsigmaExtendedSmooth2->Smooth(1,"k3a");
        h2_dXsigmaExtendedSmooth2->SetMinimum(0.);
        h2_dXsigmaExtendedSmooth2->SetMaximum(1.);
        h2_dXsigmaExtendedSmooth2->Write();
        TH2D *h2_dXsigmaExtendedSmooth2Norm = (TH2D*)h2_dXsigmaExtendedSmooth2->Clone("h2_dXsigmaExtendedSmooth2Norm");
        h2_dXsigmaExtendedSmooth2Norm->Add(h2_dXsigma,-1.);
        h2_dXsigmaExtendedSmooth2Norm->SetMinimum(-0.3);
        h2_dXsigmaExtendedSmooth2Norm->SetMaximum(+0.3);
        h2_dXsigmaExtendedSmooth2Norm->Write();
        TH2D *h2_dXsigmaExtendedSmooth3 = (TH2D*)h2_dXsigmaExtendedSmooth2->Clone("h2_dXsigmaExtendedSmooth3");
        h2_dXsigmaExtendedSmooth3->Smooth(1,"k3a");
        h2_dXsigmaExtendedSmooth3->SetMinimum(0.);
        h2_dXsigmaExtendedSmooth3->SetMaximum(1.);
        h2_dXsigmaExtendedSmooth3->Write();
        TH2D *h2_dXsigmaExtendedSmooth3Norm = (TH2D*)h2_dXsigmaExtendedSmooth3->Clone("h2_dXsigmaExtendedSmooth3Norm");
        h2_dXsigmaExtendedSmooth3Norm->Add(h2_dXsigma,-1.);
        h2_dXsigmaExtendedSmooth3Norm->SetMinimum(-0.3);
        h2_dXsigmaExtendedSmooth3Norm->SetMaximum(+0.3);
        h2_dXsigmaExtendedSmooth3Norm->Write();

        h2_dYsigmaExtended->SetMinimum(0.);
        h2_dYsigmaExtended->SetMaximum(2.);
        h2_dYsigmaExtended->Write("h2_dYsigmaExtended");
        TH2D *h2_dYsigmaExtendedSmooth1 = (TH2D*)h2_dYsigmaExtended->Clone("h2_dYsigmaExtendedSmooth1");
        h2_dYsigmaExtendedSmooth1->Smooth(1,"k3a");
        h2_dYsigmaExtendedSmooth1->SetMinimum(0.);
        h2_dYsigmaExtendedSmooth1->SetMaximum(2.);
        h2_dYsigmaExtendedSmooth1->Write();
        TH2D *h2_dYsigmaExtendedSmooth1Norm = (TH2D*)h2_dYsigmaExtendedSmooth1->Clone("h2_dYsigmaExtendedSmooth1Norm");
        h2_dYsigmaExtendedSmooth1Norm->Add(h2_dYsigma,-1.);
        h2_dYsigmaExtendedSmooth1Norm->SetMinimum(-0.3);
        h2_dYsigmaExtendedSmooth1Norm->SetMaximum(+0.3);
        h2_dYsigmaExtendedSmooth1Norm->Write();
        TH2D *h2_dYsigmaExtendedSmooth2 = (TH2D*)h2_dYsigmaExtendedSmooth1->Clone("h2_dYsigmaExtendedSmooth2");
        h2_dYsigmaExtendedSmooth2->Smooth(1,"k3a");
        h2_dYsigmaExtendedSmooth2->SetMinimum(0.);
        h2_dYsigmaExtendedSmooth2->SetMaximum(2.);
        h2_dYsigmaExtendedSmooth2->Write();
        TH2D *h2_dYsigmaExtendedSmooth2Norm = (TH2D*)h2_dYsigmaExtendedSmooth2->Clone("h2_dYsigmaExtendedSmooth2Norm");
        h2_dYsigmaExtendedSmooth2Norm->Add(h2_dYsigma,-1.);
        h2_dYsigmaExtendedSmooth2Norm->SetMinimum(-0.3);
        h2_dYsigmaExtendedSmooth2Norm->SetMaximum(+0.3);
        h2_dYsigmaExtendedSmooth2Norm->Write();
        TH2D *h2_dYsigmaExtendedSmooth3 = (TH2D*)h2_dYsigmaExtendedSmooth2->Clone("h2_dYsigmaExtendedSmooth3");
        h2_dYsigmaExtendedSmooth3->Smooth(1,"k3a");
        h2_dYsigmaExtendedSmooth3->SetMinimum(0.);
        h2_dYsigmaExtendedSmooth3->SetMaximum(2.);
        h2_dYsigmaExtendedSmooth3->Write();
        TH2D *h2_dYsigmaExtendedSmooth3Norm = (TH2D*)h2_dYsigmaExtendedSmooth3->Clone("h2_dYsigmaExtendedSmooth3Norm");
        h2_dYsigmaExtendedSmooth3Norm->Add(h2_dYsigma,-1.);
        h2_dYsigmaExtendedSmooth3Norm->SetMinimum(-0.3);
        h2_dYsigmaExtendedSmooth3Norm->SetMaximum(+0.3);
        h2_dYsigmaExtendedSmooth3Norm->Write();        

      }
    };

    std::map<unsigned int, ArmPlots> armPlots;

};

//----------------------------------------------------------------------------------------------------

CTPPSTrackArmPlotter::CTPPSTrackArmPlotter( const edm::ParameterSet& iConfig ) :
  tracksToken_( consumes< std::vector<CTPPSLocalTrackLite> >( iConfig.getParameter<edm::InputTag>( "tracksTag" ) ) ),
  outputFile( iConfig.getParameter<std::string>("outputFile") )

{
}

//----------------------------------------------------------------------------------------------------

CTPPSTrackArmPlotter::~CTPPSTrackArmPlotter()
{
}

//----------------------------------------------------------------------------------------------------

void CTPPSTrackArmPlotter::analyze( const edm::Event& iEvent, const edm::EventSetup& )
{
  // get input
  edm::Handle< std::vector<CTPPSLocalTrackLite> > tracks;
  iEvent.getByToken( tracksToken_, tracks );

  // process tracks
  std::map<unsigned int, CTPPSLocalTrackLite> tracksForMatching;
  for (const auto& trk : *tracks)
  {
    CTPPSDetId rpId(trk.getRPId());
    unsigned int rpDecId = rpId.arm()*100 + rpId.station()*10 + rpId.rp();
    rpPlots[rpDecId].fill(trk.getX(), trk.getY());

    tracksForMatching[rpDecId] = trk;
  }


  if(tracksForMatching.find(  3)!=tracksForMatching.end() && tracksForMatching.find( 23)!=tracksForMatching.end()) armPlots[0].fill(tracksForMatching[  3].getX(),tracksForMatching[  3].getY(),tracksForMatching[ 23].getX(),tracksForMatching[ 23].getY());
  if(tracksForMatching.find(103)!=tracksForMatching.end() && tracksForMatching.find(123)!=tracksForMatching.end()) armPlots[1].fill(tracksForMatching[103].getX(),tracksForMatching[103].getY(),tracksForMatching[123].getX(),tracksForMatching[123].getY());

}

//----------------------------------------------------------------------------------------------------

void CTPPSTrackArmPlotter::beginJob()
{
}

//----------------------------------------------------------------------------------------------------

void CTPPSTrackArmPlotter::endJob()
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

DEFINE_FWK_MODULE( CTPPSTrackArmPlotter );
