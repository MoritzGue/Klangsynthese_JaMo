#include "stringManager.h"

StringManager::StringManager(){
    
    // the waveguide guitarstrings allocate
    for (int i = 0; i < NumberOfGitStrings; i++) {
        strings[i] = new Guitarstring();
    }
    
    // the TPTFilter allocate
    lpFilter = new TPTFilter();
    // set initial parameters
    lpFilter->setTPTFilter(bq_type_lowpass, 1000.0 , 0.707, 44100.0);
    
    mStatus = -1;
    mNoteNumber = -1;
    mVelocity = 0;
    for (int i = 0; i < 127; i++) {
        mKeyStatus[i] = false;
    }
}

StringManager::~StringManager(){
}

void StringManager::setMidiData(MidiMan::midiMessage m){
    
    // Note number management
    if (m.byte1 == 144 && m.byte3) {
        if(mKeyStatus[m.byte2] == false) {
            mKeyStatus[m.byte2] = true;
            mNoteNumber = m.byte2;
            mVelocity = m.byte3;
            onNoteOn(m.byte2, m.byte3);
        }
    } else {
        if(mKeyStatus[m.byte2] == true) {
            mKeyStatus[m.byte2] = false;
            mNoteNumber = m.byte2;
            mVelocity = m.byte3;
            onNoteOff(m.byte2, m.byte3);
        }
    }
    
    // CC management
    switch (m.byte1) {
        case 176:
            switch (m.byte2) {
                case 1: //String bending
                {
                    
                }
                    break;
                case 2: //feedback Gain Value
                {
                    double damping = mapMidiVelocity((double)m.byte3, 0.7,1.0);
                    cout << "DAMPGAIN" << damping << endl;
                    for(int i = 0; i < NumberOfGitStrings; i++) {
                        strings[i]->setDampGain(damping);
                    }
                }
                    break;
                case 3: // Sets the envelope shape (rect, tri, half-sine, hanning)
                {
                    for(int i = 0; i < NumberOfGitStrings; i++) {
                        strings[i]->setEnvelopeShape(m.byte3);
                    }
                }
                    break;
                case 4: // Sets the envelope duration
                {
                    double duration = mapMidiVelocity((double)m.byte3, 0.001,0.25);
                    cout << "ENV DURATION" << duration << endl;
                    for(int i = 0; i < NumberOfGitStrings; i++) {
                        strings[i]->setEnvelopeDuration(duration);
                    }
                }
                    break;
                case 5: // Sets oscillator mode (Sin, Saw, Square, Tri, Noise)
                {
                    for(int i = 0; i < NumberOfGitStrings; i++) {
                        strings[i]->setOscillator(m.byte3);
                    }
                }
                    break;
                case 7: //Sets TPT filter cut-off
                {
                    double Fc = mapMidiVelocity((double)m.byte3, 50.0,5000.0);
                    cout << "CUT-OFF" << Fc << endl;
                    for(int i = 0; i < NumberOfGitStrings; i++) {
                        lpFilter->setFc(Fc);
                        if (m.byte3 == 127) lpFilter->setFc(20000.0);
                    }
                }
                    break;
                case 8: //Sets TPT filter Q
                {
                    double Q = mapMidiVelocity((double)m.byte3, 0.707,3.0);
                    cout << "RESONANCE" << Q << endl;
                    for(int i = 0; i < NumberOfGitStrings; i++) {
                        lpFilter->setQ(Q);
                    }
                }
                    break;
                case 9: //Sustain
                {
                    if (m.byte3 > 0){
                        cout << "SUSTAIN ON" << endl;
                        for(int i = 0; i < NumberOfGitStrings; i++) {
                            strings[i]->sustain = true;
                        }
                    }
                    else{
                        for(int i = 0; i < NumberOfGitStrings; i++) {
                            strings[i]->sustain = false;
                        }
                        cout << "SUSTAIN OFF" << endl;
                    }
                }
                    break;
                case 10: //Release all Notes
                {
                    for (int i = 0; i < NumberOfGitStrings; i++) {
                        strings[i]->releaseString();
                    }
                }
                    break;
                case 17: //String bending
                {
                    double bending = mapMidiVelocity((double)m.byte3, -15.0,15.0);
                    cout << "BEND" << bending << endl;
                    for(int i = 0; i < NumberOfGitStrings; i++) {
                        strings[i]->bendString(bending);
                    }
                }
            
                default:
                    break;
            }
            break;

        default:
            break;
    }
}


void StringManager::onNoteOn(int noteNumber, int velocity){
    
    // Find a free guitarstring
    for (int i = 0; i < NumberOfGitStrings; i++) {
        
        if (strings[i]->sustain == true && strings[i]->mNoteNumber == noteNumber)
            strings[i]->isActive = false;
            
        
        if (!strings[i]->isActive) {
            strings[i]->setNoteNumber(noteNumber);
            strings[i]->mVelocity = velocity;
            strings[i]->pluck(velocity);
            strings[i]->isActive = true;
            break;
        }
    }
}


void StringManager::onNoteOff(int noteNumber, int velocity){
    
    // Find the guitarstring(s) with the given noteNumber and release it
    for (int i = 0; i < NumberOfGitStrings; i++) {
        if (strings[i]->isActive && strings[i]->mNoteNumber == noteNumber) {
            strings[i]->releaseString();
        }
    }
}

double StringManager::mapMidiVelocity (const int velocity, const double minVal, const double maxVal){
    
    return ((double)velocity - 0.0) * (maxVal - minVal) / (127.0 - 0.0) + minVal;
}

