/*
  ==============================================================================

    DelayLineSimple.h
    Created: 18 Nov 2015 5:12:22pm
    Author:  Moritz Güldenring

  ==============================================================================
*/
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <string.h>

#ifndef DELAYLINESIMPLE_H_INCLUDED
#define DELAYLINESIMPLE_H_INCLUDED
using namespace std;

class DelayLineSimple {
    
public:
    
    DelayLineSimple(double sampleRate, int bufferSizeInSeconds);
    ~DelayLineSimple();
    
    //==============================================================================
    
    double lastOut( void ) const { return output; };
    
    void cookVariables();
    void resetDelay();
    inline double process(double input);
    inline double dLinTerp(double x1, double x2, double y1, double y2, double x);
    inline double allPass(double xn);

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
    
    double alpha_;
    double coeff_;
    bool doNextOut_;
    
    double xn_1;
    double yn_1;
};
//==============================================================================
//INTERPOLATE
//==============================================================================
inline double DelayLineSimple::dLinTerp(double x1, double x2, double y1, double y2, double x)
{
    double denom = x2 - x1;
    if(denom == 0)
        return y1; // should not ever happen
    
    // calculate decimal position of x
    double dx = (x - x1)/(x2 - x1);
    
    // use weighted sum method of interpolating
    double result = dx*y2 + (1-dx)*y1;
    
    return result;
    
}

inline double DelayLineSimple::allPass(double xn)
{
    //double xn = input;
    double yn = coeff_*(xn - yn_1) + xn_1;
    xn_1 = xn;
    yn_1 = yn;
    return yn;
}

//==============================================================================
//PROCESS
//==============================================================================
inline double DelayLineSimple::process(double input)
{
    
    
    // ..do something to the data...
    
    
    // Read the input
    double xn = input;
    
    // Read the Output of the delay at m_nReadIndex
    double yn = m_pBuffer[m_nReadIndex];
    
    
    //if delay < 1 sample, interpolate between input x(n) and x(n-1)
    if(m_nReadIndex == m_nWriteIndex && m_fDelayInSamples < 1.00)
    {
        //Interpolate x(n) with x(n-1), set yn = xn
        yn = xn;
    }
    
    //Read the location ONE BEHIND yn at y(n-1)
    int nReadIndex_1 = m_nReadIndex - 1;
    if(nReadIndex_1 < 0)
        nReadIndex_1 = m_nBufferSize - 1; // m_nBufferSize is last location
    
    
    //get y(n-1)
    double y_1 = m_pBuffer[nReadIndex_1];
    
    
    //interpolate: (0, yn) and (1, yn_1) by the amount of fracDelay
    double fFracDelay = m_fDelayInSamples - (int)m_fDelayInSamples;
    
    //linerp
    double fInterp = dLinTerp(0, 1, yn, y_1, fFracDelay); //Interpolate frac between them
    
    //if zero delay just pass in to out
    if (m_fDelayInSamples == 0)
        yn = xn;
    else
        yn = fInterp;
    
    
    
    
    //write the input to the delay
    m_pBuffer[m_nWriteIndex] = xn;// + m_fFeedback*yn;
    
    //create the wet/ dry mix and write output buffer
    // dry = 1 - wet
    output = allPass(yn);
    
    //increment the pointers and wrap if necessary
    
    m_nWriteIndex++;
    if (m_nWriteIndex >= m_nBufferSize)
        m_nWriteIndex = 0;
    
    m_nReadIndex++;
    if (m_nReadIndex >= m_nBufferSize)
        m_nReadIndex = 0;
    
    return output;
    
}
#endif  // DELAYLINESIMPLE_H_INCLUDED
