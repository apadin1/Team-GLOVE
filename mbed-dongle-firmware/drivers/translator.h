/*
 * Filename:  translator.h
 * Project:   EECS 473 - Team GLOVE
 * Date:      Fall 2016
 * Authors:
 *     Nick Bertoldi
 *     Ben Heckathorn
 *     Ryan O’Keefe
 *     Adrian Padin *     Tim Schumacher
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
               KeyboardMouse& HIDinput);

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

    /*
     * Calls the start() method on the periodic update task,
     * an internal timer is set up in the constructor
     */
    void startUpdateTask(uint32_t ms);

    /*
     * Calls the stop() method on the periodic update timer,
     */
    void stopUpdateTask();

private:
    void handleKeyboardInput(keyboardData&);
    void handleMouseInput(mouseData&);

private:
    // NOTE: Arrays indexed by enums

    /* Left Glove Analog Buttons */
    flexToHID* flex_sensors[FLEX_COUNT];
    imuToHID* imu_axis[IMU_COUNT];
    touchToHID* touch_sensors[TOUCH_COUNT];

    /* KeyboardMouse Object */
    KeyboardMouse& HIDinput;

    /* Glove Data */
    glove_sensors_raw_t& glove_data,

    RtosTimer* update_task_timer;
    DigitalOut working;
};

#endif /* TRANSLATOR_H_ */
