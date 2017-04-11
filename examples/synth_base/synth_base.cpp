/**
 * \file
 *
 * \brief This is an example of creating a sinusoid.
 *
 *
 *
 * \author Henrik von Coler
 *
 * \version $Revision: 0.5 $
 *
 * \date $Date: 2016/08/18$
 *
 * Contact: von_coler@tu-berlin.de
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
#include "../../src/DelayModule.h"
#include "../../src/DelayLineSimple.h"

using std::cout;
using std::endl;
using namespace std;



class SynthBase: public JackCpp::AudioIO {

private:
    Oscillator *mOscillator1;
    EnvelopeGenerator *mEnvelopeGenerator;
    MidiMan *midiMan;
    Biquad *lpFilter;
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

        /*

        switch (mm.byte1) {
          case 144:
                    noteCounter++;
                    cout << "noteNumber" << noteNumber << endl;
                    mOscillator1->setFrequency(midiMan->getMidiNoteInHertz(noteNumber, 440.0));
                    cout << "velocity ON" << velocity << endl;
                    mOscillator1->setAmplitude(midiMan->mapMidiVelocity(velocity,0.0,1.0));
                    cout << "Note Counter" << noteCounter << endl;
                    mEnvelopeGenerator->enterStage(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK);
                    mOscillator1->setMuted(false);
                    /// flush all messages
                    midiMan->flushProcessedMessages();
                    break;
          case 128:
                    if (noteCounter>0)noteCounter--;
                    cout << "status" << status << endl;
                    cout << "velocity OFF" << velocity << endl;
                    cout << "Note Counter" << noteCounter << endl;
                    /// flush all messages
                    midiMan->flushProcessedMessages();
                    break;
        }

        if(noteCounter == 0){
          mEnvelopeGenerator->enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
          //mOscillator1->setMuted(true);
          //mOscillator1->setAmplitude(0.0);
        }*/
         
        midiMan->doRtmidi();
        
        noteStatus = midiMan->getStatus();
        noteNumber = midiMan->getNoteNumber();
        velocity = midiMan->getVelocity();
        
        if (noteStatus == 144 && velocity) {
            cout << "Note Status" << noteStatus << endl;
            cout << "Velocity" << velocity << endl;
            mOscillator1->setFrequency(midiMan->getMidiNoteInHertz(noteNumber, 440.0));
            mOscillator1->setAmplitude(midiMan->mapMidiVelocity(velocity,0.0,1.0));
            mOscillator1->setMuted(false);
            if (envelopeToggle == true){
                mEnvelopeGenerator->enterStage(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK);
                envelopeToggle = false;
            }
            delayLine1->setDelayInSamples(44100.0/midiMan->getMidiNoteInHertz(noteNumber, 440.0));
        }

        if (noteStatus == -1){
            if (envelopeToggle == false){
            mEnvelopeGenerator->enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
            envelopeToggle = true;
            }
            //mOscillator1->setMuted(true);
        }
        
        
        /// LOOP over all output buffers
        for(unsigned int i = 0; i < 1; i++)
        {
          for(int frameCNT = 0; frameCNT  < nframes; frameCNT++)
          {
              
              /*double x_n = mOscillator1->nextSample() * mEnvelopeGenerator->nextSample();
              double y_n_m;
              double y_n = delayLine1->process(x_n) + 0.8*y_n_m;
              
              outBufs[0][frameCNT] = y_n;
              3.0 * delayLine_.tick( loopFilter_.tick( delayLine_.lastOut() * loopGain_ ) )
              y_n_m = delayLine1->process(y_n);*/
              
              outBufs[0][frameCNT] = delayLine1->process(0.95*delayLine1->lastOut() + (mOscillator1->nextSample() * mEnvelopeGenerator->nextSample()));
          }

        }

        ///return 0 on success
        return 0;
    }

    /// Constructor
    SynthBase(double f1) :
        JackCpp::AudioIO("synth_base", 0,1){

        reserveInPorts(2);
        reserveOutPorts(2);
        mOscillator1 = new Oscillator();
        mOscillator1->setSampleRate(44100);

        mEnvelopeGenerator = new EnvelopeGenerator();
        mEnvelopeGenerator->setSampleRate(44100.0);

        midiMan = new MidiMan();
        envelopeToggle = true;
            
        /// the biquad allocate
        lpFilter = new Biquad();
        /// set initial parameters
        lpFilter->setBiquad(bq_type_lowpass, 15000.0 / 44100.0, 0.707, 0);
            
        delay1 = new DelayModule(41000.0, 2);
        
        delayLine1 = new DelayLineSimple(41000.0, 2);
    }
};

///
///
///
int main(int argc, char *argv[]){

    double f1 = 244.0;
    /// initial ports from constructor created here.
    SynthBase * t = new SynthBase(f1);

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
