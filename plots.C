//C, C++
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include <time.h>

using namespace std;

TString getObjectName(TString namein, Int_t i);

Int_t plots(){
  //
  TString fileN;
  TString objectName;
  //
  fileN = "./trajectories.root";
  //
  TFile *f1 = new TFile(fileN.Data());
  //
  Int_t nObj = 3;
  const Int_t nChannels = 10;
  TGraph *gr_Arr[nChannels];
  Int_t i = 0;

  const Int_t colorArr[nChannels] = {    kBlack,     kRed,  kGreen + 3, kCyan - 3, kBlue + 1,
				        kGray+1, kMagenta, kYellow + 2,  kRed - 2, kBlue - 6};

  const Int_t markerArr[nChannels] = { 20, 20, 20, 20, 20,
				       21, 21, 21, 21, 21};

  TGraph *gr_frame = (TGraph*)f1->Get("gr_frame");
  gr_frame->SetMarkerStyle(25);
  for(i = 0;i<nObj;i++){
    objectName=getObjectName("gr_tr_",i);
    cout<<objectName<<endl;
    gr_Arr[i] = (TGraph*)f1->Get(objectName.Data());
  }
  
  TCanvas *c1 = new TCanvas("c1",fileN.Data(),10,10,800,800);
  //TCanvas *c1 = new TCanvas("c1",fileN.Data(),10,10,600,600);
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetStatColor(kWhite);
  //gStyle->SetOptStat(kFALSE);
 

  TMultiGraph *mg = new TMultiGraph();  
  mg->Add(gr_frame);
  for(i = 0;i<nObj;i++){
    gr_Arr[i]->SetLineColor(colorArr[i]);
    gr_Arr[i]->SetLineWidth(3.0);
    gr_Arr[i]->SetMarkerColor(colorArr[i]);
    gr_Arr[i]->SetMarkerStyle(markerArr[i]);
    mg->Add(gr_Arr[i]);
  }

  mg->Draw("ap");  
  mg->GetXaxis()->SetTitle("x, km");
  mg->GetYaxis()->SetTitle("y, km");

  /*
  TString legInfo;
  TLegend *leg = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
  for(i = 0;i<nChannels;i++){
    legInfo = "ch ";legInfo += i;
    leg->AddEntry(gr_Arr[i], legInfo.Data(), "apl");
  }
  leg->Draw();
  */  

  return 0;
}

TString getObjectName(TString namein, Int_t i){
  TString h1nameh = namein; h1nameh += i;
  return h1nameh;
}
