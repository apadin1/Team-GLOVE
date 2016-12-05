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
 */

#ifndef TRANSLATOR_H_
#define TRANSLATOR_H_

#include <vector>

#include "mbed.h"

#include "analog_button.h"
#include "keyboard_mouse.h"
#include "glove_sensors.h"

#define GESTURE_COUNT 15
#define FLEX_COUNT 4
#define TOUCH_COUNT 4
#define IMU_COUNT 6

enum SENSOR {
    hand,
    flex1,
    flex2,
    flex3,
    flex4,
    touch1,
    touch2,
    touch3,
    touch4,
    pitchup,
    pitchdown,
    rollleft,
    rollright,
    yawleft,
    yawright
};

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
    YAWLEFT,
    YAWRIGHT
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
     *
     */
    Translator(glove_sensors_raw_t* left, glove_sensors_raw_t* right,
               KeyboardMouse* input);

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
    // NOTE: Arrays indexed by enums

    /* Left Glove Analog Buttons */
    flexToHID* flex_sensorsL[FLEX_COUNT];
    imuToHID* imu_axisL[IMU_COUNT];
    touchToHID* touch_sensorsL[TOUCH_COUNT];

    /* Right Glove Analog Buttons */
    //flexToHID* flex_sensorsR[FLEX_COUNT];
    //imuToHID* imu_axisR[IMU_COUNT];
    //touchToHID* touch_sensorsR[TOUCH_COUNT];

    /* KeyboardMouse Object */
    KeyboardMouse* HIDinput;

    /* Glove Data */
    glove_sensors_raw_t* glove_dataL;
    glove_sensors_raw_t* glove_dataR;
};

#endif /* TRANSLATOR_H_ */
