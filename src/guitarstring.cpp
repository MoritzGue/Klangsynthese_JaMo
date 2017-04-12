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
  oscillator1->setSampleRate(44100);

  envelopeGenerator = new EnvelopeGenerator();
  envelopeGenerator->setSampleRate(44100.0);

  loopFilter = new OneZero(-1.0);
    retuneFilter1 = new Retune();

  delayLine1 = new DelayLineSimple(44100.0, 44100);

  envelopeToggle = true;
    
    isActive = false;
}

double Guitarstring::getNextSample()
{
  if (!isActive) return 0.0;
    
    double yn = delayLine1->process(retuneFilter1->process(loopFilter->process(delayLine1->lastOut()*0.95) + (oscillator1->nextSample() * envelopeGenerator->nextSample())));
    
    return yn;
}

void Guitarstring::setNoteNumber(int noteNumber){
    
    mNoteNumber = noteNumber;
    cout << "NoteNUMBER" << mNoteNumber << endl;
};


void Guitarstring::pluck(int velocity)
{
    frequency = 440.00 * pow (2.0, (mNoteNumber - 69) / 12.0);
    
    delayLine1->setDelayInSamples(44100.0/frequency);
    retuneFilter1->setC(frequency);
    
    this->velocity = velocity;
    oscillator1->setAmplitude(velocity);
    oscillator1->setMuted(false);

    if (envelopeToggle == true){
        envelopeGenerator->enterStage(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK);
        envelopeToggle = false;
    }
}

void Guitarstring::releaseString()
{
    if (envelopeToggle == false){
        envelopeGenerator->enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
        envelopeToggle = true;
    }
    isActive = false;
    cout << "RELEASE" << endl;
}

void Guitarstring::setFree()
{
    isActive = false;
}
