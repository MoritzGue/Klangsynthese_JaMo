#!/bin/sh

g++ -std=c++11 fm_example.cpp ../../src/oscman.cpp ../../src/sinusoid.cpp -ljackcpp -ljack -llo -o fm_example
