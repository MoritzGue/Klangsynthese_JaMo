#!/bin/sh

g++  -std=c++11  drum_example.cpp  ../../src/sinusoid.cpp  ../../src/midiman.cpp -I../../src -ljackcpp  -ljack -lrtmidi  -o drum_example
