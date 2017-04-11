#!/bin/sh

g++  -std=c++11 sinusoid_example.cpp ../../src/midiman.cpp ../../src/sinusoid.cpp -ljack -ljackcpp -lrtmidi  -o sinusoid_example
