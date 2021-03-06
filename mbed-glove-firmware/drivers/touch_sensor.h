/*
 * Filename:  touch_sensor.h * Project:   EECS 473 - Team GLOVE
 * Date:      Fall 2016
 * Authors:
 *     Nick Bertoldi
 *     Ben Heckathorn
 *     Ryan O’Keefe
 *     Adrian Padin
 *     Tim Schumacher
 *
 * Purpose: *   Top-level interface for working with capacitive sensor chip,
 *   which uses I2C and can communicate the values of up to 7 sensors
 *
 *   The AT42QT1070 in I2C comms mode can use a single interrupt line
 *   to indicate a change in state of the sensors.
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

#ifndef TOUCH_SENSOR_H_
#define TOUCH_SENSOR_H_

#include <inttypes.h>

#include "mbed.h"

#include "at42qt1070.h"

const PinName TOUCH_I2C_SCL = I2C_SCL0;  // = p7
const PinName TOUCH_I2C_SDA = I2C_SDA0;  // = p30
const PinName TOUCH_INTERRUPT = p16;  // CHANGE interrupt line (active low) (p16 on PCB)
const PinName TOUCH_NO_INTERRUPT = NC; // Don't use an interrupt
const bool TOUCH_DIGITALIN_CHANGE = true; // use DigitalIn instead of InterruptIn

/* Low-Power Mode:
 *  - set multiple of 8ms between key measurements, default 2 (16ms)
 *  - using 0, for best response time
 */
const uint8_t TOUCH_LP_MODE = 0;

/* Max On:
 *  - default is 180 steps of 160ms (28.8 seconds)
 *  - might want this to be off (0)
 *  - using 32 = ~ 5 seconds
 */
const uint8_t TOUCH_MAX_ON = 32;

/* Guard Key:
 *  - default is key 0
 *  - 0..6 valid as guard key, 7 disables guard key function
 */
const uint8_t TOUCH_GUARD_KEY = 7;

/* Negative Threshold:
 *  - default is 20
 *  - unsure what units these are, but more is less sensitive
 */
const uint8_t TOUCH_NTHRESHOLD = 255; //seem to need the maximum to avoid accidental touch/proximity

/* Averaging Factor:
 *  - default is 8
 *  - number of pulses to average for a channel
 *  - values restricted internally to 1,2,4,8,16,32
 */
const uint8_t TOUCH_AVE = 8;

/* Detection Integrator:
 *  - default is 4
 *  - number of consecutive measurementsthat must be confirmed as
 *  having passed the hey threshold before key is registered in detect
 */
const uint8_t TOUCH_DI = 4;

/* Adjacent Key Suppression:
 *  - group 0 disables feature
 *  - 1, 2, 3 are available groups
 *  - only one key in each group can be in detect simultaniously
 *  - group for each key [0..7]
 */
const uint8_t TOUCH_AKS[] = {1, 1, 1, 1, 0, 0, 0}; //{0, 1, 2, 1, 2, 0, 0};

/*
 * Unpacks the _buttonStates byte in AT42QT1070 to
 * the desired keys as struct members
 */
typedef struct {
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;

    uint8_t pack() {
        return (a << 3 | b << 2 | c << 1 | d);
    }
} key_states_t;

class TouchSensor {
public:
    /*
     * Default constructor
     *
     * Writes all the static config settings to the sensor
     * Assosciates the update() method with the CHANGE event line
     */
    TouchSensor(PinName sda=TOUCH_I2C_SDA,
                PinName scl=TOUCH_I2C_SCL,
                PinName intr=TOUCH_NO_INTERRUPT);

    /*
     * Alternate constructor - takes refrecne to mbed::I2C object
     *
     * Writes all the static config settings to the sensor
     * Assosciates the update() method with the CHANGE event line
     */
    TouchSensor(I2C& i2c, PinName intr=TOUCH_NO_INTERRUPT);

    /*
     * Write the configuration values defined above
     */
    void writeStaticConfig();

    /*
     * Copy the key states to the given key states struct
     * Internal mutual exclusion is used to exclude update()
     */
    void writeKeys(key_states_t* key_states);

    /*
     * Update the in memory state for the keys,
     * asking for the key states over I2C
     *
     * Measured to take approximately 1.01 ms
     */
    void update();

    /*
     * Do both the update and then write the key states
     *
     * Use with polling
     */
    void updateAndWrite(key_states_t* key_states);

    /*
     * Does a soft reset then re-configured the chip
     */
    void reset();

    /*
     * Callback for the change event interrupt line
     */
    void changeEventHandler();

    /*
     * Task loop for updating the buttons on change event
     *
     * Usage: needs Thread and TouchSensor objects
     *   TouchSensor touch_sensor;
     *   Thread touch_sensor_thread;
     *   touch_sensor_thread.start(&touch_sensor, &TouchSensor::updateTask);
     */
    void updateTask();

    /*
     * This update task check to see if a reset is required,
     * runs the update() function,
     */
    void singleUpdate();

    /*
     * Spawns a thread to run singleUpdate() then yields.
     * Use at the beginning of sensor-polling periodic function.
     */
    void spawnUpdateThread();

    /*
     * If using spawnUpdateThread MUST be called at the end
     * of the same periodic funciton.
     *
     * Terminates the update thread if hung and sets up for a reset
     * on next update called.
     */
    void terminateUpdateThreadIfBlocking();

    /*
     * Print out the given key states
     */
    static void print(Serial& pc, key_states_t&);

private:
    void initialize(PinName intr);

private:
    AT42QT1070 qt;
    key_states_t keys;

    /*
     * Deferred-interrupt based touch sensor
     */
    InterruptIn* change_event;
    Semaphore do_update;

    /* Polling in spawned thread
     *
     * needs_restart signals if the touch sensor was killed
     * while hanging on the bus, thus needs to get restarted
     */
    Thread* update_thread;
    DigitalIn* change_line;
    bool needs_restart;
};

#endif /* TOUCH_SENSOR_H_ */
