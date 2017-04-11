#!/bin/sh

g++  -std=c++11 biquad_example.cpp ../../src/midiman.cpp  Biquad.cpp  -ljackcpp -ljack -llo -lrtmidi -o biquad_example

