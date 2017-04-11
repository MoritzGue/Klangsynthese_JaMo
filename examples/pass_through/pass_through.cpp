 /**
* \class PassThrough
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
#include <jackaudioio.hpp>


using std::cout;
using std::endl;

class PassThrough: public JackCpp::AudioIO {

private:



public:
    /// Audio Callback Function:
    /// - the output buffers are filled here
    /// args
    virtual int audioCallback(jack_nframes_t nframes,
                              // A vector of pointers to each input port.
                              audioBufVector inBufs,
                              // A vector of pointers to each output port.
                              audioBufVector outBufs){


        /// LOOP over all samples
        for(int frameCNT = 0; frameCNT  < nframes; frameCNT++)
        {

            /// just copy sample-wise
            outBufs[0][frameCNT] =  inBufs[0][frameCNT];

        }

        ///return 0 on success
        return 0;
    }

    /// Constructor XXX
    PassThrough() :
        JackCpp::AudioIO("pass_through", 1,1){


        reserveInPorts(1);
        reserveOutPorts(2);

    }

};

///
///
///
int main(int argc, char *argv[]){

    /// initial ports from constructor created here.
    PassThrough * t = new PassThrough();

    /// activate the client
    t->start();

    /// connect input
    t->connectFromPhysical(0,0);

    /// connect ports to stereo ports
    t->connectToPhysical(0,0);		// connects this client out port 0 to physical destination port 0
    t->connectToPhysical(0,1);		// connects this client out port 1 to physical destination port 1

    /// run for EVER
    sleep(-1);

    /// never reached:!=
    t->disconnectInPort(0);	// Disconnecting ports.
    t->disconnectOutPort(1);
    t->close();	// stop client.
    delete t;	// always clean up after yourself.
    exit(0);
}
