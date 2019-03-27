#include"Cost.h"

#include <vector>
#include <iostream>
#include <string>

#include"math.h"

Cost::Cost()
{
  m_A = 0;
  m_B = 0;
  constant = 0;
}

Cost::Cost(double* coeff)
{
  m_A = coeff[0];
  m_B = coeff[1];
  constant = coeff[2];
}

void addmyConstant(Cost& cost, double& cst){cost.constant = cost.constant + cst;}

void addCost(Cost& cost, const Cost& cost2)
{
  cost.m_A = cost.m_A + cost2.m_A;
  cost.m_B = cost.m_B + cost2.m_B;
  cost.constant = cost.constant + cost2.constant;
}


//####### simplefunctions #######////####### simplefunctions #######////####### simplefunctions #######//
//####### simplefunctions #######////####### simplefunctions #######////####### simplefunctions #######//


double log_factorial(double n)
{
  double res = 0;
  for(int i = 2; i < floor(n) + 1; i++){res = res + log(i);}
  return(res);
}


double log_choose(double x, double n)
{
  if(n == x){
    return 0;
  } else if (x == 0 && n != 0){
    return 0;
  } else if(x == 1)
  {
    return log(n);
  }
  else
  {
    return log_factorial(n) - log_factorial(x) - log_factorial(n - x);
  }
};


//####### coefficients #######////####### coefficients #######////####### coefficients #######//
//####### coefficients #######////####### coefficients #######////####### coefficients #######//

///mean cost = m_A*THETA^2 + m_B*log(THETA) + constant
///variance cost = m_A*THETA - m_B*log(THETA) + constant
/// poisson cost = m_A*THETA - m_B*log(THETA) + constant
/// exp cost = m_A*THETA - m_B*log(THETA) + constant
/// rbinom cost = -m_A*log(THETA) - m_B*log(1-THETA) + constant


double* mean_coeff(double y)
{
  double* coeff = new double[3];
  coeff[0] = 1;
  coeff[1] = -2*y;
  coeff[2] = y*y;
  return(coeff);
}

double* variance_coeff(double y)
{
  double* coeff = new double[3];
  coeff[0] = y*y;
  coeff[1] = 1;
  //coeff[2] = log(2*M_PI);
  coeff[2] = 0;
  return(coeff);
}

double* poisson_coeff(double y)
{
  double* coeff = new double[3];
  coeff[0] = 1;
  coeff[1] = y;
  //coeff[2] = log_factorial(y);
  coeff[2] = 0;
  return(coeff);
}

double* exp_coeff(double y)
{
  double* coeff = new double[3];
  coeff[0] = y;
  coeff[1] = 1;
  coeff[2] = 0;
  return(coeff);
}

double* negbin_coeff(double y)
{
  double* coeff = new double[3];
  coeff[0] = y;
  coeff[1] = 1;
  coeff[2] = 0;
  return(coeff);
}



//####### minimum #######////####### minimum #######////####### minimum #######//
//####### minimum #######////####### minimum #######////####### minimum #######//

double mean_min(const Cost& cost){return(-(cost.m_B*cost.m_B/(4*cost.m_A)) + cost.constant);}
double variance_min(const Cost& cost){return(cost.m_B - cost.m_B*log(cost.m_B/cost.m_A) + cost.constant);}

double poisson_min(const Cost& cost)
{
  double res = INFINITY;
  if(cost.m_A != 0 && cost.m_B == 0){res = cost.constant;}
  if(cost.m_A != 0 && cost.m_B != 0){res = cost.m_B - cost.m_B*log(cost.m_B/cost.m_A) + cost.constant;}
  return(res);
}


double negbin_min(const Cost& cost)
{
  double res = -INFINITY;
  if(cost.m_A != 0 && cost.m_B != 0){res = - cost.m_A * log(cost.m_A/(cost.m_A + cost.m_B)) - cost.m_B * log(cost.m_B/(cost.m_A + cost.m_B)) + cost.constant;}
  return(res);
}

//####### argminimum #######////####### argminimum #######////####### argminimum #######//
//####### argminimum #######////####### argminimum #######////####### argminimum #######//

double mean_argmin(const Cost& cost){return(-cost.m_B/(2*cost.m_A));}
double variance_argmin(const Cost& cost)
{
  ///cost.m_B/cost.m_A; for the functional cost parameter
  return(sqrt(cost.m_A/cost.m_B)); ///for the standard deviation parameter
}
double poisson_argmin(const Cost& cost){return(cost.m_B/cost.m_A);}
double negbin_argmin(const Cost& cost){return(cost.m_A/(cost.m_A + cost.m_B));}



