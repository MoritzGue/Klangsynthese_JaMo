#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>
#include <jackaudioio.hpp>
#include <sndfile.h>
#include "../FFTConvolver.h"
#include "../Utilities.h"
#include "../TwoStageFFTConvolver.h"
#include <vector>

#include "../../singlesample.h"





int main(){



fftconvolver::TwoStageFFTConvolver *convolver;
convolver = new fftconvolver::TwoStageFFTConvolver();

SingleSample *singleSample;
singleSample = new SingleSample("Guitarimpulse2.wav");

//


double* ir = singleSample->get_x();

std::vector<float>ir_float;

for(int i=0,i<)

//std::cout<<ir_new[45]<<std::endl;




//convolver->init(64,256,ir,irLen)

};
