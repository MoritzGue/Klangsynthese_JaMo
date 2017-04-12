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

void StringManager::setMidiData(int mStatus,int noteNumber,int velocity)
{
    if (mStatus == 144 && velocity) {
        if(mKeyStatus[noteNumber] == false) {
            mKeyStatus[noteNumber] = true;
            mNumKeys += 1;
            mNoteNumber = noteNumber;
            mVelocity = velocity;
            onNoteOn(noteNumber, velocity);
        }
    } else {
        if(mKeyStatus[noteNumber] == true) {
            mKeyStatus[noteNumber] = false;
            mNumKeys -= 1;
            mNoteNumber = noteNumber;
            mVelocity = velocity;
            onNoteOff(noteNumber, velocity);
        }
    }
}



Guitarstring* StringManager::findFreeGitString() {
    Guitarstring* freeGitString = NULL;
    for (int i = 0; i < NumberOfGitStrings; i++) {
        if (!strings[i].isActive) {
            freeGitString = &(strings[i]);
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
}

double StringManager::getNextSample()
{
    double yn = 0.0;
    for (int i = 0; i < NumberOfGitStrings; i++) {
        Guitarstring& gitString = strings[i];
        yn += gitString.getNextSample();
    }
    return yn;
}
