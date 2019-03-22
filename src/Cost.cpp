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

void addmyConstant(Cost& cost, double& cst)
{
  cost.constant = cost.constant + cst;
}

void addCost(Cost& cost, const Cost& cost2)
{
  cost.m_A = cost.m_A + cost2.m_A;
  cost.m_B = cost.m_B + cost2.m_B;
  cost.constant = cost.constant + cost2.constant;
}

double log_factorial(double n)
{
  double res = 0;
  for(int i = 2; i < floor(n) + 1; i++){res = res + log(i);}
  return(res);
}

//####### coefficients #######////####### coefficients #######////####### coefficients #######//
//####### coefficients #######////####### coefficients #######////####### coefficients #######//

///mean cost = m_A*THETA^2 + m_B*log(THETA) + constant
///variance cost = m_A*THETA - m_B*log(THETA) + constant
/// poisson cost = m_A*THETA - m_B*log(THETA) + constant
/// exp cost = m_A*THETA - m_B*log(THETA) + constant
/// rbinom cost = m_A*THETA - m_B*log(THETA) + constant


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
  coeff[2] = log(2*M_PI);
  return(coeff);
}

double* poisson_coeff(double y)
{
  double* coeff = new double[3];
  coeff[0] = 1;
  coeff[1] = y;
  coeff[2] = log_factorial(y);
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

double* nbinom_coeff(double y)
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
double variance_min(const Cost& cost)
{
  double res = INFINITY; ///if constant
  if(cost.m_A > pow(10,-12) && cost.m_B != 0){res = cost.m_B - cost.m_B*log(cost.m_B/cost.m_A) + cost.constant;}
    else
    {
      if(cost.m_A < pow(10,-12) &&  cost.m_B != 0){res = cost.m_B - cost.m_B*log(cost.m_B/pow(10,-12)) + cost.constant;}
    }
  return(res);
}

double poisson_min(const Cost& cost)
{
  double res = INFINITY;
  if(cost.m_A == 0 && cost.m_B != 0){res = cost.constant;}
  if(cost.m_A != 0 && cost.m_B != 0){res = cost.m_B - cost.m_B*log(cost.m_B/cost.m_A) + cost.constant;}
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


Interval mean_intervalInterRoots(const Cost& cost, double& level)
{
  Interval newElement = Interval();
  double Delta = cost.m_B*cost.m_B - 4*cost.m_A*(cost.constant - level);

  if(Delta > 0)
  {
    double R = sqrt(Delta);
    newElement = Interval((-cost.m_B-R)/(2*cost.m_A), (-cost.m_B+R)/(2*cost.m_A));
  }

  return(newElement);
}

Interval variance_intervalInterRoots(const Cost& cost, double& level)
{
  Interval newElement = Interval();

  //std::cout << "variance_intervalInterRoots" << std::endl;
  //precision =
  double eps = 0.0001;
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
  //std::cout << newElement.geta() << " --- " << newElement.getb() << std::endl;
  return(newElement);
}


int mean_age(const Cost& cost){return((int) cost.m_A);}
int variance_age(const Cost& cost){return((int) cost.m_B);}
int poisson_age(const Cost& cost){return((int) cost.m_A);}
int exp_age(const Cost& cost){return((int) cost.m_B);}


//####### factories #######////####### factories #######////####### factories #######//
//####### factories #######////####### factories #######////####### factories #######//

std::function<double*(double)> coeff_factory(const std::string& type)
{
  if(type == "mean"){return std::function<double*(double)>(mean_coeff);}
  if(type == "variance"){return std::function<double*(double)>(variance_coeff);}
  if(type == "poisson"){return std::function<double*(double)>(poisson_coeff);}
  if(type == "exp"){return std::function<double*(double)>(exp_coeff);}
}

std::function<double(const Cost&)> min_factory(const std::string& type)
{
  if(type == "mean"){return std::function<double(const Cost&)>(mean_min);}
  if(type == "variance"){return std::function<double(const Cost&)>(variance_min);}
  if(type == "poisson"){return std::function<double(const Cost&)>(poisson_min);}
  if(type == "exp"){return std::function<double(const Cost&)>(variance_min);}
}

std::function<double(const Cost&)> argmin_factory(const std::string& type)
{
  if(type == "mean"){return std::function<double(const Cost&)>(mean_argmin);}
  if(type == "variance"){return std::function<double(const Cost&)>(variance_argmin);}
  if(type == "poisson"){return std::function<double(const Cost&)>(poisson_argmin);}
  if(type == "exp"){return std::function<double(const Cost&)>(poisson_argmin);}
}

std::function<Interval(const Cost&, double& level)> intervalInterRoots_factory(const std::string& type)
{
  if(type == "mean"){return std::function<Interval(const Cost&, double& level)>(mean_intervalInterRoots);}
  if(type == "variance"){return std::function<Interval(const Cost&, double& level)>(variance_intervalInterRoots);}
  if(type == "poisson"){return std::function<Interval(const Cost&, double& level)>(variance_intervalInterRoots);}
  if(type == "exp"){return std::function<Interval(const Cost&, double& level)>(variance_intervalInterRoots);}
}

std::function<int(const Cost&)> age_factory(const std::string& type)
{
  if(type == "mean"){return std::function<int(const Cost&)>(mean_age);}
  if(type == "variance"){return std::function<int(const Cost&)>(variance_age);}
  if(type == "poisson"){return std::function<int(const Cost&)>(poisson_age);}
  if(type == "exp"){return std::function<int(const Cost&)>(exp_age);}
}


