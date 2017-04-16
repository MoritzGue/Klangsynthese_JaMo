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
#include "DelayA.h"
#include "OneZero.h"
#include "oscillator.h"
#include "adsr.h"

using namespace std;

class Guitarstring
{
public:
    friend class StringManager;
    
    Guitarstring();

    inline double getNextSample();
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

    double velocity;
    double frequency;
    int mNoteNumber;
    int mVelocity;
    bool isActive;

};


//Process
inline double Guitarstring::getNextSample()
{
    if (!isActive) return 0.0;

    
    double yn = delayLine1->process(loopFilter->process(delayLine1->lastOut()*0.989 + (oscillator1->nextSample() * envelopeGenerator->nextSample())));
    
    //double yn = delayLine1->process(delayLine1->lastOut()*0.989 + (oscillator1->nextSample() * envelopeGenerator->nextSample()));
    
    //cout << yn << endl;
    return yn;
}
#endif // GUITARSTRING_H
