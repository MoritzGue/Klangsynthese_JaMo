/*
  ==============================================================================

    DelayLineSimple.h
    Created: 18 Nov 2015 5:12:22pm
    Author:  Moritz Güldenring

  ==============================================================================
*/
#include <cmath>
#include <stdio.h>
#include <string.h>

#ifndef DELAYLINESIMPLE_H_INCLUDED
#define DELAYLINESIMPLE_H_INCLUDED


class DelayLineSimple {
    
public:
    
    DelayLineSimple(double sampleRate, int bufferSizeInSeconds);
    ~DelayLineSimple();
    
    //==============================================================================
    double process(double input);
    double lastOut( void ) const { return output; };
    
    void cookVariables();
    void resetDelay();
    inline double dLinTerp(double x1, double x2, double y1, double y2, double x);

    //==============================================================================
    void setDelayInSamples(double delayInSamples);

    void setDelayInMs(double delayInMs);
    
    


private:

    double m_fDelayInSamples;

    double m_fDelay_ms;
    
    double m_nSampleRate;
    
    double* m_pBuffer;
    double output;
    int m_nReadIndex;
    int m_nWriteIndex;
    int m_nBufferSize;
    
    
};



#endif  // DELAYLINESIMPLE_H_INCLUDED
