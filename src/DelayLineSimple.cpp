#include "DelayLineSimple.h"

DelayLineSimple::DelayLineSimple(double sampleRate, int bufferSizeInSamples){
    
    m_fDelayInSamples = 200.0;
    m_fDelay_ms = 0;
    
    //reset
    m_nReadIndex = 0;
    m_nWriteIndex = 0;
    
    //no buffer yet because we don't have sample rate yet
    m_pBuffer = NULL;
    m_nBufferSize = 0;
    
    xn_1 = 0.0;
    yn_1 = 0.0;
    alpha_ = 0.0;
    coeff_ = 0.0;
    
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    m_nSampleRate = sampleRate;
    
    //setup the delay line
    m_nBufferSize = bufferSizeInSamples;
    
    //delete Buffer if it exsits
    if(m_pBuffer)
        delete [] m_pBuffer;

    //create teh new buffer
    m_pBuffer = new double[m_nBufferSize];
    
    //reset
    resetDelay();


    //then
    //cookVariables();
}

DelayLineSimple::~DelayLineSimple(){
    delete [] m_pBuffer;
}

void DelayLineSimple::resetDelay(){
    //flush Buffer
    if(m_pBuffer)
        memset(m_pBuffer, 0, m_nBufferSize*sizeof(double));
    
    //init read/write indicies
    m_nWriteIndex = 0; //reset write index to top
    m_nReadIndex = 0; //reset read index to top
    
}

void DelayLineSimple::setDelayInSamples(double delayInSamples){
    m_fDelayInSamples = delayInSamples;
    cookVariables();
}

void DelayLineSimple::setDelayInMs(double delayInMs){
    m_fDelay_ms = delayInMs;
    m_fDelayInSamples = m_fDelay_ms*(m_nSampleRate/1000.0);  // 2 seconds delay @ fs
    cookVariables();
}

void DelayLineSimple::cookVariables(){

    // subtract to make read index
    m_nReadIndex = m_nWriteIndex - (int)m_fDelayInSamples +1;
    
    // check and wrap backwards
    if (m_nReadIndex < 0)
        m_nReadIndex += m_nBufferSize;  // amount of wrap is Read + Length
 
    
    // Allpass calculation for frac delay (tuning filter)
    // see also: https://ccrma.stanford.edu/~jos/Interpolation/
    alpha_ = m_fDelayInSamples - floor(m_fDelayInSamples);
    
    if ( alpha_ < 0.5 ) {
        // The optimal range for alpha is about 0.5 - 1.5 in order to
        // achieve the flattest phase delay response.
        m_nReadIndex += 1;
        if ( m_nReadIndex >= m_nBufferSize ) m_nReadIndex -= m_nBufferSize;
        alpha_ += (double) 1.0;
    }
    coeff_ = ((1.0 - alpha_) / (1.0 + alpha_));  // coefficient for allpass
}



