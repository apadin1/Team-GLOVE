/*
 * Filename:  dot_star_leds.h
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

const PinName DOT_STAR_MOSI = p17;
const PinName DOT_STAR_MISO = p18; // not used, must be declared
const PinName DOT_STAR_SCLK = p19;

const uint8_t DOT_STAR_DEFAULT_BRIGHTNESS = 0x9; // 5-bit value

class DotStarLeds {
public:

    DotStarLeds(uint8_t num_leds=2, PinName mosi=DOT_STAR_MOSI,
            PinName miso=DOT_STAR_MISO, PinName sclk=DOT_STAR_SCLK)
        : n_leds(num_leds), spi(mosi, miso, sclk) {

        // Setup the spi for 8 bit data, high steady state clock,
        // second edge capture, with a 8MHz clock rate
        spi.format(8,2);
        spi.frequency(8000000);

        // setup the leds buffer
        // contains the start and stop words
        // can still index as though it didn't have those
        leds = new uint32_t[n_leds + 2];
        leds[0] = 0x00;
        leds[n_leds+1] = 0xFFFFFFFF;
    }

    /*
     * Set brightness, red, green, and blue
     *
     * and select the led you are setting
     */
    void set_RGB(uint8_t led, uint8_t red, uint8_t green, uint8_t blue,
            uint8_t brightness=DOT_STAR_DEFAULT_BRIGHTNESS) {

        leds[led+1] = ((0xE0 | brightness) << 24) | (blue << 16) | (green << 8) | red;
        flush();
    }

    void flush() {
        // for every 4-byte word in the buffer, write it
        // NOTE: tried to do the bytes bit that was borked...
        uint32_t word;
        for (uint8_t i = 0; i < n_leds+2; ++i) {
            word = leds[i];
            spi.write((word >> 24) & 0x000000FF);
            spi.write((word >> 16) & 0x000000FF);
            spi.write((word >> 8) & 0x000000FF);
            spi.write((word) & 0x000000FF);
        }
    }

private:
    uint8_t n_leds;
    SPI spi;

    // buffer with the packed led's values in it
    uint32_t* leds;
};

int do_lights() {

    DotStarLeds ds_leds(2);

    while (true) {
        ds_leds.set_RGB(0, 100, 0, 100);
        ds_leds.set_RGB(1, 0, 0, 100);
        wait(0.8);
        ds_leds.set_RGB(1, 100, 0, 100);
        ds_leds.set_RGB(0, 0, 0, 100);
        wait(0.8);
        /*

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

        spi.write(0xFF);
        spi.write(0xFF);
        spi.write(0xFF);
        spi.write(0xFF);

        wait(1);
        */
    }
}
