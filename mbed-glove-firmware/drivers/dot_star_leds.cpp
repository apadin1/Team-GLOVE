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

void color_to_RGB(DotStarColor color, uint8_t& red, uint8_t& green, uint8_t& blue) {
    switch(color) {
        case(Red):
            red = 255; green = 0; blue = 0; break;
        case(Green):
            red = 0; green = 255; blue = 0; break;
        case(Blue):
            red = 0; green = 0; blue = 255; break;
        case(Orange):
            red = 255; green = 40; blue = 0; break;
        case(Yellow):
            red = 255; green = 230; blue = 0; break;
        case(Maize):
            red = 255; green = 170; blue = 0; break;
        case(Purple):
            red = 180; green = 0; blue = 255; break;
        case(Cyan):
            red = 0; green = 200; blue = 200; break;
        case(Magenta):
            red = 200; green = 0; blue = 200; break;
        case(Pink):
            red = 250; green = 40; blue = 40; break;
        case(Off):
            red = 0; green = 0; blue = 0; break;
        case(White):
        default:
            red = 200, green = 200, blue = 200;
    }
}

void DotStarLEDs::set_color(uint8_t led, DotStarColor color, uint8_t brightness) {
    uint8_t red, green, blue;
    color_to_RGB(color, red, green, blue);
    set_RGB(led, red, green, blue, brightness);
}

void DotStarLEDs::set_color_all(DotStarColor color, uint8_t brightness) {
    uint8_t red, green, blue;
    color_to_RGB(color, red, green, blue);
    set_RGB_all(red, green, blue, brightness);
}

void DotStarLEDs::set_RGB(uint8_t led, uint8_t red,
        uint8_t green, uint8_t blue, uint8_t brightness) {

    set_led_rgb(led, red, green, blue, brightness);
    flush_to_spi();
}

void DotStarLEDs::set_RGB_all(uint8_t red, uint8_t green, uint8_t blue,
        uint8_t brightness) {

    for (uint8_t led = 0; led < num_leds; ++led) {
        set_led_rgb(led, red, green, blue, brightness);
    }
    flush_to_spi();
}

void DotStarLEDs::set_led_rgb(uint8_t led, uint8_t red, uint8_t green,
        uint8_t blue, uint8_t brightness) {

    uint32_t new_led_val = ((0xE0 | brightness) << 24) | (blue << 16) | (green << 8) | red;

    if (leds[led+1] != new_led_val) {
        leds[led+1] = new_led_val;
        clean = false;
    }
}

void DotStarLEDs::flush_to_spi() {
    if (clean) {
        return;
    }
    clean = true;
    // for every 4-byte word in the buffer, write it
    uint32_t word;
    for (uint8_t i = 0; i < num_leds+2; ++i) {
        word = leds[i];
        spi.write((word >> 24) & 0x000000FF);
        spi.write((word >> 16) & 0x000000FF);
        spi.write((word >> 8) & 0x000000FF);
        spi.write((word) & 0x000000FF);
    }
}

float map_unsigned_analog_to_percent(uint16_t min_, uint16_t max_, uint16_t val) {
    return (val - min_) / float(max_ - min_);
}
float map_float_analog_to_percent(float min_, float max_, float val) {
    return (val - min_) / float(max_ - min_);
}

