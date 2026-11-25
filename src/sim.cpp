//my
#include "sim.hh"
#include "simConfStr.hh"

//root
#include "TROOT.h"
#include "TStyle.h"
#include "TRandom3.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TMath.h"
#include "TH1D.h"
#include "TVector2.h"
#include "TFile.h"
#include "TMultiGraph.h"

//C, C++
#include <iostream>
#include <vector>
#include <fstream>
#include <assert.h>
#include <iomanip>
#include <bits/stdc++.h>
#include <sys/stat.h>

sim::sim(){
  _rnd = new TRandom3();
  _conf = new simConfStr();
}

sim::sim( TRandom3 *rnd, simConfStr *conf){
  _rnd = rnd;
  _conf = conf;
}

sim::~sim(){
  delete _rnd;
  delete _conf;
}

void sim::runsim(Int_t nsteps){
  //
}

void sim::save_graph(TString outputRootFileWith_graph){
  //
  //
  /*
  Double_t xl = _conf->frame_x0 - _conf->frame_L/2.0;
  Double_t xr = _conf->frame_x0 + _conf->frame_L/2.0;
  Double_t yb = _conf->frame_y0 - _conf->frame_H/2.0;
  Double_t yt = _conf->frame_y0 + _conf->frame_H/2.0;  
  //Double_t v_mod;
  ////////////////
  TFile* rootFile = new TFile(outputRootFileWith_graph.Data(), "RECREATE", " Histograms", 1);
  rootFile->cd();
  if (rootFile->IsZombie()){
    std::cout<<"  ERROR ---> file "<<outputRootFileWith_graph.Data()<<" is zombi"<<std::endl;
    assert(0);
  }
  else {
    std::cout<<"  Output file ---> "<<outputRootFileWith_graph.Data()<<std::endl;
  }
  _gr_frame = new TGraph();
  _gr_frame->SetTitle("gr_frame");
  _gr_frame->SetName("gr_frame"); 
  _gr_frame->SetPoint(_gr_frame->GetN(),xl,yb);
  _gr_frame->SetPoint(_gr_frame->GetN(),xl,yt);
  _gr_frame->SetPoint(_gr_frame->GetN(),xr,yt);
  _gr_frame->SetPoint(_gr_frame->GetN(),xr,yb);
  _gr_frame->SetPoint(_gr_frame->GetN(),xl,yb);
  ////////////////
  for(unsigned int i = 0; i<_bodyState_v.size(); i++){
    TGraph *gr_tr = new TGraph();
    TString gr_tr_name_title = "gr_tr_";
    gr_tr_name_title += i;
    gr_tr->SetTitle(gr_tr_name_title.Data());
    gr_tr->SetName(gr_tr_name_title.Data());
    //
    TGraph *gr_v = new TGraph();
    TString gr_v_name_title = "gr_v_";
    gr_v_name_title += i;
    gr_v->SetTitle(gr_v_name_title.Data());
    gr_v->SetName(gr_v_name_title.Data());
    //
    for(unsigned int j = 0; j<_bodyState_v.at(i).x.size(); j++){
      if(_bodyState_v.at(i).x.at(j)>=xl && _bodyState_v.at(i).x.at(j)<=xr){
	if(_bodyState_v.at(i).y.at(j)>=yb && _bodyState_v.at(i).y.at(j)<=yt){
	  gr_tr->SetPoint(gr_tr->GetN(),_bodyState_v.at(i).x.at(j),_bodyState_v.at(i).y.at(j));
	  gr_v->SetPoint(gr_tr->GetN(),_bodyState_v.at(i).vx.at(j),_bodyState_v.at(i).vy.at(j));
	  //v_mod = TMath::Sqrt(_bodyState_v.at(i).vx.at(j)*_bodyState_v.at(i).vx.at(j) + _bodyState_v.at(i).vy.at(j)*_bodyState_v.at(i).vy.at(j));
	  //gr_v->SetPoint(gr_tr->GetN(),gr_tr->GetN(),v_mod);
	  //std::cout<<"_bodyState_v.at(i).y.at(j) = "<<_bodyState_v.at(i).y.at(j)<<std::endl;
	}
      }
    }
    gr_tr->Write();
    gr_v->Write();
  }
  _gr_frame->Write();
  rootFile->Close();
  */
}

