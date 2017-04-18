#include "envelope.h"
#include "math.h"
#include <iostream>
#include <stdlib.h>
#include <vector>



Envelope::Envelope()
{
}

Envelope::~Envelope()
{
}
//void setEnvTable{
	//len = floor(envDuration * sampleRate);
	//double env[len];
//}


void Envelope::setEnvShape(envType type){

	len = floor(envDuration * sampleRate);
	env.reserve(len);
	
	
	if (type==RECT){

		for (int i=0;i<len;i++){
			env[i] = 1;
		}
	}
	if (type==TRI){
		

		for (int i=0;i<len/2;i++){
			env[i] = i/((double)len/2);	
		}
		for (int i=len/2;i<len;i++){
			env[i]= -i/ ((double)len/2) +2;
		}

	}
	if (type==SIN){
	
		for (int i=0;i<len;i++){
		env[i] = sin(pi * (i/((double)len-1)));
		}
	}
}

void Envelope::setEnvSwitchOn(){
	envSwitch = 1;
	currentSampleIndex = 0;
}

double Envelope::nextSample() {
	double *multiplier;
	double null=0.0;
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
}

