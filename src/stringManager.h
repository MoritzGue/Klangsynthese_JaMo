/**
 * @class StringManager
 *
 *
 * @brief Manages multiple guitarstrings controlled by different incoming MIDI messages. The main purpose of this class is to identify a free waveguide for the next synthesis according to a given note number. Finally this approach realizes POLYPHONIC waveguide plucked string synthesis.
 *
 *
 *
 * @author Moritz Güldenring & Janek Newjoto
 *
 * @Contact: moritz.gueldenring@capmus.tu-berlin.de
 *
 *
 */

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
#include "TPTFilter.h"
#include "midiman.h"

using namespace std;

class StringManager
{
public:
    StringManager();
    ~StringManager();
    
    /**
     * @brief Manages Incoming MIDI Masseges
     * @param Vector from RTMidi with 3 bytes (status, note, velocity)
     */
    void setMidiData(MidiMan::midiMessage m);
    /**
     * @brief Finds a free waveguide and plays a note
     * @param MIDI note number byte
     * @param MIDI velocity byte
     */
    void onNoteOn(int noteNumber, int velocity);
    /**
     * @brief Finds a waveguide guitarstring by noteNumber and releases it
     * @param MIDI note number byte
     * @param MIDI velocity byte
     */
    void onNoteOff(int noteNumber, int velocity);
    /**
     * @brief Maps MIDI Range 0-127 to a new custom range
     * returns maped value
     */
    double mapMidiVelocity (const int velocity, const double minVal, const double maxVal);
    /**
     * @brief The audio processing
     * returns sample
     */
    inline double getNextSample();

private:

    static const int NumberOfGitStrings = 12;
    Guitarstring *strings[NumberOfGitStrings];
    TPTFilter *lpFilter;
    
    int mStatus;
    int mNoteNumber;
    int mVelocity;
    bool mKeyStatus[128];
};


inline double StringManager::getNextSample()
{
    double yn = 0.0;
    
    for (int i = 0; i < NumberOfGitStrings; i++) {
        // sum the waveguide guitarstrings and attenuate them
        yn += strings[i]->getNextSample()*0.4;
    }
    
    // TPT-LP Filter on output to have some fun!
    yn = lpFilter->nextSample(yn);
    
    return yn;
}
#endif // STRINGMANAGER_H
