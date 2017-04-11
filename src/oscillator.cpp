/**
 * \class Oscillator
 *
 *
 * \Oscillator Class with 4 generator Signals: Sine, Square, Triangle, Sawtooth
 *
 *
 *
 * \author Moritz Güldenring insprired by http://www.martin-finke.de/blog/articles/audio-plugins-008-synthesizing-waveforms/
 *
 * \version $Revision: 0.5 $
 *
 * \date $Date: 2016/08/01 14:16:20 $
 *
 *
 *
 *
 */

#include "oscillator.h"

void Oscillator::setMode(OscillatorMode mode) {
    mOscillatorMode = mode;
}

void Oscillator::setFrequency(double frequency) {
    mFrequency = frequency;
    updateIncrement();
}

void Oscillator::setSampleRate(double sampleRate) {
    mSampleRate = sampleRate;
    updateIncrement();
}

void Oscillator::setAmplitude(double amplitude) {
    mAmplitude = amplitude;
    updateIncrement();
}

void Oscillator::updateIncrement() {
    mPhaseIncrement = mFrequency * 2 * mPI / mSampleRate;
}

double Oscillator::nextSample() {
    double oscVal = 0;
    if (isMuted) return oscVal;

    switch (mOscillatorMode) {

        case OSCILLATOR_MODE_SINE:
            oscVal = sin(mPhase);
            oscVal *= mAmplitude;

            break;

        case OSCILLATOR_MODE_SAW:
            oscVal = 1.0 - (2.0 * mPhase / twoPI);
            oscVal *= mAmplitude;
            break;

        case OSCILLATOR_MODE_SQUARE:
            if (mPhase <= mPI) {
                oscVal = 1.0;
            } else {
                oscVal = -1.0;
            }
            oscVal *= mAmplitude;
            break;

        case OSCILLATOR_MODE_TRIANGLE:
            oscVal = -1.0 + (2.0 * mPhase / twoPI);
            oscVal = 2.0 * (fabs(oscVal) - 0.5);
            oscVal *= mAmplitude;
            break;
        case OSCILLATOR_MODE_NOISE:
            double R1 = (float) rand() / (float) RAND_MAX;
            double R2 = (float) rand() / (float) RAND_MAX;
            
            oscVal = ((float) sqrt( -2.0f * log( R1 )) * cos( 2.0f * mPI * R2 )) * mAmplitude;

    }
    mPhase += mPhaseIncrement;
    while (mPhase >= twoPI) {
        mPhase -= twoPI;
    }
    return oscVal;
}
