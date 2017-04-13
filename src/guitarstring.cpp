/**
 * \class Guitarstring
 *
 *
 * \brief Implements a full
 *  Karplus-Strong Algorithm plucked string for easy handling.
 *
 *
 *
 * \author Moritz Güldenring & Janek Newjoto
 *
 * \version
 *
 * \date
 *
 * Contact:
 *
 *
 */

#include "guitarstring.h"

Guitarstring::Guitarstring()
{
    oscillator1 = new Oscillator(OSCILLATOR_MODE_NOISE);
    oscillator1->setSampleRate(44100.0);

    envelopeGenerator = new EnvelopeGenerator();
    envelopeGenerator->setSampleRate(44100.0);

    loopFilter = new OneZero(-1.0);
    retuneFilter1 = new Retune();

    delayLine1 = new DelayLineSimple(44100.0, 1);
    delayLine1->resetDelay();
    
    isActive = false;
}

double Guitarstring::getNextSample()
{
    if (!isActive) return 0.0;
    
    //double yn = delayLine1->process(retuneFilter1->process(loopFilter->process(delayLine1->lastOut()*0.95) + (oscillator1->nextSample() * envelopeGenerator->nextSample())));
    
    double yn = delayLine1->process(loopFilter->process(delayLine1->lastOut()*0.989 + (oscillator1->nextSample() * envelopeGenerator->nextSample())));
    //cout << yn << endl;
    //if (yn<abs(0.001)) isActive = false;
    return yn;
}

void Guitarstring::setNoteNumber(int noteNumber){
    
    mNoteNumber = noteNumber;
    
};


void Guitarstring::pluck(int velocity)
{
    frequency = 440.00 * pow (2.0, (mNoteNumber - 69) / 12.0);
    
    delayLine1->setDelayInSamples(44100.0/frequency);
    retuneFilter1->setC(frequency);
    
    this->velocity = velocity;
    oscillator1->setAmplitude((double)velocity/127.0);
    oscillator1->setMuted(false);

    envelopeGenerator->enterStage(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK);
}

void Guitarstring::releaseString()
{
    envelopeGenerator->enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
    delayLine1->resetDelay();
    isActive = false;
    cout << "RELEASE" << endl;
}

void Guitarstring::reset()
{
    oscillator1->setMuted(true);
    delayLine1->resetDelay();
}

void Guitarstring::setFree()
{
    isActive = false;
}
