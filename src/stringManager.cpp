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
    mStatus = -1;
    mNoteNumber = -1;
    mVelocity = 0;
    mNumKeys = 0;
    for (int i = 0; i < 127; i++) {
        mKeyStatus[i] = false;
    }
}

void StringManager::setMidiData(MidiMan::midiMessage m)
{
    if (m.byte1 == 144 && m.byte3) {
        if(mKeyStatus[m.byte2] == false) {
            mKeyStatus[m.byte2] = true;
            mNumKeys += 1;
            mNoteNumber = m.byte2;
            mVelocity = m.byte3;
            onNoteOn(m.byte2, m.byte3);
        }
    } else {
        if(mKeyStatus[m.byte2] == true) {
            mKeyStatus[m.byte2] = false;
            mNumKeys -= 1;
            mNoteNumber = m.byte2;
            mVelocity = m.byte3;
            onNoteOff(m.byte2, m.byte3);
        }
    }
}

Guitarstring* StringManager::findFreeGitString() {
    Guitarstring* freeGitString = NULL;
    for (int i = 0; i < NumberOfGitStrings; i++) {
        if (!strings[i].isActive) {
            freeGitString = &(strings[i]);
            freeGitString->releaseString();
            cout << i << endl;
            break;
        }
    }
    return freeGitString;
}

void StringManager::onNoteOn(int noteNumber, int velocity)
{
    
    Guitarstring* gitString = findFreeGitString();
    
    if (!gitString) {
        return;
    }
    
    //gitString->reset();
    gitString->setNoteNumber(noteNumber);
    gitString->mVelocity = velocity;
    gitString->pluck(velocity);
    gitString->isActive = true;
}


void StringManager::onNoteOff(int noteNumber, int velocity)
{
    // Find the voice(s) with the given noteNumber:
    for (int i = 0; i < NumberOfGitStrings; i++) {
        Guitarstring& gitString = strings[i];
        
        if (gitString.isActive && gitString.mNoteNumber == noteNumber) {
          gitString.releaseString();
        }
    }
    
    /*
    for (int i = 0; i < NumberOfGitStrings; i++) {
        if (string[i]->isActive && string[i]->mNoteNumber == noteNumber) {
            string[i]->releaseString();
        }
    }*/
}

double StringManager::getNextSample()
{
    double yn = 0.0;
    for (int i = 0; i < NumberOfGitStrings; i++) {
        Guitarstring& gitString = strings[i];
        yn += gitString.getNextSample();
        //yn += strings[i].getNextSample();
    }
    //yn = string[0]->getNextSample(); //+ string[1]->getNextSample() + string[2]->getNextSample() + string[3]->getNextSample();
    return yn;
}
