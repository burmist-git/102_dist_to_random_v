#ifndef sim_hh
#define sim_hh

//C, C++
#include <iostream>
#include <vector>

//root
#include "TROOT.h"
#include "TMath.h"
#include "TFile.h"

//my
struct simConfStr;

//root
class TString;
class TRandom3;
class TGraph;
class TH1D;
class TVector2;
class TGraph;
class TFile;

class sim {

public:
  //
  sim();
  sim( TRandom3 *rnd, simConfStr *conf);
  ~sim();
  
public:
  //
  void runsim(Int_t nsteps = 100);
  void save_graph(TString outputRootFileWith_graph = "trajectories.root");
  void save_gif(TString output_gif = "tr_gif", Int_t n_gifs = 100);

private:
  //
  TRandom3 *_rnd;
  simConfStr *_conf;

  TGraph *_gr_frame;
  TGraph *_gr;
  
};

#endif //ifndef sim_hh
