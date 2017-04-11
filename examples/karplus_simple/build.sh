#!/bin/sh

g++  -std=c++11 karplus_simple.cpp ../../src/midiman.cpp ../../src/Biquad.cpp ../../src/OneZero.cpp ../../src/DelayModule.cpp ../../src/DelayLineSimple.cpp ../../src/sinusoid.cpp ../../src/oscillator.cpp ../../src/adsr.cpp -ljack -ljackcpp -lrtmidi  -o karplus_simple
