#include "guitarstring.h"

Guitarstring::Guitarstring(){
    
    isActive = false;
    fadeOut = false;
    
    oscillator1 = new Oscillator(OSCILLATOR_MODE_NOISE);
    oscillator1->setSampleRate(44100.0);

	envelopeGenerator = new Envelope();
	envelopeGenerator->setSampleRate(44100.0);

    loopFilter = new OneZero(-1.0);

    delayLine1 = new DelayLineSimple(44100.0, 5000);
    delayLine1->resetDelay();
    
    dampGain = 0.99;
    fadeGain = 0.9;
}

Guitarstring::~Guitarstring(){
}

void Guitarstring::pluck(int velocity){
    
    // Calculate the frequency from given MIDI note number
    frequency = 440.00 * pow (2.0, (mNoteNumber - 69) / 12.0);
    
    // Reset the waveguides delay
    delayLine1->resetDelay();
    // Set the waveguides delay line according to the desired frequency
    delayLine1->setDelayInSamples(44100.0/frequency - 1.0);
    //cout << "FREQ" << frequency << endl;
    
    // Set the oscillators amplitude
    this->velocity = velocity;
    oscillator1->setAmplitude((double)velocity/127.0);
    oscillator1->setMuted(false);
    
    // reset the fade out gain to 0.9
    fadeGain = 0.9;
    fadeOut = false;
    
    // Start the envelope "pluck"
	envelopeGenerator->setEnvSwitchOn();
}

void Guitarstring::bendString(double bendValue){
    
    // add some pitch (in HZ) to the actual notes frequency
    double f = frequency + bendValue;
    // adjust the delay length
    delayLine1->setDelayInSamples(44100.0/f - 1.0);
}

void Guitarstring::setOscillator(int controlValue1){
    
    switch (controlValue1) {
        case 0:
            oscillator1->setMode(OSCILLATOR_MODE_SINE);
            cout << "SINE" << endl;
            break;
        case 31:
            oscillator1->setMode(OSCILLATOR_MODE_SAW);
            cout << "SAW" << endl;
            break;
        case 63:
            oscillator1->setMode(OSCILLATOR_MODE_SQUARE);
            cout << "SQUARE" << endl;
            break;
        case 96:
            oscillator1->setMode(OSCILLATOR_MODE_TRIANGLE);
            cout << "TRI" << endl;
            break;
        case 127:
            oscillator1->setMode(OSCILLATOR_MODE_NOISE);
            cout << "NOISE" << endl;
            break;
        default:
            break;
    }
}

void Guitarstring::setEnvelopeShape(int controlValue2){
    
    switch (controlValue2) {
        case 0:
            envelopeGenerator->setEnvShape(ENVELOPE_SHAPE_RECT);
            cout << "RECT SHAPE" << endl;
            break;
        case 64:
            envelopeGenerator->setEnvShape(ENVELOPE_SHAPE_TRI);
            cout << "TRIANGLE SHAPE" << endl;
            break;
        case 127:
            envelopeGenerator->setEnvShape(ENVELOPE_SHAPE_SIN);
            cout << "ROUND SHAPE" << endl;
            break;
        default:
            break;
    }
}

void Guitarstring::setEnvelopeDuration(double durValue){
    
    envelopeGenerator->setEnvDuration(durValue);
}

void Guitarstring::releaseString(){
    
    fadeOut = true;
    //delayLine1->resetDelay();
    //cout << "RELEASE" << fadeOut << endl;
}

void Guitarstring::reset(){

    oscillator1->setMuted(true);
    delayLine1->resetDelay();
}

void Guitarstring::setFree(){
    
    isActive = false;
}
