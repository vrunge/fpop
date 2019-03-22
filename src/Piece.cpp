#include "Piece.h"
#include "Cost.h"
#include "ExternFunctions.h"

#include <math.h>
#include <stdlib.h>
#include<iostream>

#include<vector>

Piece::Piece(Interval const& inter, Cost const& cost)
{
  m_interval = inter;
  m_cost = cost;
  nxt = NULL;
}

Piece::Piece(const Piece* piece)
{
  m_interval = piece -> m_interval;
  m_cost = piece -> m_cost;
  nxt = NULL;
}


//####### destructor #######////####### destructor #######////####### destructor #######//
//####### destructor #######////####### destructor #######////####### destructor #######//

Piece::~Piece()
{
  delete(nxt);
  nxt = NULL;
}


//####### accessors #######////####### accessors #######////####### accessors #######//
//####### accessors #######////####### accessors #######////####### accessors #######//

Interval Piece::getInterval() const {return(m_interval);}
Cost Piece::getCost() const {return(m_cost);}
Cost& Piece::getRefCost(){return(m_cost);}


//####### addConstant #######////####### addConstant #######////####### addConstant #######//
//####### addConstant #######////####### addConstant #######////####### addConstant #######//

void Piece::addConstant(double& myconstant)
{
  addmyConstant(this -> getRefCost(), myconstant);
}

//####### addDataPoint #######////####### addDataPoint #######////####### addDataPoint #######//
//####### addDataPoint #######////####### addDataPoint #######////####### addDataPoint #######//

void Piece::addDataPoint(double& y, Track& track)
{
  //Initialize minimum to +Inf
  track.setMinimum(INFINITY);

  ///Build Cost to add
  Cost newCost = Cost(cost_coeff(y));
  Piece* tmp = this;
  double minimum;

  while(tmp != NULL)
  {
    addCost(tmp -> getRefCost(), newCost); /// update the cost
    minimum = cost_min(tmp -> m_cost); /// find the minimum
    if(minimum < track.getMinimum())
      {
        track.setMinimum(minimum);
        track.setArgminimum(cost_argmin(tmp -> m_cost));
        track.setNbSteps(cost_age(tmp -> m_cost));
      }
    tmp = tmp -> nxt;
  }
  //std::cout << "TRACKTRACKTRACKTRACKTRACKTRACK "<< track.getArgminimum() << " --- " << track.getMinimum() << " --- " << track.getNbSteps() << std::endl;
}



//####### cut #######////####### cut #######////####### cut #######//
//####### cut #######////####### cut #######////####### cut #######//

