#!/bin/sh

g++ -O3 -std=c++11 karplus_poly.cpp ../src/midiman.cpp ../src/TPTFilter.cpp ../src/OneZero.cpp ../src/DelayLineSimple.cpp ../src/oscillator.cpp ../src/envelope.cpp ../src/guitarstring.cpp ../src/stringManager.cpp ../src/singlesample.cpp ../src/FFTConvolver/FFTConvolver.cpp ../src/FFTConvolver/AudioFFT.cpp ../src/FFTConvolver/Utilities.cpp -ljack -lsndfile -ljackcpp -lrtmidi  -o karplus_poly
