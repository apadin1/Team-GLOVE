# Team-GLOVE
## EECS 473 Final Project

This project was created as part of a Major Design Experience (MDE) by five senior Computer Engineering students at the University of Michigan. The goal of the project was to create a pair of gloves that could behave as controllers for any PC video game using the standard HID protocol. The gloves collected data from flex sensors in the fingers, capacitive touch sensors in the fingertips, and an IMU mounted on the back of the hand, and transmitted this data wirelessly over bluetooth to a receiver board which translated the data into stardard keyboard and mouse commands. To read more about the results of this project, you can see our [final report](Team%20GLOVE%20Final%20Report.pdf) and [project poster](Team%20GLOVE%20Poster.pptx.pdf) which are included in this repository.

We have decided to make this repository public in the hopes that others could expand upon our work. Our goal is to make video games more fun, intuitive, and immersive, and we hope that you can help.

Sincerely,

Team GLOVE

Nick Bertoldi, Ben Heckathorn, Ryan O'Keefe, Adrian Padin, Tim Schumacher

## Repository Organization
This repository is divided into several sections:
* ConfigApp - Contains code for a Windows app which was used to configure the inputs on the glove. It is mostly written in C#
* Hardware - This folder holdsthe eagle and gerber files for the PCB that was designed to run on the glove
* mbed-dongle-firmware - The code that ran on the reciever board (USB dongle) that did the translation and generated keyboard and mouse commands
* mbed-glove-firmware - The software running on the glove itself to collect sensor data and transmit to the receiver

## Compiling mbed-cli
This project was created using the mbed real-time operating system and libraries. It was specifically designed to run on the NRF51822 processor because of the built-in Bluetooth capabilities. 

To get started with the mbed environment, you can check out their website: www.developer.mbed.org

For instructions on how to setup the mbed-cli, see their documentation page: https://docs.mbed.com/docs/mbed-os-handbook/en/5.1/dev_tools/cli/

You will also need GCC ARM embedded compiler, version 4.9, which can be found here: https://launchpad.net/gcc-arm-embedded/4.9/4.9-2015-q3-update

For Linux users, we recommend installing to /usr/local/ (just `tar -xjvf` the download in that dir)

You will also need git, python, and several python dependencies, as described in the mbed-cli instructions

To setup the enviroment in our repo, go into the mbed-glove-firmware folder and type `mbed-deploy` to update the mbed-os lib

## Contributing
This software is licensed under the Apache 2.0 software license. If you would like to contribute, please submit a pull request and we can review your changes before we apply them. If you would like to use this project as a starting point for other projects, consider forking this repository using the Fork button above. Just be sure to cite this repository in your work!
