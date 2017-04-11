/*
  ==============================================================================

    DelayLineSimple.cpp
    Created: 18 Nov 2015 5:12:22pm
    Author:  Moritz Güldenring

  ==============================================================================
*/

#include "DelayLineSimple.h"

DelayLineSimple::DelayLineSimple(double sampleRate, int bufferSizeInSeconds)
{
    m_fDelayInSamples = 200.0;
    
    m_fDelay_ms = 0;

    
    //reset
    m_nReadIndex = 0;
    m_nWriteIndex = 0;
    
    //no buffer yet because we don't have sample rate yet
    m_pBuffer = NULL;
    m_nBufferSize = 0;
    
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    m_nSampleRate = sampleRate;
    
    //setup the delay line
    m_nBufferSize = 2*(int)sampleRate;
    
    //delete Buffer if it exsits
    if(m_pBuffer)
        delete [] m_pBuffer;

    //create teh new buffer
    m_pBuffer = new double[m_nBufferSize];
    
    //reset
    resetDelay();

    
    
    //then
    cookVariables();

}

DelayLineSimple::~DelayLineSimple()
{
    delete [] m_pBuffer;
}
//==============================================================================
//RESET DELAY
//==============================================================================

void DelayLineSimple::resetDelay()
{
    //flush Buffer
    if(m_pBuffer)
        memset(m_pBuffer, 0, m_nBufferSize*sizeof(double));
    
    //init read/write indicies
    m_nWriteIndex = 0; //reset write index to top
    m_nReadIndex = 0; //reset read index to top
    
}
//==============================================================================
//SET VARIABLES
//==============================================================================

void DelayLineSimple::setDelayInSamples(double delayInSamples)
{
    m_fDelayInSamples = delayInSamples;
    cookVariables();
}


void DelayLineSimple::setDelayInMs(double delayInMs)
{
    m_fDelay_ms = delayInMs;
    m_fDelayInSamples = m_fDelay_ms*(m_nSampleRate/1000.0);  // 2 seconds delay @ fs
    cookVariables();
}


//==============================================================================
//COOK
//==============================================================================

void DelayLineSimple::cookVariables()
{

    // subtract to make read index
    m_nReadIndex = m_nWriteIndex - (int)m_fDelayInSamples;
    
    // check and wrap backwards
    if (m_nReadIndex < 0)
        m_nReadIndex += m_nBufferSize;  // amount of wrap is Read + Length
}
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
//==============================================================================
//PROCESS
//==============================================================================

double DelayLineSimple::process(double input)
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
    
                /*
                //get y(n-1)
                double yn_1 = m_pBuffer[nReadIndex_1];
    
                //interpolate: (0, yn) and (1, yn_1) by the amount of fracDelay
                double fFracDelay = m_fDelayInSamples - (int)m_fDelayInSamples;
                
                //linerp
                double fInterp = dLinTerp(0, 1, yn, yn_1, fFracDelay); //Interpolate frac between them
                
                //if zero delay just pass in to out
                if (m_fDelayInSamples == 0)
                    yn = xn;
                else
                    yn = fInterp;
    */
    
                //write the input to the delay
                m_pBuffer[m_nWriteIndex] = xn;// + m_fFeedback*yn;
                
                //create the wet/ dry mix and write output buffer
                // dry = 1 - wet
                output = yn;
                
                //increment the pointers and wrap if necessary
    
                m_nWriteIndex++;
                if (m_nWriteIndex >= m_nBufferSize)
                    m_nWriteIndex = 0;
                
                m_nReadIndex++;
                if (m_nReadIndex >= m_nBufferSize)
                    m_nReadIndex = 0;
                
    return output;
    
}
