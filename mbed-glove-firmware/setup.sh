#!/bin/sh
# quick and dirty mbed setup
set -x

sudo pip install mbed-cli

mbed deploy
mkdir mbed-os/.git/info/
mbed deploy

sudo pip install -r mbed-os/requirements.txt

set +x
echo "\nYou're gonna need gcc-arm-none-eabi version 4.9"
