//
//  Biquad.cpp
//
//  Created by Nigel Redmon on 11/24/12
//  EarLevel Engineering: earlevel.com
//  Copyright 2012 Nigel Redmon
//
//  For a complete explanation of the Biquad code:
//  http://www.earlevel.com/main/2012/11/26/biquad-c-source-code/
//
//  License:
//
//  This source code is provided as is, without warranty.
//  You may copy and distribute verbatim copies of this document.
//  You may modify and use this source code to create binary code
//  for your own purposes, free or commercial.
//

#include <math.h>
#include "TPTFilter.h"

TPTFilter::TPTFilter() {
    type = bq_type_lowpass;
    sampleRate = 44100.0;
    Fc = 0.50;
    Q = 0.707;
    z1 = z2 = 0.0;
    calcTPTFilter();
}

TPTFilter::TPTFilter(int type, double Fc, double Q, double sampleRate) {
    calcTPTFilter();(type, Fc, Q, sampleRate);
    z1 = z2 = 0.0;
}

TPTFilter::~TPTFilter() {
}

void TPTFilter::setType(int type) {
    this->type = type;
    calcTPTFilter();
}

void TPTFilter::setQ(double Q) {
    this->Q = Q;
    calcTPTFilter();
}

void TPTFilter::setFc(double Fc) {
    this->Fc = Fc;
    calcTPTFilter();
}

    
void TPTFilter::setTPTFilter(int type, double Fc, double Q, double sampleRate) {
    this->type = type;
    this->Q = Q;
    this->Fc = Fc;
    this->sampleRate = sampleRate;
    calcTPTFilter();
}

void TPTFilter::calcTPTFilter(void) {

    
    switch (this->type) {
        case bq_type_lowpass:
            g = tan(M_PI * Fc / sampleRate);
            h = 1.0 / (1 + g / Q + g*g);
            
            
            break;
            
        case bq_type_highpass:
  
            break;
            
        case bq_type_bandpass:

            break;
            
        case bq_type_notch:

            break;
            
        /*case bq_type_peak:
            if (peakGain >= 0) {    // boost

            }
            else {    // cut

            }
            break;
        
        case bq_type_lowshelf:
            if (peakGain >= 0) {    // boost

            }
            else {    // cut

            }
            break;
        
        case bq_type_highshelf:
            if (peakGain >= 0) {    // boost

            }
            else {    // cut

            }
            break;*/
    }
    
    return;
}
