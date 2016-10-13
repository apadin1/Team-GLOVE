#!/bin/sh
# quick and dirty mbed setup
set -x

mbed deploy
mkdir mbed-os/.git/info/
mbed deploy
