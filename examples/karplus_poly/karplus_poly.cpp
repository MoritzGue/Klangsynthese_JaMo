/**
 * \file
 *
 * \brief This is a polyphonic Karplus-Strong algorithm
 *
 *
 *
 * \author Moritz Gueldenring & Janek Newjoto
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
#include "../../src/stringManager.h"
#include "../../src/Biquad.h"
#include "../../src/midimanOld.h"

using std::cout;
using std::endl;
using namespace std;



class KarplusPoly: public JackCpp::AudioIO {

private:
    
    StringManager *gitStrings;
    Biquad *lpFilter;
    MidiMan *midiMan;
    
    int noteStatus;
    int noteNumber;
    int velocity;


public:
    /// Audio Callback Function:
    /// - the output buffers are filled here
    virtual int audioCallback(jack_nframes_t nframes,
                              // A vector of pointers to each input port.
                              audioBufVector inBufs,
                              // A vector of pointers to each output port.
                              audioBufVector outBufs){


        /// LOOP over all output buffers
        for(unsigned int i = 0; i < 1; i++)
        {
          for(int frameCNT = 0; frameCNT  < nframes; frameCNT++)
          {
              processMIDI();
              
              outBufs[0][frameCNT] = gitStrings->getNextSample();
          }
 
        }

        ///return 0 on success
        return 0;
    }

    /// Constructor
    KarplusPoly() :
        JackCpp::AudioIO("karplus_poly", 0,1){

        reserveInPorts(2);
        reserveOutPorts(2);
        
        gitStrings = new StringManager();
        midiMan = new MidiMan();

        /// the biquad allocate
        lpFilter = new Biquad();
        /// set initial parameters
        lpFilter->setBiquad(bq_type_lowpass, 1000.0 / 44100.0, 0.707, 0);
        
        
            
    }
    
    void processMIDI() {
        /// process midi messages
        MidiMan::midiMessage val = midiMan->get_rtmidi();
        
        if(val.hasBeenProcessed)
        {
            gitStrings->setMidiData(val);
            
            midiMan->flushProcessedMessages();
        }
    }
};




///
///
///
int main(int argc, char *argv[]){


    /// initial ports from constructor created here.
    KarplusPoly * t = new KarplusPoly();

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