void sim::save_gif(TString output_gif, Int_t reduction_f){
  /*
  mkdir(output_gif.Data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  //
    //
  Double_t xl = _conf->frame_x0 - _conf->frame_L/2.0;
  Double_t xr = _conf->frame_x0 + _conf->frame_L/2.0;
  Double_t yb = _conf->frame_y0 - _conf->frame_H/2.0;
  Double_t yt = _conf->frame_y0 + _conf->frame_H/2.0;  
  //
  TString merge_gif_name=output_gif;
  merge_gif_name += "/mrg.sh";
  std::ofstream merge_gif;
  merge_gif.open(merge_gif_name.Data());
  //
  //merge_gif<<"convert -delay 1 -loop 1000 ";
  merge_gif<<"gifsicle --delay 1 --loopcount ";
  //
  std::vector<TString> gif_name_short_v;    
  //
  TString out_gif_pdf_file="";
  TString gif_name_short="";
  Int_t file_counter = 0;
  for(unsigned int i = 0; i<_bodyState_v.at(0).x.size(); i++){
    if(i%((unsigned int)reduction_f)==0){
      TGraph *gr = new TGraph();
      gr->SetTitle("");
      gr->SetName("");
      for( unsigned int j = 0; j<_bodyState_v.size(); j++)
	if(_bodyState_v.at(j).x.at(i)>=xl && _bodyState_v.at(j).x.at(i)<=xr)
	  if(_bodyState_v.at(j).y.at(i)>=yb && _bodyState_v.at(j).y.at(i)<=yt)
	    gr->SetPoint( gr->GetN(), _bodyState_v.at(j).x.at(i), _bodyState_v.at(j).y.at(i));
      //
      //gStyle->SetPalette(kRainBow);
      //gStyle->SetPalette(kCool);
      //gStyle->SetPalette(kIsland);
      //gStyle->SetPalette(kCherry);
      //TColor::InvertPalette();
      gStyle->SetPalette(kInvertedDarkBodyRadiator);
      gStyle->SetOptStat(kFALSE);
      TCanvas *c1 = new TCanvas("c1","c1",800,800);
      gPad->SetRightMargin(0.12);
      gPad->SetLeftMargin(0.12);
      gPad->SetTopMargin(0.1);
      gPad->SetBottomMargin(0.15);
      TMultiGraph *mg = new TMultiGraph();  
      mg->SetTitle("");
      mg->SetName("");
      _gr_frame->SetMarkerStyle(25);
      gr->SetMarkerStyle(20);
      mg->Add(_gr_frame);
      mg->Add(gr);
      //
      mg->Draw("ap");  
      mg->GetXaxis()->SetTitle("x, km");
      mg->GetYaxis()->SetTitle("y, km");
      //
      out_gif_pdf_file = "./";
      out_gif_pdf_file += output_gif;
      out_gif_pdf_file += "/";
      out_gif_pdf_file += "gr_";
      out_gif_pdf_file += file_counter;
      out_gif_pdf_file += ".gif";
      //
      gif_name_short  = "gr_";
      gif_name_short += file_counter;
      gif_name_short += ".gif";      
      c1->SaveAs(out_gif_pdf_file.Data());
      gif_name_short_v.push_back(gif_name_short);
      merge_gif<<gif_name_short<<" ";
      file_counter++;
    }
  }
  TString outtotGifName = "> mrg.gif";
  TString outtotGif_inv_Name = "> mrg_inv.gif";
  merge_gif<<outtotGifName.Data();
  //
  merge_gif<<std::endl<<std::endl;
  merge_gif<<"gifsicle --delay 1 --loopcount ";
  for( unsigned int i = 0; i<gif_name_short_v.size(); i++)
    merge_gif<<gif_name_short_v.at(gif_name_short_v.size() - 1 - i)<<" ";
  //
  merge_gif<<outtotGif_inv_Name.Data();
  merge_gif.close();
  */
}
