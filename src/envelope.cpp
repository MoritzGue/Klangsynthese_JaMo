#include "envelope.h"
#include "math.h"
#include <iostream>
#include <stdlib.h>



Envelope::Envelope()
{
}

Envelope::~Envelope()
{
}

void Envelope::setEnvShape(envType type){

	
	int len = envDuration * sampleRate;
	double env[len];

	if (type==RECT){

		for (int i=0;i<len/2;i++){
			env[i] = 1;
		}
	}
	if (type==TRI){
		

		for (int i=0;i<len/2;i++){
			env[i] = i/(len/2-1);	
		}
		for (int i=len/2;i<len;i++){
			env[i] = 1/(i-(len/2-1));
		}

	}
	if (type==SIN){
	
		for (int i=0;i<len;i++){
		env[i] = sin(pi * (i/(len-1)));
		}
	}

for (int i=0;i<len;i++)
std::cout << env[i]<<std::endl ;

}

//double Envelope::nextSample() {

//}

