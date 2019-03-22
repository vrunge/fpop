#include "Track.h"

Track::Track()
{
  minimum = INFINITY;
  argminimum = INFINITY;
  nbSteps = 0;
}

double Track::getMinimum() const {return(minimum);}
double Track::getArgminimum() const {return(argminimum);}
int Track::getNbSteps() const {return(nbSteps);}

void Track::setMinimum(double mini){minimum = mini;}
void Track::setArgminimum(double argmini){argminimum = argmini;}
void Track::setNbSteps(int newNbSteps){nbSteps = newNbSteps;}

