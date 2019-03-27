#include "Omega.h"
#include "Cost.h"
#include "Piece.h"
#include "Track.h"

#include "ExternFunctions.h"

#include <algorithm>    // std::reverse
#include<iostream>
#include <stdlib.h>

//####### constructor #######////####### constructor #######////####### constructor #######//
//####### constructor #######////####### constructor #######////####### constructor #######//

Omega::Omega(double beta)
{
  penalty = beta;
  functionalCost = new Piece(Interval(-INFINITY, INFINITY));
}

//####### destructor #######////####### destructor #######////####### destructor #######//
//####### destructor #######////####### destructor #######////####### destructor #######//

Omega::~Omega(){delete(functionalCost);}

//####### accessors #######////####### accessors #######////####### accessors #######//
//####### accessors #######////####### accessors #######////####### accessors #######//

std::vector< int > Omega::GetChangepoints() const {return(changepoints);}
std::vector< double > Omega::GetMeans() const {return(means);}
double Omega::GetGlobalCost() const {return(globalCost);}


//####### algo #######////####### algo #######////####### algo #######//
//####### algo #######////####### algo #######////####### algo #######//

void Omega::algo(std::vector< double >& vectData)
{
  int n = vectData.size();

  ///INITIALIZATION
  Track track = Track();
  int* chpts = new int[n]; ///vector of changepoints build by fpop
  double* ms = new double[n]; ///vector of means build by fpop

  ///
  /// FIRST STEP
  ///
  addmyConstant(functionalCost -> getRefCost(), penalty);
  functionalCost -> addDataPoint(vectData[0], track); /// + update track
  chpts[0] =  - track.getNbSteps();
  ms[0] = track.getArgminimum();

  ///
  /// INTERN STEPS
  ///
  bool Delta; /// argminimum - vectData[i] > 0
  for(unsigned int i = 1; i < n; i++)
  {
    if(track.getArgminimum() - cost_argmin(Cost(cost_coeff(vectData[i]))) > 0){Delta = true;}else{Delta = false;}

    functionalCost = functionalCost -> cut(track.getMinimum() + penalty, track.getArgminimum(), Delta);
    functionalCost -> addDataPoint(vectData[i], track); /// + update track

    ///
    chpts[i] = i - track.getNbSteps();
    ms[i] = track.getArgminimum();
  }

  globalCost = track.getMinimum() - penalty;

  ///backtracking
  int tau = n - 1;

  while(tau != -1)
  {
    changepoints.push_back(tau + 1);
    means.push_back(ms[tau]);
    tau = chpts[tau];
  }

  std::reverse(changepoints.begin(), changepoints.end());
  std::reverse(means.begin(), means.end());

  delete[] chpts;
  chpts = NULL;
  delete[] ms;
  ms = NULL;

}


