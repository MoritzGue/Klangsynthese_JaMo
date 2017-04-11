/**
* \class JackSampler
*
* \brief Host for the biquad implementation from "earlevel.com"!
*
* \author Henrik von Coler
*
* \version $Revision: 0.5 $
*
* \date $Date: 2016/08/14 14:16:20 $
*
* \todo Needs interpolation of parameters!
*
* Contact: von_coler@tu-berlin.de
*
*
*/

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>
#include <jackaudioio.hpp>

#include "../../src/midiman.h"
#include "Biquad.h"

using std::cout;
using std::endl;



class JackFilter: public JackCpp::AudioIO {

private:

    MidiMan *midiMan;
    Biquad *lpFilter;

public:
    /// Audio Callback Function:
    /// - the output buffers are filled here
    /// args
    virtual int audioCallback(jack_nframes_t nframes,
                              // A vector of pointers to each input port.
                              audioBufVector inBufs,
                              // A vector of pointers to each output port.
                              audioBufVector outBufs){


        /// process midi messages
        int val = midiMan->get_rtmidi(2);
        int n   = midiMan->getNumFaderMessages();



        if(n!=0)
        {

            double fc = std::pow((double) val / 126.0,4.0) * 16000.0;
             lpFilter->setBiquad(bq_type_lowpass, fc / 44100.0, 0.9, 0);
            /// flush all messages
            midiMan->flushProcessedMessages();
        }



        /// LOOP
        for(int frameCNT = 0; frameCNT  < nframes; frameCNT++)
        {

            outBufs[0][frameCNT] = lpFilter->process(inBufs[0][frameCNT]);

        }

        ///return 0 on success
        return 0;
    }

    /// Constructor
    JackFilter() :
        JackCpp::AudioIO("biquad_filter", 1,1){


        reserveInPorts(2);
        reserveOutPorts(2);

        /// allocate a new midi manager
        midiMan = new MidiMan();

        /// the biquad allocate
        lpFilter = new Biquad();

        /// set initial parameters
        lpFilter->setBiquad(bq_type_lowpass, 1000.0 / 44100.0, 0.707, 0);


    }

};

///
///
///
int main(int argc, char *argv[]){



    /// initial ports from constructor created here.
    JackFilter * t = new JackFilter();

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
