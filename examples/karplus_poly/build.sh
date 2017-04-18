#!/bin/sh

g++  -std=c++11 karplus_poly.cpp ../../src/midimanOld.cpp ../../src/TPTFilter.cpp ../../src/OneZero.cpp ../../src/DelayModule.cpp ../../src/DelayLineSimple.cpp ../../src/DelayA.cpp ../../src/sinusoid.cpp ../../src/oscillator.cpp ../../src/envelope.cpp ../../src/guitarstring.cpp ../../src/stringManager.cpp ../../src/retune.cpp ../../src/FFTConvolver/FFTConvolver.cpp ../../src/FFTConvolver/AudioFFT.cpp ../../src/FFTConvolver/Utilities.cpp -ljack -ljackcpp -lrtmidi  -o karplus_poly
