#!/bin/sh

g++  -std=c++11 karplus_poly.cpp ../../src/midiman.cpp ../../src/Biquad.cpp ../../src/OneZero.cpp ../../src/DelayModule.cpp ../../src/DelayLineSimple.cpp ../../src/sinusoid.cpp ../../src/oscillator.cpp ../../src/adsr.cpp ../../src/guitarstring.cpp ../../src/retune.cpp -ljack -ljackcpp -lrtmidi  -o karplus_poly
