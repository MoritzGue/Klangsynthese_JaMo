#!/bin/sh

g++  -std=c++11 synth_base.cpp ../../src/midiman.cpp ../../src/Biquad.cpp ../../src/DelayModule.cpp ../../src/DelayLineSimple.cpp ../../src/sinusoid.cpp ../../src/oscillator.cpp ../../src/adsr.cpp -ljack -ljackcpp -lrtmidi  -o synth_base
