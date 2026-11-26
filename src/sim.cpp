//my
#include "sim.hh"
#include "simConfStr.hh"
#include "sierpinskiStr.hh"

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
  _gr_frame = NULL;
  _gr = NULL;
}

sim::sim( TRandom3 *rnd, simConfStr *conf){
  _rnd = rnd;
  _conf = conf;
  _gr_frame = NULL;
  _gr = NULL;
}

sim::~sim(){
  delete _rnd;
  delete _conf;
}

void sim::runsim(Int_t nsteps){
  sierpinskiStr sp(*_conf);
  //
  _gr = new TGraph();
  _gr->SetNameTitle("gr","gr");
  // vertices of the figure
  for(unsigned int i = 0; i<(unsigned int)sp.n; i++)
    _gr->SetPoint(_gr->GetN(),sp.x.at(i),sp.y.at(i));
  //
  Double_t x, y;
  Int_t iapex;
  Double_t x_ratio, y_ratio;
  // starting point: uniform inside the figure (rejection sampling in the bounding box)
  do {
    x = _rnd->Uniform(sp.xmin, sp.xmax);
    y = _rnd->Uniform(sp.ymin, sp.ymax);
  } while(!sp.isInside(x, y));
  for(Int_t i = 0; i<nsteps; i++){
    // uniformly chosen apex: vertex (sp.x[iapex], sp.y[iapex])
    iapex = _rnd->Integer(sp.n);
    // point on the line between (x, y) and the apex, at fraction ratio
    // of the distance from (x, y) towards the apex (ratio = 1/2 -> middle)
    x_ratio = x + _conf->ratio * (sp.x.at(iapex) - x);
    y_ratio = y + _conf->ratio * (sp.y.at(iapex) - y);
    //
    //
    x = x_ratio;
    y = y_ratio;
    //
    _gr->SetPoint(_gr->GetN(),x,y);
  }
}

void sim::save_graph(TString outputRootFileWith_graph){
  //
  //
  // square frame of side frame_L centered at (0,0), plus frame_margine on each side
  Double_t xl = - _conf->frame_L/2.0 - _conf->frame_margine;
  Double_t xr =   _conf->frame_L/2.0 + _conf->frame_margine;
  Double_t yb = - _conf->frame_L/2.0 - _conf->frame_margine;
  Double_t yt =   _conf->frame_L/2.0 + _conf->frame_margine;
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
  _gr_frame->Write();
  if(_gr != NULL)
    _gr->Write();
  rootFile->Close();
  delete rootFile;

}

void sim::save_gif(TString output_gif, Int_t n_gifs){
  // Animation of the growing pattern: frame k (k = 0 .. n_gifs-1) shows the
  // first (k+1)/n_gifs fraction of the points of _gr, inside the frame _gr_frame.
  // Requires runsim() (fills _gr) and save_graph() (fills _gr_frame) before.
  Int_t np = _gr->GetN();
  mkdir(output_gif.Data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  //
  TString merge_gif_name=output_gif;
  merge_gif_name += "/mrg.sh";
  std::ofstream merge_gif;
  merge_gif.open(merge_gif_name.Data());
  //
  merge_gif<<"gifsicle --delay 30 --loopcount ";
  //
  std::vector<TString> gif_name_short_v;
  //
  gStyle->SetOptStat(kFALSE);
  _gr_frame->SetMarkerStyle(25);
  //
  TString out_gif_file="";
  TString gif_name_short="";
  for(Int_t i = 0; i<n_gifs; i++){
    // number of points shown in this frame
    Int_t np_frame = (Int_t)((Long64_t)np * (i + 1) / n_gifs);
    TGraph *gr = new TGraph(np_frame, _gr->GetX(), _gr->GetY());
    gr->SetTitle("");
    gr->SetName("");
    gr->SetMarkerStyle(1);
    //
    TCanvas *c1 = new TCanvas("c1","c1",800,800);
    gPad->SetRightMargin(0.12);
    gPad->SetLeftMargin(0.12);
    gPad->SetTopMargin(0.1);
    gPad->SetBottomMargin(0.15);
    _gr_frame->SetTitle("");
    _gr_frame->Draw("ap");
    gr->Draw("p same");
    //
    gif_name_short  = "gr_";
    gif_name_short += i;
    gif_name_short += ".gif";
    out_gif_file  = "./";
    out_gif_file += output_gif;
    out_gif_file += "/";
    out_gif_file += gif_name_short;
    c1->SaveAs(out_gif_file.Data());
    gif_name_short_v.push_back(gif_name_short);
    merge_gif<<gif_name_short<<" ";
    //
    delete c1;
    delete gr;
  }
  _gr_frame->SetTitle("gr_frame");
  //
  merge_gif<<"> mrg.gif";
  merge_gif<<std::endl<<std::endl;
  merge_gif<<"gifsicle --delay 1 --loopcount ";
  for(unsigned int i = 0; i<gif_name_short_v.size(); i++)
    merge_gif<<gif_name_short_v.at(gif_name_short_v.size() - 1 - i)<<" ";
  merge_gif<<"> mrg_inv.gif";
  merge_gif<<std::endl;
  merge_gif.close();
}
