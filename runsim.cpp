//my
#include "sim.hh"
#include "simConfStr.hh"
#include "sierpinskiStr.hh"

//root
#include "TROOT.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TRandom3.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TCanvas.h"

//C, C++
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <cmath>

// Test of sierpinskiStr: build the polygon from the configuration file,
// print the vertices, check the geometry and save the closed polygon
// as TGraph "gr_polygon" into the output root file.
// Returns 0 if all checks pass, 1 otherwise.
Int_t test_sierpinski(TString input_configuration_file, TString outputRootFile){
  simConfStr conf;
  conf.readFromFile(input_configuration_file);
  conf.printInfo(2);
  //
  sierpinskiStr sp(conf);
  sp.printInfo();
  //
  const Double_t eps = 1.0e-9;
  Bool_t ok = true;
  // 1) center of the figure at (0,0)
  Double_t xc = 0.0, yc = 0.0;
  for(Int_t i = 0; i < sp.n; i++){
    xc += sp.x.at(i);
    yc += sp.y.at(i);
  }
  xc /= sp.n;
  yc /= sp.n;
  if(TMath::Abs(xc) > eps || TMath::Abs(yc) > eps){
    std::cout<<"  FAIL : center ("<<xc<<", "<<yc<<") != (0, 0)"<<std::endl;
    ok = false;
  }
  // 2) all sides equal side_size, all vertices on the circumcircle
  for(Int_t i = 0; i < sp.n; i++){
    Int_t j = (i + 1) % sp.n;
    Double_t side = std::hypot(sp.x.at(j) - sp.x.at(i), sp.y.at(j) - sp.y.at(i));
    if(TMath::Abs(side - sp.side_size) > eps){
      std::cout<<"  FAIL : side "<<i<<"-"<<j<<" = "<<side<<" != "<<sp.side_size<<std::endl;
      ok = false;
    }
    Double_t r = std::hypot(sp.x.at(i), sp.y.at(i));
    if(TMath::Abs(r - sp.R) > eps){
      std::cout<<"  FAIL : vertex "<<i<<" radius "<<r<<" != R "<<sp.R<<std::endl;
      ok = false;
    }
  }
  // 3) bottom is a horizontal side: exactly two vertices at minimal y
  Double_t ymin = sp.y.at(0);
  for(Int_t i = 1; i < sp.n; i++)
    if(sp.y.at(i) < ymin)
      ymin = sp.y.at(i);
  Int_t n_bottom = 0;
  for(Int_t i = 0; i < sp.n; i++)
    if(TMath::Abs(sp.y.at(i) - ymin) < eps)
      n_bottom++;
  if(n_bottom != 2){
    std::cout<<"  FAIL : "<<n_bottom<<" vertices at the bottom (expected 2)"<<std::endl;
    ok = false;
  }
  //
  std::cout<<"--------------------"<<std::endl
	   <<"test_sierpinski : "<<(ok ? "PASS" : "FAIL")<<std::endl;
  //
  // closed polygon
  TGraph *gr_polygon = new TGraph();
  gr_polygon->SetNameTitle("gr_polygon", "gr_polygon");
  for(Int_t i = 0; i <= sp.n; i++)
    gr_polygon->SetPoint(i, sp.x.at(i % sp.n), sp.y.at(i % sp.n));
  gr_polygon->SetMarkerStyle(20);
  gr_polygon->SetLineWidth(2);
  //
  TFile *rootFile = new TFile(outputRootFile.Data(), "RECREATE", " Histograms", 1);
  if(rootFile->IsZombie()){
    std::cout<<"  ERROR ---> file "<<outputRootFile.Data()<<" is zombi"<<std::endl;
    assert(0);
  }
  gr_polygon->Write();
  rootFile->Close();
  std::cout<<"  Output file ---> "<<outputRootFile.Data()<<std::endl;
  delete rootFile;
  delete gr_polygon;
  //
  return (ok ? 0 : 1);
}

int main(int argc, char *argv[]){
  //
  clock_t start, finish;
  start = clock();
  //
  if(argc == 4 && atoi(argv[1])==0){
    //
    TString input_configuration_file = argv[2];
    TString outputRootFileWith_graph = argv[3];
    //
    std::cout<<"runsim"<<std::endl
	     <<"input_configuration_file        "<<input_configuration_file<<std::endl
    	     <<"outputRootFileWith_graph        "<<outputRootFileWith_graph<<std::endl;
    //
    simConfStr *conf = new simConfStr();
    conf->readFromFile(input_configuration_file);
    conf->printInfo(2);
    //
    //
    //
    UInt_t seed = 12313;
    TRandom3 *rnd = new TRandom3(seed);
    sim *gsim = new sim( rnd, conf);
    gsim->runsim(conf->n);
    gsim->save_graph(outputRootFileWith_graph);
    gsim->save_gif("tr_gif", conf->n_gifs);
    //
    delete gsim; // also deletes rnd and conf
  }
  else if(argc == 3 && atoi(argv[1])==100){
    //
    TString input_configuration_file = argv[2];
    std::cout<<"test read conf"<<std::endl
	     <<"input_configuration_file        "<<input_configuration_file<<std::endl;
    //
    simConfStr *conf = new simConfStr();
    conf->readFromFile(input_configuration_file);
    conf->printInfo(2);
    delete conf;
  }
  else if(argc == 4 && atoi(argv[1])==101){
    //
    TString input_configuration_file = argv[2];
    TString outputRootFile = argv[3];
    std::cout<<"test sierpinski"<<std::endl
	     <<"input_configuration_file        "<<input_configuration_file<<std::endl
	     <<"outputRootFile                  "<<outputRootFile<<std::endl;
    //
    if(test_sierpinski(input_configuration_file, outputRootFile) != 0)
      return 1;
  }
  else{
    std::cout<<"  runID [1] = 0                        "<<std::endl
 	     <<"        [2] - input configuration file "<<std::endl
    	     <<"        [3] - output root file with graphs"<<std::endl;
    std::cout<<"  runID [1] = 100 (test read conf)     "<<std::endl
 	     <<"        [2] - input configuration file "<<std::endl;
    std::cout<<"  runID [1] = 101 (test sierpinski)    "<<std::endl
 	     <<"        [2] - input configuration file "<<std::endl
	     <<"        [3] - output root file with gr_polygon"<<std::endl;
  }  //
  finish = clock();
  std::cout<<"-------------------------"<<std::endl
  	   <<"Working time : "<<((Double_t)(finish - start)/CLOCKS_PER_SEC)<<" (sec)"<<std::endl
  	   <<"-------------------------"<<std::endl;
  //
  return 0;
}
