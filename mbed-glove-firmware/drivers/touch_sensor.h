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
 */

#ifndef TOUCH_SENSOR_H_
#define TOUCH_SENSOR_H_

#include <inttypes.h>

#include "mbed.h"

#include "at42qt1070.h"

/*
 * touch_sensor_t
 */
typedef struct {
    uint8_t id;
    uint16_t capacitance;
} touch_sensor_t;

/*
 * Callback to update the in memory state for the four touch sensors,
 */
void updateTouchSensors(touch_sensor_t *sensor);

#endif /* TOUCH_SENSOR_H_ */
