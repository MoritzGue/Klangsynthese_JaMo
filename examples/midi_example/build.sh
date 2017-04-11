#!/bin/sh

g++  -std=c++11 midi_class_example.cpp ../../src/midiman.cpp    -lrtmidi -o midi_class

