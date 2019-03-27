#ifndef PIECE_H
#define PIECE_H

#include "Interval.h"
#include "Track.h"
#include "Cost.h"

#include<vector>
#include<string>

class Piece
{
  public:
    Piece(Interval const& inter = Interval(), Cost const& cost = Cost());
    Piece(const Piece* piece); ///COPY CONSTRUCTOR => copy only the first Piece. piece -> nxt = NULL

    ~Piece();

    Interval getInterval() const;
    Cost getCost() const;
    Cost& getRefCost();

    void addConstant(double& myconstant);
    void addDataPoint(double& y, Track& track);
    Piece* cut(double level, double currentArgmin, bool Delta);

    Piece* ProcessNxt(Interval& interRoots, double& level, int& type);

    void show();

  private:

    Interval m_interval;
    Cost m_cost;  /// cost associated to the current piece
    Piece *nxt;   /// pointer to next piece

};


#endif // PIECE_H
