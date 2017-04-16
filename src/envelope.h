#ifndef ENVELOPE_H
#define ENVELOPE_H

typedef enum {
    RECT = 0,
    TRI,
    SIN
}envType;

class Envelope {

public:


  	Envelope();

  	~Envelope();

	double pi = 3.14159265359;
	void setSampleRate(double newSampleRate){sampleRate = sampleRate;};
	void setEnvDuration(double newEnvDuration){envDuration = newEnvDuration;};
	void setEnvShape(envType type);

private:
	double sampleRate;
	//double env;
	envType mEnvType;
	double envDuration;
;
};
#endif
