#include "retune.h"
#include <cmath>

retune::retune()
{
  xn_1 = 0.0;
  yn_1 = 0.0;
}

retune::~retune()
{
}

void retune::setC( double f1 )
{
    int N = floor(44100/f1); // int period
    double P1 = 44100/f1; // real period
    double Ts = 1/44100;
    double Pc = P1-N-0.5; // phase delay for retune
    c = (1-Pc) /(1+Pc); // filter coefficient for allpass approx
    //c = (sin(2*pi*f1*Ts)-sin(2*pi*f1*Ts*Pc))/(sin(2*pi*f1*Ts*Pc+2*pi*f1*Ts)); // filter coefficient for allpass exact
}



