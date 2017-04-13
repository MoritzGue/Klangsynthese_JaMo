#ifndef STRINGMANAGER
#define STRINGMANAGER

#define _USE_MATH_DEFINES

#include<vector>
#include<math.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>

#include "guitarstring.h"
#include "midimanOld.h"

using namespace std;

class StringManager
{
public:
    StringManager();

    void setMidiData(MidiMan::midiMessage m);
    void onNoteOn(int noteNumber, int velocity);
    void onNoteOff(int noteNumber, int velocity);
    double getNextSample();

private:

    static const int NumberOfGitStrings = 6;
    Guitarstring strings[NumberOfGitStrings];
    Guitarstring* findFreeGitString();
    
    int mStatus;
    int mNoteNumber;
    int mVelocity;
    int mNumKeys;
    bool mKeyStatus[128];
    
};

#endif // STRINGMANAGER_H
