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