Piece* Piece::cut(double level, double currentArgmin, bool Delta)
{
  Piece* FirstPiece;
  Piece* tmp = this;
  int type;

  /// intervals
  Interval interRoots = Interval();
  Interval saveInterval;

  switch(Delta)
  {
  ///
  /// level cut for means < argminimum
  ///
  case true:
  {
    //INITIALIZATION of FirstPiece
    FirstPiece = new Piece();
    FirstPiece -> addConstant(level);
    FirstPiece -> m_interval.seta(-INFINITY);
    FirstPiece -> nxt = tmp;
    tmp = FirstPiece;

    while((tmp -> nxt != NULL) && (tmp -> nxt -> m_interval.isInside(currentArgmin) == false))
    {
      saveInterval = tmp -> nxt -> m_interval;
      tmp -> nxt -> m_interval.intersection(interRoots, type);

      if(type != 0)
      {
        tmp -> nxt -> m_interval = saveInterval;
        interRoots = cost_intervalInterRoots(tmp -> nxt -> m_cost, level); //recompute interRoots
        tmp -> nxt -> m_interval.intersection(interRoots, type);
      }
      tmp = tmp -> ProcessNxt(interRoots, level, type);
    }

    ///piece of the currentArgmin
    interRoots = cost_intervalInterRoots(tmp -> nxt -> m_cost, level);
    if(tmp -> nxt -> m_interval.geta() < interRoots.geta())
    {
      tmp -> m_interval.setb(interRoots.geta());
      tmp -> nxt -> m_interval.seta(interRoots.geta());
    }
    break;
  }

  ///
  /// level cut for means > argminimum
  ///
  case false:
  {
    FirstPiece = tmp;
    while(tmp -> m_interval.isInside(currentArgmin) == false){tmp = tmp -> nxt;}

    ///piece of the currentArgmin
    interRoots = cost_intervalInterRoots(tmp  -> m_cost, level);
    if(interRoots.getb() < tmp -> m_interval.getb()) ///type = 2
    {
      Piece* newConstPiece2 = new Piece();
      newConstPiece2 -> addConstant(level);
      newConstPiece2 -> m_interval.seta(interRoots.getb());
      newConstPiece2 -> nxt = tmp -> nxt;
      tmp -> m_interval.setb(interRoots.getb());
      tmp -> nxt = newConstPiece2;
      tmp = newConstPiece2;
    }

    ///right pieces
    while(tmp -> nxt != NULL)
    {
      saveInterval = tmp -> nxt -> m_interval;
      tmp -> nxt -> m_interval.intersection(interRoots, type);

      if(type != 0)
      {
        tmp -> nxt -> m_interval = saveInterval; ///delete intersection
        interRoots = cost_intervalInterRoots(tmp -> nxt -> m_cost, level); //recompute interRoots
        tmp -> nxt -> m_interval.intersection(interRoots, type);
      }
      tmp = tmp -> ProcessNxt(interRoots, level, type);
    }
    break;
  }
  }

  return(FirstPiece);
}


//####### ProcessNxt #######////####### ProcessNxt #######////####### ProcessNxt #######//
//####### ProcessNxt #######////####### ProcessNxt #######////####### ProcessNxt #######//

Piece* Piece::ProcessNxt(Interval& interRoots, double& level, int& type)
{
  Piece* tmp = this;

  switch(type)
  {
    case -1:
    {
      Piece* ToDeletePiece = tmp -> nxt;
      tmp -> nxt = ToDeletePiece -> nxt;
      ToDeletePiece -> nxt = NULL;
      delete(ToDeletePiece);
      break;
    }
    case 0:
    {
      tmp = tmp -> nxt;
      break;
    }
    case 1:
    {
      tmp -> m_interval.setb(interRoots.geta());
      tmp = tmp -> nxt;
      break;
    }
    case 2:
    {
      Piece* newConstPiece2 = new Piece();
      newConstPiece2 -> addConstant(level);
      newConstPiece2 -> m_interval.seta(interRoots.getb());
      newConstPiece2 -> nxt = tmp -> nxt -> nxt;
      tmp -> nxt -> nxt = newConstPiece2;
      tmp = newConstPiece2;
      break;
    }
    case 3:
    {
      tmp -> m_interval.setb(interRoots.geta());
      Piece* newConstPiece3 = new Piece();
      newConstPiece3 -> addConstant(level);
      newConstPiece3 -> m_interval.seta(interRoots.getb());
      newConstPiece3 -> nxt = tmp -> nxt -> nxt;
      tmp -> nxt -> nxt = newConstPiece3;
      tmp = newConstPiece3;
      break;
    }
  }

  return(tmp);
}



//####### show #######////####### show #######////####### show #######//
//####### show #######////####### show #######////####### show #######//

void Piece::show()
{
  Piece* tmp = this;
  //if(tmp == NULL){std::cout << "#NULL EMPTY POINTER# "<< std::endl;}
  while(tmp != NULL)
  {
    //std::cout << "#INTERVAL# "<< tmp -> m_interval.geta() << " -- " << tmp -> m_interval.getb() << " && ";
    //std::cout << "#COST# "<<  tmp -> m_cost.m_A << " -- " <<  tmp -> m_cost.m_B << " -- " <<  tmp -> m_cost.constant << std::endl;
    tmp = tmp -> nxt;
  }
}

