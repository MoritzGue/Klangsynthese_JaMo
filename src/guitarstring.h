#ifndef GUITARSTRING_H
#define GUITARSTRING_H

#define _USE_MATH_DEFINES

#include<vector>
#include<math.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>

#include "DelayLineSimple.h"
#include "OneZero.h"
#include "oscillator.h"
#include "adsr.h"
#include "retune.h"

class Guitarstring
{
public:
    Guitarstring();

    double getNextSample();
    void setPitchInHz(double frequeny);
    void pluck(double velocity);
    void releaseString();


private:

  DelayLineSimple *delayLine1;
  OneZero *loopFilter;
  Oscillator *oscillator1;
  EnvelopeGenerator *envelopeGenerator;
    Retune *retuneFilter1;

  bool envelopeToggle;
  double velocity;
    double frequency;

};

#endif // GUITARSTRING_H
