/**
 * @class Guitarstring
 *
 *
 * @brief Implements a plucked string interpolating Karplus-Strong Algorithm
 *
 *
 *
 * @author Moritz Güldenring & Janek Newjoto
 *
 *
 * @Contact: moritz.gueldenring@capmus.tu-berlin.de
 *
 *
 */

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
#include "envelope.h"

using namespace std;

class Guitarstring
{
public:
    friend class StringManager;
    
    Guitarstring();
    ~Guitarstring();
    
    /**
     * Setter functions
     */
    void setNoteNumber(int noteNumber){mNoteNumber = noteNumber;};
    void setDampGain(double gain){dampGain = gain;};
    
    /**
     * @brief This function should be called on "Note-ON". Prepares the waveguide
     * synthesis and hits a note "plucks a guitarstring"
     * @param MIDI velocity byte
     */
    void pluck(int velocity);
    /**
     * @brief Extends or reduces the delay length for bending/ guitar tremolo fx
     * @param pitch value in HZ
     */
    void bendString(double bendValue);
    /**
     * @brief Sets the oscillatiors singnal type
     * @param MIDI CC
     */
    void setOscillator(int controlValue1);
    /**
     * @brief Sets the envelopes shape type
     * @param MIDI CC
     */
    void setEnvelopeShape(int controlValue2);
    /**
     * @brief Sets the envelope duration
     * @param Duration in seconds (should be very short)
     */
    void setEnvelopeDuration(double durValue);
    /**
     * @brief This function should be called on "Note-OFF".
     * Starts a short fade out and releases the guitarstring
     * @param MIDI velocity byte
     */
    void releaseString();
    
    void reset();
    void setFree();
    /**
     * @brief The audio processing
     * returns sample
     */
    inline double getNextSample();

private:

    DelayLineSimple *delayLine1;
    OneZero *loopFilter;
    Oscillator *oscillator1;
	Envelope *envelopeGenerator;

    double velocity;
    double frequency;
    double dampGain;
    double fadeGain;
    
    int mNoteNumber;
    int mVelocity;
    
    bool isActive;
    bool fadeOut;
};


//Process
inline double Guitarstring::getNextSample(){

    if (!isActive) return 0.0;

    // Extended Karlpus Strong Algorithm as introduced by J.O.Smith
    double yn = delayLine1->process(loopFilter->process(delayLine1->lastOut()*dampGain + (oscillator1->nextSample() * envelopeGenerator->nextSample())));
    
    // Short fade out when releaseString() is called to avoid note-off crackling
    if (fadeOut == true){
        yn = yn * fadeGain;
        fadeGain = fadeGain - 0.001;
        if(fadeGain <= 0.0){
            isActive = false;
            fadeOut = false;
        }
    }
    return yn;
}
#endif // GUITARSTRING_H
