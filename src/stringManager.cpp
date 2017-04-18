/**
 * \class StrinManager
 *
 *
 * \brief Implements a full
 *  Manages multiple Guitarstrings controlled by MIDI inputs. Up to 6 Guitarstrings are possible.
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

#include "stringManager.h"

StringManager::StringManager()
{
    for (int i = 0; i < NumberOfGitStrings; i++) {
        strings[i] = new Guitarstring();
    }
    
    mStatus = -1;
    mNoteNumber = -1;
    mVelocity = 0;
    for (int i = 0; i < 127; i++) {
        mKeyStatus[i] = false;
    }
}

void StringManager::setMidiData(MidiMan::midiMessage m)
{
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
    
    switch (m.byte1) {
        case 176:
            switch (m.byte2) {
                case 1: //Feedback Gain Value 0.5-1.0
                {
                    double bending = mapMidiVelocity((double)m.byte3, -15.0,15.0);
                    cout << "BEND" << bending << endl;
                    for(int i = 0; i < NumberOfGitStrings; i++) {
                        strings[i]->bendString(bending);
                    }
                }
                    break;
                case 2: //Feedback Gain Value 0.5-1.0
                {
                    double damping = mapMidiVelocity((double)m.byte3, 0.5,0.999);
                    cout << "DAMPGAIN" << damping << endl;
                    for(int i = 0; i < NumberOfGitStrings; i++) {
                        strings[i]->setDampGain(damping);
                    }
                }
                    break;
                case 5: //Oscillator Mode
                {
                    for(int i = 0; i < NumberOfGitStrings; i++) {
                        strings[i]->setOscillator(m.byte3);
                    }
                }
                    break;
            
                default:
                    break;
            }
            break;
            
        default:
            break;
    }
}

/*Guitarstring* StringManager::findFreeGitString() {
    Guitarstring* freeGitString = NULL;
    for (int i = 0; i < NumberOfGitStrings; i++) {
        if (!strings[i]->isActive) {
            freeGitString = strings[i];//&(strings[i]);
            freeGitString->releaseString();
            //cout << i << endl;
            break;
        }
    }
    return freeGitString;
}*/

void StringManager::onNoteOn(int noteNumber, int velocity)
{
    
    /*Guitarstring* gitString = findFreeGitString();
    
    if (!gitString) {
        return;
    }
    
    //gitString->releaseString();
    gitString->setNoteNumber(noteNumber);
    gitString->mVelocity = velocity;
    gitString->pluck(velocity);
    gitString->isActive = true;*/
    
    for (int i = 0; i < NumberOfGitStrings; i++) {
        if (!strings[i]->isActive) {
            strings[i]->setNoteNumber(noteNumber);
            strings[i]->mVelocity = velocity;
            strings[i]->pluck(velocity);
            strings[i]->isActive = true;
            break;
        }
    }
}


void StringManager::onNoteOff(int noteNumber, int velocity)
{
    /*// Find the voice(s) with the given noteNumber:
    for (int i = 0; i < NumberOfGitStrings; i++) {
        Guitarstring& gitString = strings[i];
        
        if (gitString.isActive && gitString.mNoteNumber == noteNumber) {
          gitString.releaseString();
        }
    }*/
    
    
    for (int i = 0; i < NumberOfGitStrings; i++) {
        if (strings[i]->isActive && strings[i]->mNoteNumber == noteNumber) {
            strings[i]->releaseString();
        }
    }
}

double StringManager::mapMidiVelocity (const int velocity, const double minVal, const double maxVal)
{
    return ((double)velocity - 0.0) * (maxVal - minVal) / (127.0 - 0.0) + minVal;
}
