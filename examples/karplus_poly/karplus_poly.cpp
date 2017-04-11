/**
 * \file
 *
 * \brief This is a simple Karplus-Strong algorithm
 *
 *
 *
 * \author Moritz Gueldenring
 *
 * \version $Revision: 0.5 $
 *
 * \date $Date: 2017/02/11$
 *
 * Contact:
 *
 */



#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>
#include <jackaudioio.hpp>


#include "../../src/guitarstring.h"
#include "../../src/midiman.h"
#include "../../src/Biquad.h"

using std::cout;
using std::endl;
using namespace std;



class KarplusSimple: public JackCpp::AudioIO {

private:
    MidiMan *midiMan;
    Guitarstring *gitString1;
    Biquad *lpFilter;

    int noteStatus;
    int noteNumber;
    int velocity;

    bool envelopeToggle;

public:
    /// Audio Callback Function:
    /// - the output buffers are filled here
    virtual int audioCallback(jack_nframes_t nframes,
                              // A vector of pointers to each input port.
                              audioBufVector inBufs,
                              // A vector of pointers to each output port.
                              audioBufVector outBufs){


        midiMan->doRtmidi();

        noteStatus = midiMan->getStatus();
        noteNumber = midiMan->getNoteNumber();
        velocity = midiMan->getVelocity();

        if (noteStatus == 144 && velocity) {
            cout << "Note Status" << noteStatus << endl;
            cout << "Velocity" << velocity << endl;
            gitString1->setPitchInHz(midiMan->getMidiNoteInHertz(noteNumber, 440.0));
            gitString1->pluck(midiMan->mapMidiVelocity(velocity,0.0,1.0));
        }

        if (noteStatus == -1){
            gitString1->releaseString();
        }


        /// LOOP over all output buffers
        for(unsigned int i = 0; i < 1; i++)
        {
          for(int frameCNT = 0; frameCNT  < nframes; frameCNT++)
          {
              outBufs[0][frameCNT] = gitString1->getNextSample();
          }

        }

        ///return 0 on success
        return 0;
    }

    /// Constructor
    KarplusSimple() :
        JackCpp::AudioIO("karplus_simple", 0,1){

        reserveInPorts(2);
        reserveOutPorts(2);

        midiMan = new MidiMan();
        envelopeToggle = true;

        /// the biquad allocate
        lpFilter = new Biquad();
        /// set initial parameters
        lpFilter->setBiquad(bq_type_lowpass, 1000.0 / 44100.0, 0.707, 0);
        gitString1 = new Guitarstring();

    }
};

///
///
///
int main(int argc, char *argv[]){


    /// initial ports from constructor created here.
    KarplusSimple * t = new KarplusSimple();

    /// activate the client
    t->start();

    /// connect sine ports to stereo ports
    t->connectToPhysical(0,0);		// connects this client out port 0 to physical destination port 0
    t->connectToPhysical(0,1);		// connects this client out port 1 to physical destination port 1

    ///print names
    cout << "outport names:" << endl;
    for(unsigned int i = 0; i < t->outPorts(); i++)
        cout << "\t" << t->getOutputPortName(i) << endl;

    /// run for EVER
    sleep(-1);

    /// never reached:!=
    t->disconnectInPort(0);	// Disconnecting ports.
    t->disconnectOutPort(1);
    t->close();	// stop client.
    delete t;	// always clean up after yourself.
    exit(0);
}
