/**
 * \class Guitarstring
 *
 *
 * \brief Implements a full
 *  Karplus-Strong Algorithm plucked string for easy handling.
 *
 *
 *
 * \author Moritz Güldenring/ Janek Newjoto
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
}

double Guitarstring::getNextSample()
{
  double yn = delayLine1->process(retuneFilter1->process(loopFilter->process(delayLine1->lastOut()*0.989) + (oscillator1->nextSample() * envelopeGenerator->nextSample())));
    
    return yn;
}

void Guitarstring::setPitchInHz(double frequency){
    this->frequency = frequency;
    delayLine1->setDelayInSamples(44100.0/frequency);
    retuneFilter1->setC(frequency);
}

void Guitarstring::pluck(double velocity){
  this->velocity = velocity;
  oscillator1->setAmplitude(velocity);
  oscillator1->setMuted(false);

  if (envelopeToggle == true){
      envelopeGenerator->enterStage(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK);
      envelopeToggle = false;
  }
}

void Guitarstring::releaseString(){
  if (envelopeToggle == false){
  envelopeGenerator->enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
  envelopeToggle = true;
  }
}
