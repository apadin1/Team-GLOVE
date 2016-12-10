/*
 * Filename:  translator.h
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
 *  Translator to interpret glove data as HID input
 *
 * Copyright (c) 2016 by Nick Bertoldi, Ben Heckathorn, Ryan O'Keefe,
 *                       Adrian Padin, Timothy Schumacher
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef TRANSLATOR_H_
#define TRANSLATOR_H_

#include "mbed.h"

#include "analog_button.h"
#include "keyboard_mouse.h"
#include "glove_sensors.h"
#include "gpio.h"

#define FLEX_COUNT 4
#define TOUCH_COUNT 4
#define IMU_COUNT 4

enum FLEX {
    FLEX1,
    FLEX2,
    FLEX3,
    FLEX4
};

enum TOUCH {
    TOUCH1,
    TOUCH2,
    TOUCH3,
    TOUCH4
};

enum IMU {
    PITCHUP,
    PITCHDOWN,
    ROLLLEFT,
    ROLLRIGHT,
};

/* Translator
 *
 * Single class to handle data interpretation between
 * glove data and HID input. Methods are written
 * to update internally, and to write all methods into a
 * data structure
 */
class Translator {
public:
    /*
     * Constructor for translator
     */
    Translator(glove_sensors_raw_t& glove_data,
               KeyboardMouse& HIDinput,
               bool is_left,
               flexToHID* f,
               touchToHID* t,
               imuToHID* i);

    /*
     * Update gesture mapping via new configuration arrary.
     */
    void updateGestureMap(uint8_t* config);

    /*
     * Analyze sensors to determine if gesture
     * is occuring. If so, generate proper HID data to be sent to HID class.
     * This function designed to be set up as a periodic task.
     */
    void gestureCheck();

private:
    void handleKeyboardInput(keyboardData&);
    void handleMouseInput(mouseData&);

private:
    /* Glove Data */
    glove_sensors_raw_t& glove_data;

    /* KeyboardMouse Object */
    KeyboardMouse& HIDinput;
    bool is_left;

    /* Left Glove Analog Buttons */
    flexToHID* flex_sensors;
    touchToHID* touch_sensors;
    imuToHID* imu_axis;
};

#endif /* TRANSLATOR_H_ */
