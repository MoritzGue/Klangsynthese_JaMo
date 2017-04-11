#!/bin/sh

g++ -std=c++11 va_filter.cpp ../../src/midiman.cpp -ljackcpp -ljack -lrtmidi -o va_filter

