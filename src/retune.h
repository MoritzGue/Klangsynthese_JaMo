#ifndef RETUNE_H
#define RETUNE_H
#include <iostream>
#include <stdlib.h>
#include <cmath>

using std::cout;
using std::endl;
using namespace std;


class Retune
{

 public:

  //! The default constructor creates an allpass filter.
  Retune();

  //! Class destructor.
  ~Retune();

  void setC(double f1);
    double getC(){return c;};

    double process(double xn);
 
 private:
    
  double f1;
  double c;
    double c_temp;
  double xn_1;
  double yn_1;
  //const double pi = 3.1415926535897;

};

inline double Retune::process(double xn)
{
    //double xn = input;
    double yn = (c * xn) + xn_1 - (c * yn_1);
    xn_1 = xn;
    yn_1 = yn;
    return yn;
}
#endif
