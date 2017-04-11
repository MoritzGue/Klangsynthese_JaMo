#!/bin/sh

g++ -std=c++11 jack_sampler.cpp ../../src/midiman.cpp ../../src/singlesample.cpp -ljackcpp -ljack -lsndfile -lrtmidi -o jack_sampler
