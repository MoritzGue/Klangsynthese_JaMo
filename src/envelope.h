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
	void setSampleRate(double newSampleRate){sampleRate = newSampleRate;};
	void setEnvDuration(double newEnvDuration){envDuration = newEnvDuration;};
	void setEnvShape(envType type);
	void setEnvSwitchOn();
	void setEnvTable();
	double nextSample();
	int envSwitchOn();

private:
	double sampleRate;
	int envSwitch;
	envType mEnvType;
	double envDuration;
	int currentSampleIndex;
	int len;


};
#endif
