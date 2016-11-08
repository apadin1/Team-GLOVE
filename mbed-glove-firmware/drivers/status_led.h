/*
 * Filename:  touch_sensor.cpp
 * Project:   EECS 473 - Team GLOVE
 * Date:      Fall 2016
 * Authors:
 *     Nick Bertoldi
 *     Ben Heckathorn
 *     Ryan O’Keefe
 *     Adrian Padin
 *     Tim Schumacher
 *
 * Purpose:
 *   Interface to the DotStar LEDs
 *
 */
#include "mbed.h"

SPI spi(p17, p18, p19); // mosi, miso, sclk
DigitalOut cs(p20);
int do_lights() {
    // Chip must be deselected
    cs = 1;
    // Setup the spi for 8 bit data, high steady state clock,
    // second edge capture, with a 1MHz clock rate
    spi.format(8,2);
    spi.frequency(2000000);
    while (true) {
        // Select the device by seting chip select low
        cs = 0;
        led1 = !led1;
        // Send 0x8f, the command to read the WHOAMI register
        spi.write(0x00);
        spi.write(0x00);
        spi.write(0x00);
        spi.write(0x00);

        spi.write(0xEF);
        spi.write(0x0F);
        spi.write(0x0F);
        spi.write(0x0F);

        spi.write(0xE7);
        spi.write(0x00);
        spi.write(0x00);
        spi.write(0xFF);

        spi.write(0xE7);
        spi.write(0xFF);
        spi.write(0x00);
        spi.write(0x00);

        spi.write(0xE7);
        spi.write(0x00);
        spi.write(0xFF);
        spi.write(0x00);

        spi.write(0xFF);
        spi.write(0xFF);
        spi.write(0xFF);
        spi.write(0xFF);


        // Deselect the device
        cs = 1;
        wait(1);
    }
}
