/*
  ==============================================================================

    DelayModule.h
    Created: 18 Nov 2015 5:12:22pm
    Author:  Moritz Güldenring

  ==============================================================================
*/
#include <cmath>
#include <stdio.h>
#include <string.h>

#ifndef DELAYMODULE_H_INCLUDED
#define DELAYMODULE_H_INCLUDED


class DelayModule {
    
public:
    
    DelayModule(double sampleRate, int bufferSizeInSeconds);
    ~DelayModule();
    
    //==============================================================================
    double process(double input);
    
    void cookVariables();
    void resetDelay();
    inline double dLinTerp(double x1, double x2, double y1, double y2, double x);

    //==============================================================================
    void setDelayInSamples(double delayInSamples);
    void setFeedback(double feedback);
    void setWetLevel(double wetLevel);

    void setDelayInMs(double delayInMs);
    void setFeedbackInPct(double feedbackInPct);
    void setWetLevelInPct(double wetLevelInPct);

private:

    double m_fDelayInSamples;
    double m_fFeedback;
    double m_fWetLevel;
    
    double m_fDelay_ms;
    double m_f_Feedback_pct;
    double m_f_WetLevel_pct;
    
    double m_nSampleRate;
    
    double* m_pBuffer;
    int m_nReadIndex;
    int m_nWriteIndex;
    int m_nBufferSize;
    
    
};



#endif  // DELAYMODULE_H_INCLUDED
