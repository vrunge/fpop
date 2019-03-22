#ifndef OMEGA_H
#define OMEGA_H

#include"Piece.h"

#include <math.h>
#include<vector>

#include "Rcpp.h"


class Omega
{
  public:
    Omega(double beta);
    ~Omega();

    std::vector< int > GetChangepoints() const;
    std::vector< double > GetMeans() const;
    double GetGlobalCost() const;
    void algo(std::vector< double >& vectData);

  private:
    double penalty;
    Piece* functionalCost;

    std::vector< int > changepoints; ///vector of changepoints build by fpop (first index of each segment). size c
    std::vector< double > means; ///vector of means build by fpop. size c
    double globalCost;
};

#endif // OMEGA_H
