/*
 * Filename:  dot_star_leds.cpp
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
 *   Implementation for interface to DotStar LEDs
 *   and some helpful functions
 */

#include "dot_star_leds.h"

DotStarLEDs::DotStarLEDs(uint8_t num_leds,
        PinName mosi, PinName miso, PinName sclk)
    : num_leds(num_leds), spi(mosi, miso, sclk) {

    // Setup the spi for 8 bit data, high steady state clock,
    // second edge capture, with a 8MHz clock rate
    spi.format(8,2);
    spi.frequency(8000000);

    // setup the leds buffer
    // contains the start and stop words
    // can still index as though it didn't have those
    leds = new uint32_t[num_leds + 2];
    leds[0] = 0x00;
    leds[num_leds+1] = 0xFFFFFFFF;
}

void DotStarLEDs::set_RGB(uint8_t led, uint8_t red,
        uint8_t green, uint8_t blue, uint8_t brightness) {

    leds[led+1] = ((0xE0 | brightness) << 24) | (blue << 16) | (green << 8) | red;
    flush_to_spi();
}

void DotStarLEDs::set_RGB_all(uint8_t red, uint8_t green, uint8_t blue,
        uint8_t brightness) {

    for (uint8_t led = 0; led < num_leds; ++led) {
        leds[led+1] = ((0xE0 | brightness) << 24) | (blue << 16) | (green << 8) | red;
    }
    flush_to_spi();
}

void DotStarLEDs::flush_to_spi() {
    // for every 4-byte word in the buffer, write it
    // NOTE: tried to do the bytes but that was borked...
    uint32_t word;
    for (uint8_t i = 0; i < num_leds+2; ++i) {
        word = leds[i];
        spi.write((word >> 24) & 0x000000FF);
        spi.write((word >> 16) & 0x000000FF);
        spi.write((word >> 8) & 0x000000FF);
        spi.write((word) & 0x000000FF);
    }
}

/*
 * Given the upper and lower bound on unsigned analog values,
 * map the value into the range and return as a percent
 */
float map_unsigned_analog_to_percent(uint16_t min_, uint16_t max_, uint16_t val) {
    return (val - min_) / float(max_ - min_);
}
float map_float_analog_to_percent(float min_, float max_, float val) {
    return (val - min_) / float(max_ - min_);
}


