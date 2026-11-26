#ifndef sierpinskiStr_hh
#define sierpinskiStr_hh

//my
#include "simConfStr.hh"

//root
#include "TROOT.h"
#include "TMath.h"
#include "TVector2.h"

//C, C++
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <assert.h>
#include <vector>

// Regular polygon with n vertices (n = 3 triangle, n = 4 square, ...)
// and side length side_size.
// Center of the figure is at (0,0).
// Orientation: the bottom of the figure is a horizontal side (not a vertex).
struct sierpinskiStr {
  //
  Int_t n;
  Double_t side_size;
  Double_t R;              // circumradius
  Double_t xmin;
  Double_t xmax;
  Double_t ymin;
  Double_t ymax;
  std::vector<Double_t> x; // vertex x positions
  std::vector<Double_t> y; // vertex y positions
  //
  sierpinskiStr(){
    n = 0;
    side_size = 0.0;
    R = 0.0;
    xmin = 0.0;
    xmax = 0.0;
    ymin = 0.0;
    ymax = 0.0;
  }
  sierpinskiStr(const simConfStr &conf){
    fromConf(conf);
  }
  void fromConf(const simConfStr &conf){
    n = conf.ver_n;
    side_size = conf.side_size;
    assert(n >= 3);
    assert(side_size > 0.0);
    // circumradius of a regular n-gon with side length side_size
    R = side_size / (2.0 * TMath::Sin(TMath::Pi() / n));
    // Vertices at -pi/2 +- pi/n are symmetric around the downward
    // direction, so the side between them is horizontal and at the bottom.
    Double_t phi0 = -TMath::Pi() / 2.0 + TMath::Pi() / n;
    x.clear();
    y.clear();
    TVector2 tmp_v;
    for(Int_t i = 0; i < n; i++){
      tmp_v.SetMagPhi(R, phi0 + 2.0 * TMath::Pi() / n * i);
      x.push_back(tmp_v.X());
      y.push_back(tmp_v.Y());
    }
    // bounding box of the figure
    xmin = xmax = x.at(0);
    ymin = ymax = y.at(0);
    for(Int_t i = 1; i < n; i++){
      xmin = TMath::Min(xmin, x.at(i));
      xmax = TMath::Max(xmax, x.at(i));
      ymin = TMath::Min(ymin, y.at(i));
      ymax = TMath::Max(ymax, y.at(i));
    }
  }
  // Point (xp, yp) inside the polygon (boundary included).
  // Vertices are ordered counter-clockwise, so the point is inside
  // if it is on the left of (or on) every edge.
  Bool_t isInside(Double_t xp, Double_t yp) const {
    for(Int_t i = 0; i < n; i++){
      Int_t j = (i + 1) % n;
      Double_t cross = (x.at(j) - x.at(i)) * (yp - y.at(i)) - (y.at(j) - y.at(i)) * (xp - x.at(i));
      if(cross < 0.0)
	return false;
    }
    return true;
  }
  void printInfo(){
    std::cout<<"--------------------"<<std::endl
	     <<"n         "<<n<<std::endl
	     <<"side_size "<<side_size<<std::endl
	     <<"R         "<<R<<std::endl;
    for(Int_t i = 0; i < n; i++)
      std::cout<<std::setw(3)<<i<<" "
	       <<std::setw(12)<<x.at(i)<<" "
	       <<std::setw(12)<<y.at(i)<<std::endl;
  }
};

#endif //#ifndef sierpinskiStr_hh
