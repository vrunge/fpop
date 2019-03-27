#include "Omega.h"
#include "Cost.h"
#include "Piece.h"
#include "Track.h"

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

  //std::cout << "ALGOALGOALGOALGOALGOALGOALGOALGOALGOALGOALGOALGOALGO   "<< 0 << std::endl;
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
    //std::cout << "ALGOALGOALGOALGOALGOALGOALGOALGOALGOALGOALGOALGOALGO   "<< i << std::endl;
    if(track.getArgminimum() - vectData[i] > 0){Delta = true;}else{Delta = false;}

    functionalCost = functionalCost -> cut(track.getMinimum() + penalty, track.getArgminimum(), Delta);
    //functionalCost -> show();
    functionalCost -> addDataPoint(vectData[i], track); /// + update track

    ///
    chpts[i] = i - track.getNbSteps();
    ms[i] = track.getArgminimum();
    //std::cout <<  ms[i] << " -- " << chpts[i] << " --- " << Delta << std::endl;
  }

  //std::cout << "ALGOALGOALGOALGOALGOALGOALGOALGOALGOALGOALGOALGOALGO   "<< n << std::endl;

  globalCost = track.getMinimum() - penalty;

  for(int i = 0; i < n; i++)
  {
 //   std::cout << chpts[i] << " --- " << ms[i] << std::endl;
  }

  ///backtracking
  int tau = n - 1;

  while(tau != -1)
  {
    changepoints.push_back(tau + 1);
    means.push_back(ms[tau]);
    tau = chpts[tau];
  }

  //std::cout << "reverse" << std::endl;

  std::reverse(changepoints.begin(), changepoints.end());
  std::reverse(means.begin(), means.end());

  //std::cout << "ENDENDENDENDENDENDENDENDENDENDENDENDENDENDENDENDENDENDENDENDENDENDENDENDENDENDEND" << std::endl;


  delete[] chpts;
  chpts = NULL;
  delete[] ms;
  ms = NULL;

}


