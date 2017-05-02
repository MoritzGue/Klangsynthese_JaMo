/**
 * \file
 *
 * \brief This is the main file and implements a polyphonic Karplus-Strong Waveguide algorithm.
 *          When starting the application enter a wavefile path to an impulse response or type "noir" for
 *          no convolution.
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
#include <vector>
#include <jackaudioio.hpp>


#include "../src/guitarstring.h"
#include "../src/stringManager.h"
#include "../src/midiman.h"
#include "../src/singlesample.h"
#include "../src/FFTConvolver/FFTConvolver.h"

using std::cout;
using std::endl;
using namespace std;


class KarplusPoly: public JackCpp::AudioIO {

private:
    
    StringManager *gitStrings;          // Manages MIDI-Messages and dristibutes them to multiple Waveguides
    MidiMan *midiMan;                   // MIDI handling
    SingleSample *singleSample;
    fftconvolver::FFTConvolver fftconv; // FFT-Convolution
    
    int noteStatus;
    int noteNumber;
    int velocity;
    bool bypassConv = true;
    //float *ir_temp;
    //std::vector<float> ir;


public:
    /// Audio Callback Function:
    /// - the output buffers are filled here
    virtual int audioCallback(jack_nframes_t nframes,
                              // A vector of pointers to each input port.
                              audioBufVector inBufs,
                              // A vector of pointers to each output port.
                              audioBufVector outBufs){

        if (bypassConv == true){
        
            /// LOOP without Convolution over all output buffers
            for(unsigned int i = 0; i < 1; i++)
            {
                for(int frameCNT = 0; frameCNT  < nframes; frameCNT++)
                {
                    processMIDI();
                    // Synthesize Samples
                    outBufs[0][frameCNT] = gitStrings->getNextSample();
                }
            }
        }

        if (bypassConv == false){

            /// LOOP with Convolution over all output buffers
            for(unsigned int i = 0; i < 1; i++)
            {
                for(int frameCNT = 0; frameCNT  < nframes; frameCNT++)
                {
                    processMIDI();
                    // Synthesize Samples
                    outBufs[0][frameCNT] = gitStrings->getNextSample()*0.1;
                }
                // Convolution with guitar body IR
                fftconv.process(&outBufs[0][0], &outBufs[0][0], nframes);
            }
        }
        
        
        ///return 0 on success
        return 0;
    }

    /// Constructor
    KarplusPoly(std::string s) :
    JackCpp::AudioIO("karplus_poly", 0,1)
    {

        reserveInPorts(2);
        reserveOutPorts(2);
        
        
        
        if ( s != "noir"){
            singleSample = new SingleSample(s);
            float *ir_temp = singleSample->get_x();

            // initialize FFT-Convolution with IR
            fftconv.init(1024, ir_temp, singleSample->get_L());
            bypassConv = false;
        }
        
        gitStrings = new StringManager();
        midiMan = new MidiMan();
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

    
    if(argc<=1)
    {
        cout << "Pass path to wav-file as argument!" << endl;
        return 0;
    }
    
    std::string s(argv[1]);
    
    /// initial ports from constructor created here.
    KarplusPoly * t = new KarplusPoly(s);

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
