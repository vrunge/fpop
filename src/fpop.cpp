#include<Rcpp.h>
// Enable C++11 via this plugin (Rcpp 0.10.3 or later)
// [[Rcpp::plugins(cpp11)]]

#include<math.h>

#include"Omega.h"
#include"Cost.h"
#include"ExternFunctions.h"

using namespace Rcpp;
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//' fpop with many cost functions
//' @description A fast implementation of the fpop algorithm with many cost functions available
//' @param vectData vector of data to segment
//' @param penalty the penalty coefficient. A positive number
//' @param change A string defining the kind of cost function to use
//' @return a list of two vectors and a value  = (changepoints, segment parameters, global cost).
//' 'changepoints' is the vector of changepoints (we give the last element of each segment).
//' 'parameters' is the vector of successive parameters of each segment
//' 'globalCost' is a number equal to the global cost of the graph-constrained segmentation
// [[Rcpp::export]]
List fpop(std::vector<double> vectData, double penalty, std::string change = "mean")
{
  if(change != "mean" && change != "variance" && change != "exp" && change != "poisson" && change != "negbin")
  {throw std::range_error("Arugment 'change' not appropriate. Choose among 'mean', 'variance', 'exp', 'poisson' and 'negbin'");}

  if(penalty <= 0)
    {throw std::range_error("Penalty should be a positive number");}

  //////////
  //////////
  double epsilon = pow(10,-12);

  if(change == "variance")
  {
    double mean = 0;
    for(int i = 0; i < vectData.size(); i++){mean = mean + vectData[i];}
    mean = mean/vectData.size();
    for(int i = 0; i < vectData.size(); i++){vectData[i] = vectData[i] - mean; if(vectData[i] == 0){vectData[i] = epsilon;}}
  }

  if(change == "exp")
  {
    for(int i = 0; i < vectData.size(); i++){if(vectData[i] <= 0){throw std::range_error("Data has to be all positive");}}
  }

  if(change == "poisson")
  {
    for(int i = 0; i < vectData.size(); i++){if(vectData[i] < 0 || (vectData[i]  > floor(vectData[i]))){throw std::range_error("There are some non-integer data");}}
  }

  if(change == "negbin")
  {
    int windowSize = 100;
    int k = vectData.size() / windowSize;
    double mean = 0;
    double variance = 0;
    double disp = 0;

    for(int j = 0; j < k; j++)
    {
      mean = 0;
      variance = 0;
      for(int i = j * windowSize; i < (j + 1)*windowSize; i++){mean = mean + vectData[i];}
      mean = mean/windowSize;
      for(int i =  j * windowSize; i < (j + 1)*windowSize; i++){variance = variance + (vectData[i] - mean) * (vectData[i] - mean);}
      variance = variance/(windowSize - 1);
      disp = disp  + (mean * mean / (variance - mean));
    }
    disp = disp/k;
    for(int i = 0; i < vectData.size(); i++){vectData[i] = vectData[i]/disp; if(vectData[i] == 0){vectData[i] = epsilon/(1- epsilon);}}
  }

  //////////
  //////////

  cost_coeff = coeff_factory(change);
  cost_min = min_factory(change);
  cost_argmin = argmin_factory(change);
  cost_intervalInterRoots = intervalInterRoots_factory(change);
  cost_age = age_factory(change);

  Omega omega = Omega(penalty);
  omega.algo(vectData);

  /// RETURN
  List res = List::create(
    _["changepoints"] = omega.GetChangepoints(),
    _["parameters"] = omega.GetMeans(),
    _["globalCost"] = omega.GetGlobalCost()
  );

  return res;
}
