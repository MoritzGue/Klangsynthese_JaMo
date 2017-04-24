/**
 * @class Oscillator
 *
 *
 * @Oscillator Class generating 5 signals: Sine, Square, Triangle, Sawtooth, Noise
 *
 *
 *
 * @author Moritz Güldenring & Janek Newojoto. Insprired by http://www.martin-finke.de/blog/articles/audio-plugins-008-synthesizing-waveforms/
 *
 *
 *
 */

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

public:
    /**
     * @brief General setter functions
     */
    void setMode(OscillatorMode mode);
    void setFrequency(double frequency);
    void setSampleRate(double sampleRate);
    void setAmplitude(double amplitude);
    
    inline void setMuted(bool muted) { isMuted = muted; }
    /**
     * @brief The audio processing
     * returns sample
     */
    inline double nextSample();
    
    Oscillator(OscillatorMode mode) :
    mOscillatorMode(mode),
    mPI(2*acos(0.0)),
    twoPI(2*mPI),
    isMuted(true),
    mFrequency(198.0),
    mPhase(0.0),
    mAmplitude(1.0),
    mSampleRate(44100.0) { updateIncrement(); };

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
};



//  Audio processing
inline double Oscillator::nextSample() {
    
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
