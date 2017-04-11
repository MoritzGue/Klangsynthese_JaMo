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

#include "../../src/sinusoid.h"
#include "../../src/oscillator.h"
#include "../../src/adsr.h"
#include "../../src/midiman.h"
#include "../../src/Biquad.h"
#include "../../src/OneZero.h"
#include "../../src/DelayModule.h"
#include "../../src/DelayLineSimple.h"

using std::cout;
using std::endl;
using namespace std;



class KarplusSimple: public JackCpp::AudioIO {

private:
    MidiMan *midiMan;
    
    Oscillator *mOscillator1;
    EnvelopeGenerator *mEnvelopeGenerator;
    
    Biquad *lpFilter;
    OneZero *loopFilter;
    
    DelayModule *delay1;
    DelayLineSimple *delayLine1;

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
            delayLine1->setDelayInSamples(44100.0/midiMan->getMidiNoteInHertz(noteNumber, 440.0));
            mOscillator1->setAmplitude(midiMan->mapMidiVelocity(velocity,0.0,1.0));
            mOscillator1->setMuted(false);
            
            if (envelopeToggle == true){
                mEnvelopeGenerator->enterStage(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK);
                envelopeToggle = false;
            }
            
        }

        if (noteStatus == -1){
            if (envelopeToggle == false){
            mEnvelopeGenerator->enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
            envelopeToggle = true;
            }
        }
        
        
        /// LOOP over all output buffers
        for(unsigned int i = 0; i < 1; i++)
        {
          for(int frameCNT = 0; frameCNT  < nframes; frameCNT++)
          {
              
              outBufs[0][frameCNT] = delayLine1->process(loopFilter->process(delayLine1->lastOut()*0.989) + (mOscillator1->nextSample() * mEnvelopeGenerator->nextSample()));
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
        mOscillator1 = new Oscillator(OSCILLATOR_MODE_NOISE);
        mOscillator1->setSampleRate(44100);

        mEnvelopeGenerator = new EnvelopeGenerator();
        mEnvelopeGenerator->setSampleRate(44100.0);

        midiMan = new MidiMan();
        envelopeToggle = true;
            
        /// the biquad allocate
        lpFilter = new Biquad();
        /// set initial parameters
        lpFilter->setBiquad(bq_type_lowpass, 1000.0 / 44100.0, 0.707, 0);
            
        loopFilter = new OneZero(-1.0);

            
        //delay1 = new DelayModule(44100.0, 2);
        
        delayLine1 = new DelayLineSimple(44100.0, 44100);
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
