#!/bin/sh

# Onboard Sound:
#jackd -dalsa -r44100 -p2048 -n3 &

# Focusrite:
#jackd -dalsa -d hw:USB -p256  -n3 -r44100 &

# Renkforce:
jackd -dalsa -d hw:Device -p64 -n2 -r44100 &