//####### intervalInterRoots #######////####### intervalInterRoots #######////####### intervalInterRoots #######//
//####### intervalInterRoots #######////####### intervalInterRoots #######////####### intervalInterRoots #######//

//####### MEAN #######////
Interval mean_intervalInterRoots(const Cost& cost, double& level)
{
  Interval newElement = Interval();
  double Delta = cost.m_B*cost.m_B - 4*cost.m_A*(cost.constant - level);

  if(Delta > 0)
  {
    double R = sqrt(Delta);
    newElement = Interval((-cost.m_B - R)/(2 * cost.m_A), (-cost.m_B + R)/(2 * cost.m_A));
  }

  return(newElement);
}

//####### VARIANCE #######////
Interval variance_intervalInterRoots(const Cost& cost, double& level)
{
  Interval newElement = Interval();

  double eps = 0.000001;
  double temp = 1;
  //roots of A THETA - B log THETA + C = 0
  // <=> roots of THETA - log THETA = 1 + a
  double U = cost.m_A/cost.m_B;
  double a = -(((cost.constant - level)/cost.m_B) + log(U) + 1);

  if(a > 0)
  {
    int nb = 0;

    double leftRoot = -(1 + a);
    while(fabs(temp - leftRoot) > eps && nb < 100)
    {
      temp = leftRoot;
      leftRoot = leftRoot - 1 - (leftRoot + a)/(1 - exp(leftRoot));
      nb = nb + 1;
    }

    nb = 0;
    temp = 1;
    double rightRoot = 1 + a;
    while(fabs(temp - rightRoot) > eps && nb < 100)
    {
      temp = rightRoot;
      rightRoot = (log(rightRoot) + a)*rightRoot/(rightRoot - 1);
      nb = nb + 1;
    }

    leftRoot = exp(leftRoot)/U;
    rightRoot = rightRoot/U;

    newElement.seta(leftRoot);
    newElement.setb(rightRoot);
    if(leftRoot >= rightRoot){newElement = Interval();}
    }

  return(newElement);
}

//####### POISSON #######////
Interval poisson_intervalInterRoots(const Cost& cost, double& level)
{
  Interval newElement = Interval();

  if(cost.m_B > 0)
  {
    double eps = 0.000001;
    double temp = 1;
    //roots of A THETA - B log THETA + C = 0
    // <=> roots of THETA - log THETA = 1 + a
    double U = cost.m_A/cost.m_B;
    double a = -(((cost.constant - level)/cost.m_B) + log(U) + 1);

    if(a > 0)
    {
      int nb = 0;

      double leftRoot = -(1 + a);
      while(fabs(temp - leftRoot) > eps && nb < 100)
      {
        temp = leftRoot;
        leftRoot = leftRoot - 1 - (leftRoot + a)/(1 - exp(leftRoot));
        nb = nb + 1;
      }

      nb = 0;
      temp = 1;

      double rightRoot = 1 + a;
      while(fabs(temp - rightRoot) > eps && nb < 100)
      {
        temp = rightRoot;
        rightRoot = (log(rightRoot) + a)*rightRoot/(rightRoot - 1);
        nb = nb + 1;
      }

      leftRoot = exp(leftRoot)/U;
      rightRoot = rightRoot/U;

      newElement.seta(leftRoot);
      newElement.setb(rightRoot);
      if(leftRoot >= rightRoot){newElement = Interval();}
    }
  }
  else
  {
    newElement.seta(0);
    newElement.setb(level - cost.constant);
  }

  return(newElement);
}


