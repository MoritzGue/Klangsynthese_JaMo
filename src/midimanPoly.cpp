#include "midimanPoly.h"


MidiManPoly::MidiManPoly()
{
    // rtmidid intit
    midiin = new RtMidiIn(RtMidiIn::Api::UNSPECIFIED ,std::string("RtMidi Input Client"),(unsigned int) 100);

    //
    unsigned int nPorts = midiin->getPortCount();

    midiin->openPort( 0 );
    //    // Don't ignore sysex, timing, or active sensing messages.
    midiin->ignoreTypes( false, false, false );

    
    mStatus = -1;
    noteNumber = -1;
    velocity = 0;
}

MidiManPoly::~MidiManPoly()
{
    delete midiin;
}


void MidiManPoly::setVerbose()
{
    isVerbose = true;
}

void MidiManPoly::doRtmidi()
{
    std::vector<unsigned char>  a;

    int nBytes = 1;

    /// this is kind of a dirty workaroud -
    /// we loop over all in-massages and take the last one:
    while(nBytes>0)
    {
        midiin->getMessage(&a);
        nBytes = a.size();
        /// only do something if bytes are received
        if(nBytes!=0)
        {
            /// only give feedback if 'verbose-mode' is active
            if(isVerbose == true  )
            {
                std::cout << "received " << nBytes << "Bytes: " ;
                for (int i=0; i<nBytes; i++ )
                    std::cout <<  i << " = " << (int)a[i] << " -- " ;
                std::cout <<  std::endl;
            }
            mStatus = a[0];
            noteNumber = a[1];
            velocity = a[2];
       }
    }
}

//////////////////////////////////////////////////////////////////
// FLUSH
//////////////////////////////////////////////////////////////////

void MidiManPoly::flushProcessedMessages()
{
    val.clear();
}


int MidiManPoly::getNumFaderMessages()
{
    int v  = (int) val.size();
    return v;
}

double MidiManPoly::getMidiNoteInHertz (const int noteNumber, const double frequencyOfA)
{
    return frequencyOfA * pow (2.0, (noteNumber - 69) / 12.0);
}

double MidiManPoly::mapMidiVelocity (const int velocity, const double minVal, const double maxVal)
{
    return ((double)velocity - 0.0) * (maxVal - minVal) / (127.0 - 0.0) + minVal;
}