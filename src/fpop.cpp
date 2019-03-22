
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
  if(change != "mean" && change != "variance" && change != "exp" && change != "poisson")
    {throw std::range_error("Arugment 'change' not appropriate. Choose among 'mean', 'variance', 'exp' and 'poisson'");}

  if(change == "variance")
  {
    double mean = 0;
    for(int i = 0; i < vectData.size(); i++){mean = mean + vectData[i];}
    mean = mean/vectData.size();
    for(int i = 0; i < vectData.size(); i++){vectData[i] = vectData[i] - mean;}
  }

  if(change == "exp")
  {
    for(int i = 0; i < vectData.size(); i++){if(vectData[i] <= 0){throw std::range_error("Data has to be all positive");}}
  }

  if(change == "poisson")
  {
    for(int i = 0; i < vectData.size(); i++){if(vectData[i] < 0 || (vectData[i]  > floor(vectData[i]))){throw std::range_error("There are some non-integer data");}}
  }

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
