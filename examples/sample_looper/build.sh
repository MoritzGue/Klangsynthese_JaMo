#!/bin/sh

g++ -std=c++11 sample_looper.cpp ../../src/singlesample.cpp -ljack -lsndfile -ljackcpp -o sample_looper
