#include "Interval.h"

#include<iostream>

Interval::Interval() : m_a(INFINITY), m_b(INFINITY){}
Interval::Interval(double a, double b) : m_a(a), m_b(b){}

void Interval::seta(double a){m_a = a;}
void Interval::setb(double b){m_b = b;}
double Interval::geta() const{return(m_a);}
double Interval::getb() const{return(m_b);}


bool Interval::isInside(double x) const {return((m_a <= x) && (x <= m_b));}

void Interval::intersection(Interval const& intervalInterRoots, int& type)
{
  type = -1;
  //type = -1 => intersection = empty or a point
  //type = 0 => intersection = no change
  //type = 1 => intersection = change the left bound
  //type = 2 => intersection = change the right bound
  //type = 3 => intersection = change the two bounds

  if((intervalInterRoots.m_a < m_b) && (m_a < intervalInterRoots.m_b))
  {
    type = 0;
    if(intervalInterRoots.m_a > m_a){type = type + 1; m_a = intervalInterRoots.m_a;}
    if(intervalInterRoots.m_b < m_b){type = type + 2; m_b = intervalInterRoots.m_b;}
  }
}



void Interval::show() const
{
  //std::cout << "INTERVAL ## " << m_a << " -- " << m_b << std::endl;
}
