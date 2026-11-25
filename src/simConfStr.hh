#ifndef simConfStr_hh
#define simConfStr_hh

//root
#include "TROOT.h"
#include "TMath.h"
#include "TVector2.h"

//C, C++
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <assert.h>
#include <string>

struct simConfStr {
  //
  Int_t n;
  Int_t ver_n;
  Double_t side_size;
  TString ratio_s;
  Double_t ratio;
  Double_t frame_L;
  Double_t frame_margine;
  //
  simConfStr(){
    n = 1000;
    ver_n = 3;
    side_size = 1.0;
    ratio_s = "1:2";
    ratio = 1.0/2.0;
    frame_L = 1.0;
    frame_margine = 0.1;
  }
  void printInfo(Int_t verboseLevel = 1){
    std::cout<<"--------------------"<<std::endl;
    if(verboseLevel>0){
      std::cout<<"n             "<<n<<std::endl
	       <<"ver_n         "<<ver_n<<std::endl
	       <<"side_size     "<<side_size<<std::endl
	       <<"ratio_s       "<<ratio_s<<std::endl
	       <<"ratio         "<<ratio<<std::endl
	       <<"frame_L       "<<frame_L<<std::endl
	       <<"frame_margine "<<frame_margine<<std::endl;
    }
  }  
  // Parse "a:b" (optionally quoted) into ratio_s and ratio = a/b
  void setRatio(std::string val){
    if(val.size() >= 2 && val.front() == '"' && val.back() == '"')
      val = val.substr(1, val.size() - 2);
    size_t pos = val.find(':');
    if(pos == std::string::npos || pos == 0 || pos == val.size() - 1){
      std::cout<<"Wrong ratio_s format (expected a:b) : "<<val<<std::endl;
      assert(0);
    }
    Double_t a = atof(val.substr(0, pos).c_str());
    Double_t b = atof(val.substr(pos + 1).c_str());
    if(b == 0.0){
      std::cout<<"ratio_s denominator is zero : "<<val<<std::endl;
      assert(0);
    }
    ratio_s = val.c_str();
    ratio = a/b;
  }
  void readFromFile(TString name){
    std::ifstream confFile(name.Data());
    if (confFile.is_open()) {
      std::string mot;
      while(confFile>>mot){
	//std::cout<<mot<<std::endl;
	if(mot == "n:")
	  confFile>>n;
	if(mot == "ver_n:")
	  confFile>>ver_n;
	if(mot == "side_size:")
	  confFile>>side_size;
	if(mot == "ratio_s:"){
	  std::string val;
	  confFile>>val;
	  setRatio(val);
	}
	if(mot == "frame_L:")
	  confFile>>frame_L;
	if(mot == "frame_margine:")
	  confFile>>frame_margine;
      }
      confFile.close();
    }
    else {
      std::cout << "Unable to open file"<<std::endl; 
      assert(0);
    }
  }
};

#endif //#ifndef simConfStr_hh
