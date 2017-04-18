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
    ENVELOPE_SHAPE_SIN
}envType;

class Envelope {

public:


  	Envelope();

  	~Envelope();

	double pi = 3.14159265359;
	void setSampleRate(double newSampleRate);
	void setEnvDuration(double newEnvDuration);
	void setEnvShape(envType type);
    void calculateEnv();
	void setEnvSwitchOn();
	void setEnvTable();
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
