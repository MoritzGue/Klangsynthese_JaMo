#include "retune.h"


Retune::Retune()
{
  xn_1 = 0.0;
  yn_1 = 0.0;
}

Retune::~Retune()
{
}

void Retune::setC( double f1 )
{
    cout << "Frequenz" << f1 << endl;
    int N = floor(44100/f1); // int period
    double P1 = 44100/f1; // real period
    double Ts = 1/44100;
    double Pc = P1-N-0.5; // phase delay for retune
    c_temp = (1-Pc) /(1+Pc); // filter coefficient for allpass approx
    if (c_temp >= 1){
        c_temp = 1;
    }
    else if (c_temp <= -1){
        c_temp = -1;
    }
    c = c_temp;
    cout << "Koeff" << c << endl;
    //c = (sin(2*pi*f1*Ts)-sin(2*pi*f1*Ts*Pc))/(sin(2*pi*f1*Ts*Pc+2*pi*f1*Ts)); // filter coefficient for allpass exact
}



