/*
 * Filename:  touch_sensor.h
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
 *   Top-level interface for working with capacitive sensor chip,
 *   which uses I2C and can communicate the values of up to 7 sensors
 *
 *   The AT42QT1070 in I2C comms mode can use a single interrupt line
 *   to indicate a change in state of the sensors.
 *
 *   NEED TODO:
 *    - deferred interrupt in
 *    - callback on the interrupt to update the touch sensors
 *      - priority lower than the total updater
 *      - callback WaitingSemaphore state from mbed::Thread
 *
 *   Sensor Calibration:
 *     - max on -> recalibration from 160ms - 40800ms (can disable)
 *     - Threshold -> number of samples before change in state (10)
 *
 *   Guard Channel:
 *    - decide if we even need one...
 *
 *   Low-Power Mode:
 *    - set multiple of 8ms between key measurements, default 2 (16ms)
 *    - using 0, for best response time
 *
 *   Max On:
 *    - default is 180 (28.8 seconds)
 *    - might want this to be off
 *
 *   Calibrate:
 *    - can send into "calibration cycle" with any non-zero value
 *
 *   Reset:
 *    - soft reset with any non-zero value
 *    - hard reset with minimum 5us pulse to reset line
 *
 */

#ifndef TOUCH_SENSOR_H_
#define TOUCH_SENSOR_H_

#include <inttypes.h>

#include "mbed.h"
#include "rtos.h"

#include "at42qt1070.h"

const PinName TOUCH_I2C_SCL = I2C_SCL0;  // = p7
const PinName TOUCH_I2C_SDA = I2C_SDA0;  // = p30
const PinName TOUCH_CHANGE = p13; // CHANGE interrupt line (active low)
                                  // stays low until read to the status bytes

/*
 * touch_sensor_t
 */
typedef struct {
    uint8_t id;
    uint16_t capacitance;
} touch_sensor_t;

/*
 * Unpacks the _buttonStates byte in AT42QT1070 to
 * the desired keys as struct members
 */
typedef struct {
    uint8_t a : 1;
    uint8_t b : 1;
    uint8_t c : 1;
    uint8_t d : 1;
} key_states_t;

/*
 * Callback to update the in memory state for the four touch sensors,
 */
void updateTouchSensors(touch_sensor_t *sensor);

#endif /* TOUCH_SENSOR_H_ */
