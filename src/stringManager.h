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
    double mapMidiVelocity (const int velocity, const double minVal, const double maxVal);
    inline double getNextSample();

private:

    static const int NumberOfGitStrings = 12;
    Guitarstring *strings[NumberOfGitStrings];
    //Guitarstring* findFreeGitString();
    
    int mStatus;
    int mNoteNumber;
    int mVelocity;
    bool mKeyStatus[128];
    
};
inline double StringManager::getNextSample()
{
    double yn = 0.0;
    for (int i = 0; i < NumberOfGitStrings; i++) {
        //Guitarstring& gitString = strings[i];
        //yn += gitString.getNextSample();
        yn += strings[i]->getNextSample();
    }
    
    //yn = strings[0]->getNextSample() + strings[1]->getNextSample() + strings[2]->getNextSample() + strings[3]->getNextSample() + strings[4]->getNextSample() + strings[5]->getNextSample() + strings[6]->getNextSample();

    return yn;
}
#endif // STRINGMANAGER_H
