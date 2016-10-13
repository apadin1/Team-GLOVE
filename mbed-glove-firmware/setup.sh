#!/bin/sh
# quick and dirty mbed setup
set -x

sudo pip install mbed-cli

mbed deploy
mkdir mbed-os/.git/info/
mbed deploy

sudo pip install -r mbed-os/requirements.txt

set +x
echo "\n Don't forget to install GCC ARM embedded 4.9 https://launchpad.net/gcc-arm-embedded/4.9/4.9-2015-q3-update"
