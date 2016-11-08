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

const uint8_t DOT_STAR_DEFAULT_BRIGHTNESS = 4; // 5-bit value 0-31

class DotStarLEDs {
public:

    /*
     * Initialize a string of DotStar LEDs,
     * given the number of the leds in the strand,
     * and the pins for the SPI output
     *
     * NOTE: MISO must be provided but is not used by the LEDs
     */
    DotStarLEDs(uint8_t num_leds=2, PinName mosi=DOT_STAR_MOSI,
            PinName miso=DOT_STAR_MISO, PinName sclk=DOT_STAR_SCLK);
    /*
     * Select LED in the strand (zero indexed, do your own bounds check)
     * red, green, blue are 0-255
     * birghtness is 0-31
     */
    void set_RGB(uint8_t led, uint8_t red, uint8_t green, uint8_t blue,
            uint8_t brightness=DOT_STAR_DEFAULT_BRIGHTNESS);

    /*
     * Set all LEDs in the strand
     * red, green, blue are 0-255
     * birghtness is 0-31
     */
    void set_RGB_all(uint8_t red, uint8_t green, uint8_t blue,
            uint8_t brightness=DOT_STAR_DEFAULT_BRIGHTNESS);

private:
    uint8_t num_leds;
    SPI spi;
    // buffer with the packed led's values in it (plus start and stop codes)
    uint32_t* leds;

    void flush_to_spi();
};

int do_lights() {

    DotStarLEDs ds_leds(2);

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
