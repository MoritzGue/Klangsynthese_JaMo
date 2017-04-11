
#define _USE_MATH_DEFINES

#include <iostream>
#include <stdlib.h>
#include<math.h>
#include<cmath>

class EnvelopeGenerator {
public:
    
    enum EnvelopeStage {
        ENVELOPE_STAGE_OFF = 0,
        ENVELOPE_STAGE_ATTACK,
        ENVELOPE_STAGE_DECAY,
        ENVELOPE_STAGE_SUSTAIN,
        ENVELOPE_STAGE_RELEASE,
        kNumEnvelopeStages
    };
    
    void enterStage(EnvelopeStage newStage);
    void setStageValue(EnvelopeStage stage, double value);
    double nextSample();
    void setSampleRate(double newSampleRate);
    inline EnvelopeStage getCurrentStage() const { return currentStage; };
    const double minimumLevel;
    
    EnvelopeGenerator() :
    minimumLevel(0.0001),
    currentStage(ENVELOPE_STAGE_OFF),
    currentLevel(minimumLevel),
    multiplier(1.0),
    sampleRate(44100.0),
    currentSampleIndex(0),
    nextStageSampleIndex(0) {
        stageValue[ENVELOPE_STAGE_OFF] = 0.0;
        stageValue[ENVELOPE_STAGE_ATTACK] = 0.0;
        stageValue[ENVELOPE_STAGE_DECAY] = 0.1;
        stageValue[ENVELOPE_STAGE_SUSTAIN] = 0.0;
        stageValue[ENVELOPE_STAGE_RELEASE] = 0.0;
    };

private:
    
    EnvelopeStage currentStage;
    double currentLevel;
    double multiplier;
    double sampleRate;
    double stageValue[kNumEnvelopeStages];
    void calculateMultiplier(double startLevel, double endLevel, unsigned long long lengthInSamples);
    unsigned long long currentSampleIndex;
    unsigned long long nextStageSampleIndex;

};


