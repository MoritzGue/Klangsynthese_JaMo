/**
* \class SimpleFm
*
* \brief
*
* \author Henrik von Coler
*
* \version $Revision: 0.5 $
*
* \date $Date: 2016/08/14 14:16:20 $
*
* \todo Many many things ...
*
* \author voncoler@tu-berlin.de
*
*
*/

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>
#include <jackaudioio.hpp>

#include "../../src/sinusoid.h"
#include "../../src/oscman.h"


using std::cout;
using std::endl;



class SimpleFm: public JackCpp::AudioIO {

private:

    Sinusoid *sin1;
    Sinusoid *sin2;

    ///
    double rate;

    OscMan *oscMan;

public:
    /// Audio Callback Function:
    /// - the output buffers are filled here
    virtual int audioCallback(jack_nframes_t nframes,
                              // A vector of pointers to each input port.
                              audioBufVector inBufs,
                              // A vector of pointers to each output port.
                              audioBufVector outBufs){


        /// process OSC
        if(oscMan->getNumberOfdMess() > 0)
        {


             dMess tmp = oscMan->getLast_dMess();

             std::string p   = tmp.path;

             double val         = tmp.val;

            if (p == "/leftVolume")
                sin1->frequency((double (val) / 127.0) * 5000.0);
            if (p == "/rightVolume")
                sin2->frequency((double (val) / 127.0) * 5000.0);
            if (p == "/crossfader")
                rate = (0.1 + (double (val) / 127.0)) * 100.0;


        }



        /// LOOP over all output buffers
        for(unsigned int i = 0; i < 1; i++)
        {

            for(int frameCNT = 0; frameCNT  < nframes; frameCNT++)
            {

                sin2->frequency(sin2->frequency() + rate * sin1->getNextSample());

                outBufs[0][frameCNT] = sin2->getNextSample();
            }
        }

        ///return 0 on success
        return 0;
    }

    /// Constructor
    SimpleFm(double f1, double f2, double r, int port) :
        JackCpp::AudioIO("fm_example", 0,1){


        /// create new OscMan Object
        oscMan = new OscMan(port);


        reserveInPorts(2);
        reserveOutPorts(2);



        sin1        = new Sinusoid(f1,0.5,1,44100);
        sin2        = new Sinusoid(f2,0.5,1,44100);
        rate        = r;



    }

};

///
///
///
int main(int argc, char *argv[]){



    if(argc<1)
    {
        std::cout << "Need port number!" << std::endl;
        return 0;
    }

    int port  = atoi(argv[1]);


    double f1 = 1000.0;
    double f2 = 200.0;

    double rate = 20.0;


    /// initial ports from constructor created here.
    SimpleFm * t = new SimpleFm(f1,f2,rate,port);

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
