/**
* \class JackSampler
*
* \brief This class processes MIDI massages
*           and uses them to control the playback speed
*           of a looped sample!
*
* \author Henrik von Coler
*
* \version $Revision: 0.5 $
*
* \date $Date: 2016/08/14 14:16:20 $
*
* \todo Nothing, for now.
*
* Contact: voncoler@tu-berlin.de
*
*
*/

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>
#include <jackaudioio.hpp>


#include "../../src/midiman.h"
#include "../../src/singlesample.h"


using std::cout;
using std::endl;



class JackSampler: public JackCpp::AudioIO {

private:

    MidiMan         *midiMan;
    SingleSample    *sample;

public:
    /// Audio Callback Function:
    /// - the output buffers are filled here
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
            sample->set_rate((double) val / 66.0);
            /// flush all messages
            midiMan->flushProcessedMessages();
        }


        /// display rate
        //  cout << sample->get_rate() << endl;



        /// LOOP over all output buffers
        for(unsigned int i = 0; i < 1; i++)
        {

            sample->get_frame(nframes,outBufs[0]);

        }

        ///return 0 on success
        return 0;
    }

    /// Constructor
    JackSampler(std::string s) :
        JackCpp::AudioIO("jack_sampler", 0,1){


        reserveInPorts(2);
        reserveOutPorts(2);

        /// allocate a new midi manager
        midiMan = new MidiMan();

        /// allocate the sample player
        sample = new SingleSample(s);

        sample->set_rate(1.0);

    }

};

///
///
///
int main(int argc, char *argv[]){

    if(argc<=1)
    {
        cout << "Pass path to wav-file as argument!" << endl;
        return 0;
    }

    std::string s(argv[1]);

    /// initial ports from constructor created here.
    JackSampler * t = new JackSampler(s);

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
