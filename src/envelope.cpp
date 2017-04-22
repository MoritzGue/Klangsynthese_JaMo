/**
 * \class Envelope
 *
 *
 * \Creates a vector for shaping the form of an excitation burst
 *  
 *
 *
 *
 * \author Moritz Güldenring & Janek Newjoto
 *
 * \version
 *
 * \date
 *
 * Contact:
 *
 *
 */

#include "envelope.h"


Envelope::Envelope()
{
    mEnvType = ENVELOPE_SHAPE_RECT;
    setEnvDuration(0.01);
}

Envelope::~Envelope()
{
}

void Envelope::setSampleRate(double newSampleRate){
    sampleRate = newSampleRate;
    calculateEnv();
}

void Envelope::setEnvDuration(double newEnvDuration){
    envDuration = newEnvDuration;
    calculateEnv();
}

void Envelope::setEnvShape(envType type){
    mEnvType = type;
    calculateEnv();
}


void Envelope::calculateEnv(){

	len = floor(envDuration * sampleRate);
	env.reserve(len);
	
	
	if (mEnvType==ENVELOPE_SHAPE_RECT){
		for (int i=0;i<len;i++){
			env[i] = 1;
		}
    	}
	if (mEnvType==ENVELOPE_SHAPE_TRI){
		

		for (int i=0;i<len/2;i++){
			env[i] = i/((double)len/2);	
		}
		for (int i=len/2;i<len;i++){
			env[i]= -i/ ((double)len/2) +2;
		}

	}
	if (mEnvType==ENVELOPE_SHAPE_SIN){
	
		for (int i=0;i<len;i++){
		env[i] = sin(pi * (i/((double)len-1)));
		}
	}
	if (mEnvType==ENVELOPE_SHAPE_HAN){
		for (int i=0;i<len;i++) {
    		env[i] = 0.5 * (1 - cos(2*pi*i/(len-1)));
		}
	}
}

void Envelope::setEnvSwitchOn(){
	envSwitch = 1;
	currentSampleIndex = 0;
}

double Envelope::nextSample() {

	if(envSwitch == 1){
		multiplier = &env[currentSampleIndex];
		currentSampleIndex++;
		if(currentSampleIndex == len){
			envSwitch = 0;
		}

	}
	else {
		multiplier = &null;
	}
return *multiplier;
//std::cout<<*multiplier<<std::endl;
}

