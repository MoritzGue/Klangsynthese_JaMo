

#define _USE_MATH_DEFINES

#include<vector>
#include<math.h>
#include <cstdlib>
#include <iostream>

using namespace std;

typedef enum {
    OSCILLATOR_MODE_SINE = 0,
    OSCILLATOR_MODE_SAW,
    OSCILLATOR_MODE_SQUARE,
    OSCILLATOR_MODE_TRIANGLE,
    OSCILLATOR_MODE_NOISE,
    kNumOscillatorModes
}OscillatorMode;

class Oscillator {

private:

    OscillatorMode mOscillatorMode;
    const double mPI;
    const double twoPI;
    bool isMuted;
    double mFrequency;
    double mPhase;
    double mAmplitude;
    double mSampleRate;
    double mPhaseIncrement;
    void updateIncrement();

public:

    void setMode(OscillatorMode mode);
    void setFrequency(double frequency);
    void setSampleRate(double sampleRate);
    void setAmplitude(double amplitude);
    inline void setMuted(bool muted) { isMuted = muted; }
    double nextSample();

    Oscillator(OscillatorMode mode) :
    mOscillatorMode(mode),
    mPI(2*acos(0.0)),
    twoPI(2*mPI),
    isMuted(true),
    mFrequency(440.0),
    mPhase(0.0),
    mAmplitude(1.0),
    mSampleRate(44100.0) { updateIncrement(); };
};
