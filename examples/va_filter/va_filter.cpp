/**
* \class SimpleVaFilter
*
* \brief Most simple example of a pass through.
*
* \author Henrik von Coler
*
* \version $Revision: 0.5 $
*
* \date $Date: 2016/11/22 $
*
* Contact: von_coler@tu-berlin.de
*
*/

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>
#include <math.h>
#include <jackaudioio.hpp>

#include "../../src/midiman.h"

using std::cout;
using std::endl;
using namespace std;

class SimpleVaFilter: public JackCpp::AudioIO {

private:

    MidiMan *midiMan;
    float z1;
    float G;
    
    
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
            
            // pre-warp the cutoff- these are bilinear-transform filters
            float wd = 2*M_PI*fc;
            //float T  = 1/44100.0;
            float T  = 1/(float)getSampleRate();
            float wa = (2/T)*tan(wd*T/2);
            float g  = wa*T/2;
            // big combined value
            G = g/(1.0 + g);
            
            /// flush all messages
            midiMan->flushProcessedMessages();
        }

        /// LOOP over all samples
        for(int frameCNT = 0; frameCNT  < nframes; frameCNT++)
        {

            /// just copy sample-wise
            float v = (inBufs[0][frameCNT] - z1)*G;
            
            float y = v + z1;
            
            z1 = y + v;
            
            outBufs[0][frameCNT] = y;
        
        }

        ///return 0 on success
        return 0;
    }

    /// Constructor XXX
    SimpleVaFilter() :
        JackCpp::AudioIO("va_filter", 1,1){

        reserveInPorts(1);
        reserveOutPorts(2);
            
        /// allocate a new midi manager
        midiMan = new MidiMan();
            
        z1 = 0;
        G = 0.5;
    }
    
   //~SimpleVaFilter(){delete midiMan;}

};

///
///
///
int main(int argc, char *argv[]){
    

    /// initial ports from constructor created here.
    SimpleVaFilter * t = new SimpleVaFilter();

    /// activate the client
    t->start();

    /// connect input
    t->connectFromPhysical(0,0);

    /// connect ports to stereo ports
    t->connectToPhysical(0,0);		// connects this client out port 0 to physical destination port 0
    t->connectToPhysical(0,1);		// connects this client out port 1 to physical destination port 1
    

    /// run for EVER
    sleep(10);
    
    /// never reached:!=
    t->disconnectInPort(0);	// Disconnecting ports.
    t->disconnectOutPort(1);
    t->close();	// stop client.
    delete t;	// always clean up after yourself.
    exit(0);
}