//####### negbin #######////
Interval negbin_intervalInterRoots(const Cost& cost, double& level)
{
  Interval newElement = Interval();

  double eps = 0.000001;
  double temp = 1;
  //roots of - A log(THETA) - B log(1-THETA) + C = level
  double U = cost.m_A/(cost.m_A + cost.m_B);
  double a = level + cost.m_A*log(U) + cost.m_B*(1-U) - cost.constant;

  if(a > 0)
  {
    int nb = 0;

    double leftRoot = (cost.constant - level)/cost.m_A;
    while(fabs(temp - leftRoot) > eps && nb < 100)
    {
      temp = leftRoot;
      leftRoot = leftRoot - ((1 + exp(leftRoot))/(- cost.m_A + cost.m_B * exp(leftRoot))) * (- cost.m_A * leftRoot + (cost.m_A + cost.m_B)*log(1 + exp(leftRoot)) + cost.constant - level);
      nb = nb + 1;
    }

    nb = 0;
    temp = 1;

    double rightRoot = (level - cost.constant)/cost.m_B;
    while(fabs(temp - rightRoot) > eps && nb < 100)
    {
      temp = rightRoot;
      rightRoot = rightRoot - ((1 + exp(rightRoot))/(- cost.m_A + cost.m_B * exp(rightRoot))) * (- cost.m_A * rightRoot + (cost.m_A + cost.m_B)*log(1 + exp(rightRoot)) + cost.constant - level);
      nb = nb + 1;
    }

    leftRoot = exp(leftRoot)/(1 + exp(leftRoot));
    rightRoot = exp(rightRoot)/(1 + exp(rightRoot));

    newElement.seta(leftRoot);
    newElement.setb(rightRoot);
    if(leftRoot >= rightRoot){newElement = Interval();}
  }

  return(newElement);
}


//####### ages #######////####### ages #######////####### ages #######//
//####### ages #######////####### ages #######////####### ages #######//

int mean_age(const Cost& cost){return((int) cost.m_A);}
int variance_age(const Cost& cost){return((int) cost.m_B);}
int poisson_age(const Cost& cost){return((int) cost.m_A);}
int exp_age(const Cost& cost){return((int) cost.m_B);}
int negbin_age(const Cost& cost){return((int) cost.m_B);}


//####### factories #######////####### factories #######////####### factories #######//
//####### factories #######////####### factories #######////####### factories #######//

std::function<double*(double)> coeff_factory(const std::string& type)
{
  if(type == "mean"){return std::function<double*(double)>(mean_coeff);}
  if(type == "variance"){return std::function<double*(double)>(variance_coeff);}
  if(type == "poisson"){return std::function<double*(double)>(poisson_coeff);}
  if(type == "exp"){return std::function<double*(double)>(exp_coeff);}
  if(type == "negbin"){return std::function<double*(double)>(negbin_coeff);}
}

std::function<double(const Cost&)> min_factory(const std::string& type)
{
  if(type == "mean"){return std::function<double(const Cost&)>(mean_min);}
  if(type == "variance"){return std::function<double(const Cost&)>(variance_min);}
  if(type == "poisson"){return std::function<double(const Cost&)>(poisson_min);}
  if(type == "exp"){return std::function<double(const Cost&)>(variance_min);}
  if(type == "negbin"){return std::function<double(const Cost&)>(negbin_min);}
}

std::function<double(const Cost&)> argmin_factory(const std::string& type)
{
  if(type == "mean"){return std::function<double(const Cost&)>(mean_argmin);}
  if(type == "variance"){return std::function<double(const Cost&)>(variance_argmin);}
  if(type == "poisson"){return std::function<double(const Cost&)>(poisson_argmin);}
  if(type == "exp"){return std::function<double(const Cost&)>(poisson_argmin);}
  if(type == "negbin"){return std::function<double(const Cost&)>(negbin_argmin);}
}

std::function<Interval(const Cost&, double& level)> intervalInterRoots_factory(const std::string& type)
{
  if(type == "mean"){return std::function<Interval(const Cost&, double& level)>(mean_intervalInterRoots);}
  if(type == "variance"){return std::function<Interval(const Cost&, double& level)>(variance_intervalInterRoots);}
  if(type == "poisson"){return std::function<Interval(const Cost&, double& level)>(poisson_intervalInterRoots);}
  if(type == "exp"){return std::function<Interval(const Cost&, double& level)>(variance_intervalInterRoots);}
  if(type == "negbin"){return std::function<Interval(const Cost&, double& level)>(negbin_intervalInterRoots);}
}

std::function<int(const Cost&)> age_factory(const std::string& type)
{
  if(type == "mean"){return std::function<int(const Cost&)>(mean_age);}
  if(type == "variance"){return std::function<int(const Cost&)>(variance_age);}
  if(type == "poisson"){return std::function<int(const Cost&)>(poisson_age);}
  if(type == "exp"){return std::function<int(const Cost&)>(exp_age);}
  if(type == "negbin"){return std::function<int(const Cost&)>(negbin_age);}
}


