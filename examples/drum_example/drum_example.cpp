
/**
* \class KickDrum
*
* \brief Exponential sinusoidal kick-drum, MIDI triggered!
*
* \author Henrik von Coler
*
* \version $Revision: 0.5 $
*
* \date $Date: 2016/08/14 14:16:20 $
*
*  \todo Many many things ...
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

#include "../../src/sinusoid.h"
#include "../../src/midiman.h"




using std::cout;
using std::endl;



class KickDrum: public JackCpp::AudioIO {

private:

    MidiMan     *midiMan;
    Sinusoid    *sin1;

public:
    /// Audio Callback Function:
    /// - the output buffers are filled here
    virtual int audioCallback(jack_nframes_t nframes,
                              // A vector of pointers to each input port.
                              audioBufVector inBufs,
                              // A vector of pointers to each output port.
                              audioBufVector outBufs){



        int val = 0;
        /// process midi messages
        val = midiMan->get_rtmidi(0);




        /// LOOP over all output buffers
        for(unsigned int i = 0; i < 1; i++)
        {

            for(int frameCNT = 0; frameCNT  < nframes; frameCNT++)
            {

                /// let frequency decay "exponentially"
                sin1->frequency( sin1->frequency() * 0.9999);

                outBufs[0][frameCNT] = sin1->getNextSample();
            }

            /// reset for loop
            if( val==144)
                sin1->frequency( 190.0);
        }

        ///return 0 on success
        return 0;
    }

    /// Constructor
    KickDrum() :
        JackCpp::AudioIO("kick_drum", 0,1){

        reserveInPorts(2);
        reserveOutPorts(2);



        /// allocate a new midi manager
        midiMan = new MidiMan();
        midiMan->setVerbose();

        sin1        = new Sinusoid(190,0.5,1,44100);




    }

};

///
///
///
int main(int argc, char *argv[]){



    /// initial ports from constructor created here.
    KickDrum * t = new KickDrum();

    /// activate the client
    t->start();

    /// connect ports to stereo ports
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




//#include <stdio.h>
//#include <errno.h>
//#include <unistd.h>
//#include <stdlib.h>
//#include <string.h>
//#include <math.h>

//#include <vector>
//#include <algorithm>
//#include <atomic>


//#include <jack/jack.h>
//#include "../../src/sinusoid.h"

//jack_client_t *client;
//jack_port_t *output_port;

//Sinusoid *sin1;

//int
//process (jack_nframes_t nframes, void *arg)
//{

//    jack_default_audio_sample_t  *out;

//    out = static_cast<float *> (jack_port_get_buffer(output_port, nframes));

//    /// loop over the output buffer
//    for(int sampCNT=0; sampCNT<nframes; sampCNT++)
//    {

//        /// assign sample to output buffer
//        out[sampCNT] = sin1->getNextSample();

//        /// let frequency decay "exponentially"
//        sin1->frequency( sin1->frequency() * 0.9999);



//    }


//    /// reset for loop
//    if( sin1->frequency() <= 10)
//        sin1->frequency( 190.0);


//    return 0;
//}



///**
// * JACK calls this shutdown_callback if the server ever shuts down or
// * decides to disconnect the client.
// */
//void
//jack_shutdown (void *arg)
//{
//    exit (1);
//}

//int
//main (int argc, char *argv[])
//{




//    const char **ports;
//    const char *client_name = "simple";
//    const char *server_name = NULL;
//    jack_options_t options = JackNullOption;
//    jack_status_t status;

//    // initialize



//    /* open a client connection to the JACK server */

//    client = jack_client_open (client_name, options, &status, server_name);
//    if (client == NULL) {
//        fprintf (stderr, "jack_client_open() failed, "
//                         "status = 0x%2.0x\n", status);
//        if (status & JackServerFailed) {
//            fprintf (stderr, "Unable to connect to JACK server\n");
//        }
//        exit (1);
//    }
//    if (status & JackServerStarted) {
//        fprintf (stderr, "JACK server started\n");
//    }
//    if (status & JackNameNotUnique) {
//        client_name = jack_get_client_name(client);
//        fprintf (stderr, "unique name `%s' assigned\n", client_name);
//    }

//    /* tell the JACK server to call `process()' whenever
//       there is work to be done.
//    */

//    jack_set_process_callback (client, process, 0);


//    input_port  = jack_port_register(client,"MIDI in",JACK_DEFAULT_MIDI_TYPE,JackPortIsInput,0);


//    jack_on_shutdown (client, jack_shutdown, 0);

//    /* display the current sample rate.
//     */


//    output_port = jack_port_register (client, "output",
//                                      JACK_DEFAULT_AUDIO_TYPE,
//                                      JackPortIsOutput, 0);

//    if ((output_port == NULL)) {
//        fprintf(stderr, "no more JACK ports available\n");
//        exit (1);
//    }



//    if (jack_activate (client)) {
//        fprintf (stderr, "cannot activate client");
//        exit (1);
//    }


//    ports = jack_get_ports (client, NULL, NULL,
//                            JackPortIsPhysical|JackPortIsOutput);

//    free (ports);

//    ports = jack_get_ports (client, NULL, NULL,
//                            JackPortIsPhysical|JackPortIsInput);
//    if (ports == NULL) {
//        fprintf(stderr, "no physical playback ports\n");
//        exit (1);
//    }

//    if (jack_connect (client, jack_port_name (output_port), ports[0])) {
//        fprintf (stderr, "cannot connect output ports\n");
//    }

//    free (ports);


//    double fs   = (double) jack_get_sample_rate (client);


//    sleep (-1);

//    jack_client_close (client);
//    exit (0);
//}

