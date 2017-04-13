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

using namespace std;

class Guitarstring
{
public:
    friend class StringManager;
    
    Guitarstring();

    double getNextSample();
    void setNoteNumber(int noteNumber);
    void setPitchInHz(int noteNumber);
    void pluck(int velocity);
    void releaseString();
    void reset();
    void setFree();

private:

    DelayLineSimple *delayLine1;
    OneZero *loopFilter;
    Oscillator *oscillator1;
    EnvelopeGenerator *envelopeGenerator;
    Retune *retuneFilter1;

    double velocity;
    double frequency;
    int mNoteNumber;
    int mVelocity;
    bool isActive;

};

#endif // GUITARSTRING_H
