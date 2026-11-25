//my
#include "sim.hh"
#include "simConfStr.hh"

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

int main(int argc, char *argv[]){
  //
  clock_t start, finish;
  start = clock();
  //
  if(argc == 5 && atoi(argv[1])==0){
    //
    TString input_configuration_file = argv[2];
    Int_t nsteps = atoi(argv[3]);
    TString outputRootFileWith_graph = argv[4];
    //
    std::cout<<"runsim"<<std::endl
	     <<"input_configuration_file        "<<input_configuration_file<<std::endl
	     <<"nsteps                          "<<nsteps<<std::endl
    	     <<"outputRootFileWith_graph        "<<outputRootFileWith_graph<<std::endl;
    //
    simConfStr *conf = new simConfStr();
    conf->readFromFile(input_configuration_file);
    conf->printInfo(2);
    //
    //
    UInt_t seed = 12313;
    TRandom3 *rnd = new TRandom3(seed);
    sim *gsim = new sim( rnd, conf);
    gsim->runsim(nsteps);
    gsim->save_graph(outputRootFileWith_graph);
    gsim->save_gif("tr_gif", 20);
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
  else{
    std::cout<<"  runID [1] = 0                        "<<std::endl
 	     <<"        [2] - input configuration file "<<std::endl
	     <<"        [3] - number of steps          "<<std::endl
    	     <<"        [4] - output root file with graphs"<<std::endl;
    std::cout<<"  runID [1] = 100 (test read conf)     "<<std::endl
 	     <<"        [2] - input configuration file "<<std::endl;
  }  //
  finish = clock();
  std::cout<<"-------------------------"<<std::endl
  	   <<"Working time : "<<((Double_t)(finish - start)/CLOCKS_PER_SEC)<<" (sec)"<<std::endl
  	   <<"-------------------------"<<std::endl;
  //
  return 0;
}
