#include "midiman.h"

bool MidiMan::done;

MidiMan::MidiMan()
{
    // rtmidid intit
    midiin = new RtMidiIn(RtMidiIn::Api::UNSPECIFIED ,std::string("RtMidi Input Client"),(unsigned int) 100);

    //
    unsigned int nPorts = midiin->getPortCount();

    midiin->openPort( 0 );
    //    // Don't ignore sysex, timing, or active sensing messages.
    midiin->ignoreTypes( false, false, false );
    done = false;
    
    mStatus = -1;
    mNoteNumber = -1;
    mVelocity = 0;
    mNumKeys = 0;
    for (int i = 0; i < 127; i++) {
        mKeyStatus[i] = false;
    }
}

MidiMan::~MidiMan()
{
    delete midiin;
}


void MidiMan::setVerbose()
{
    isVerbose = true;
}

void MidiMan::doRtmidi()
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
        
        // There are only note on/off messages in the queue
        if (mStatus == 144 && velocity) {
            if(mKeyStatus[noteNumber] == false) {
                mKeyStatus[noteNumber] = true;
                mNumKeys += 1;
            }
            // A key pressed later overrides any previously pressed key:
            if (noteNumber != mNoteNumber) {
                mNoteNumber = noteNumber;
                mVelocity = velocity;
            }
        } else {
            if(mKeyStatus[noteNumber] == true) {
                mKeyStatus[noteNumber] = false;
                mNumKeys -= 1;
            }
            // If the last note was released, nothing should play:
            if (noteNumber == mNoteNumber) {
                mStatus = -1;
                mNoteNumber = -1;
                mVelocity = 0;
            }
            
        /*if(a[0] > 0){
            break;
        }
        //mStatus = -1;
        //mNoteNumber = -1;
        //mVelocity = 0;*/
        }
    }
}




//////////////////////////////////////////////////////////////////
// FLUSH
//////////////////////////////////////////////////////////////////

void MidiMan::flushProcessedMessages()
{

    val.clear();


}


int MidiMan::getNumFaderMessages()
{

    int v  = (int) val.size();

    return v;
}

double MidiMan::getMidiNoteInHertz (const int noteNumber, const double frequencyOfA)
{
    return frequencyOfA * pow (2.0, (noteNumber - 69) / 12.0);
}

double MidiMan::mapMidiVelocity (const int velocity, const double minVal, const double maxVal)
{
    return ((double)velocity - 0.0) * (maxVal - minVal) / (127.0 - 0.0) + minVal;
}
