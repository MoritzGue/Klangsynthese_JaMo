/**
 * @class Envelope
 *
 *
 * @brief Creates a vector for shaping the form of an excitation burst
 *  
 *
 *
 *
 * \author Moritz Güldenring & Janek Newjoto
 *
 *
 * Contact:
 *
 *
 */

#ifndef ENVELOPE_H
#define ENVELOPE_H

#include "math.h"
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

typedef enum {
    ENVELOPE_SHAPE_RECT = 0,
    ENVELOPE_SHAPE_TRI,
    ENVELOPE_SHAPE_SIN,
    ENVELOPE_SHAPE_HAN
}envType;

class Envelope {

public:


  	Envelope();

  	~Envelope();

	double pi = 3.14159265359;
	void setSampleRate(double newSampleRate);
	/**
        * @brief sets the duration of the excitation window
        * @param time in seconds
        */
	void setEnvDuration(double newEnvDuration);

	/**
        * @brief sets the window shape of the excitation window (rectangular, triangular, sinus, hanning)
        * @param type
        */
	void setEnvShape(envType type);

	/**
        * @brief uses envelope duration and window shape to calculate the excitation window as a vector of samples
        */
    	void calculateEnv();

	void setEnvSwitchOn();

	/**
        * @brief returns a pointer to the next sample of the excitation window
        * as long as duration time is not exceeded
        */
	double nextSample();

	int envSwitchOn();

private:

	std::vector<double>env;
	double sampleRate;
	int envSwitch;
	envType mEnvType;
	double envDuration;
	int currentSampleIndex;
	int len;
	double *multiplier;
	double null=0.0;


};
#endif
