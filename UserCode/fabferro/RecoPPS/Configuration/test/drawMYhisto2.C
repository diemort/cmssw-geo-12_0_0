{
  TFile *_file0 = TFile::Open("DQM_V0001_R000382027__Global__CMSSW_X_Y_Z__RECO.root");
  TH2F *intercept45210frHor;
  TH2F *intercept45210frVT;
  TH2F *intercept45210frBT;
  TH2F *intercept45220frHor;
  TH2F *intercept45220frVT;
  TH2F *intercept45220frBT;
 
  TH2F *hitsDia45220nrHor;
  TH2F *hitsDia45220Cyl;
  
  _file0->cd("DQMData/Run 382027/CTPPS/Run summary/TrackingStrip/sector 45/station 210/fr_tp");
  gDirectory->GetObject("track XY profile",intercept45210frVT);
  _file0->cd("DQMData/Run 382027/CTPPS/Run summary/TrackingStrip/sector 45/station 210/fr_bt");
    gDirectory->GetObject("track XY profile",intercept45210frBT);
  _file0->cd("DQMData/Run 382027/CTPPS/Run summary/TrackingPixel/sector 45/station 210/fr_hr");
  gDirectory->GetObject("track intercept point",intercept45210frHor);

  _file0->cd("DQMData/Run 382027/CTPPS/Run summary/TrackingStrip/sector 45/station 220/fr_tp");
  gDirectory->GetObject("track XY profile",intercept45220frVT);
  _file0->cd("DQMData/Run 382027/CTPPS/Run summary/TrackingStrip/sector 45/station 220/fr_bt");
    gDirectory->GetObject("track XY profile",intercept45220frBT);
  _file0->cd("DQMData/Run 382027/CTPPS/Run summary/TrackingPixel/sector 45/station 220/fr_hr");
  gDirectory->GetObject("track intercept point",intercept45220frHor);

  _file0->cd("DQMData/Run 382027/CTPPS/Run summary/TimingDiamond/sector 45/station 220/nr_hr");
  gDirectory->GetObject("hits 2D",hitsDia45220nrHor);
  _file0->cd("DQMData/Run 382027/CTPPS/Run summary/TimingDiamond/sector 45/station 220cyl/cyl_hr");
  gDirectory->GetObject("hits 2D",hitsDia45220Cyl);


  TCanvas * c1 = new TCanvas("c45", "c45", 1500, 1000);
  c1->Divide(4,3);
  c1->cd(3);
  intercept45210frVT->Draw();
  c1->cd(7);
  intercept45210frHor->Draw();
  c1->cd(11);
  intercept45210frBT->Draw();
  c1->cd(4);
  intercept45220frVT->Draw();
  c1->cd(8);
  intercept45220frHor->Draw();
  c1->cd(12);
  intercept45220frBT->Draw();
  c1->cd(5);
  hitsDia45220nrHor->Draw("Box");
  c1->cd(6);
  hitsDia45220Cyl->Draw("Box");
 

  TH2F *intercept56210frHor;
  TH2F *intercept56210frVT;
  TH2F *intercept56210frBT;
  TH2F *intercept56220frHor;
  TH2F *intercept56220frVT;
  TH2F *intercept56220frBT;
 
  TH2F *hitsDia56220nrHor;
  TH2F *hitsDia56220Cyl;
  
  _file0->cd("DQMData/Run 382027/CTPPS/Run summary/TrackingStrip/sector 56/station 210/fr_tp");
  gDirectory->GetObject("track XY profile",intercept56210frVT);
  _file0->cd("DQMData/Run 382027/CTPPS/Run summary/TrackingStrip/sector 56/station 210/fr_bt");
    gDirectory->GetObject("track XY profile",intercept56210frBT);
  _file0->cd("DQMData/Run 382027/CTPPS/Run summary/TrackingPixel/sector 56/station 210/fr_hr");
  gDirectory->GetObject("track intercept point",intercept56210frHor);

  _file0->cd("DQMData/Run 382027/CTPPS/Run summary/TrackingStrip/sector 56/station 220/fr_tp");
  gDirectory->GetObject("track XY profile",intercept56220frVT);
  _file0->cd("DQMData/Run 382027/CTPPS/Run summary/TrackingStrip/sector 56/station 220/fr_bt");
    gDirectory->GetObject("track XY profile",intercept56220frBT);
  _file0->cd("DQMData/Run 382027/CTPPS/Run summary/TrackingPixel/sector 56/station 220/fr_hr");
  gDirectory->GetObject("track intercept point",intercept56220frHor);

  _file0->cd("DQMData/Run 382027/CTPPS/Run summary/TimingDiamond/sector 56/station 220/nr_hr");
  gDirectory->GetObject("hits 2D",hitsDia56220nrHor);
  _file0->cd("DQMData/Run 382027/CTPPS/Run summary/TimingDiamond/sector 56/station 220cyl/cyl_hr");
  gDirectory->GetObject("hits 2D",hitsDia56220Cyl);

  TCanvas * c2 = new TCanvas("c56", "c56", 1500, 1000);
  c2->Divide(4,3);
  c2->cd(3);
  intercept56210frVT->Draw();
  c2->cd(7);
  intercept56210frHor->Draw();
  c2->cd(11);
  intercept56210frBT->Draw();
  c2->cd(4);
  intercept56220frVT->Draw();
  c2->cd(8);
  intercept56220frHor->Draw();
  c2->cd(12);
  intercept56220frBT->Draw();
  c2->cd(5);
  hitsDia56220nrHor->Draw("Box");
  c2->cd(6);
  hitsDia56220Cyl->Draw("Box");



  
}
