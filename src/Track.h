#ifndef TRACK_H
#define TRACK_H

#include"math.h"

class Track
{
  public:
    Track();

    double getMinimum() const;
    double getArgminimum() const;
    int getNbSteps() const;

    void setMinimum(double mini);
    void setArgminimum(double argmini);
    void setNbSteps(int newNbSteps);

  private:
    double minimum;
    double argminimum;
    int nbSteps;
};

#endif // TRACK_H
