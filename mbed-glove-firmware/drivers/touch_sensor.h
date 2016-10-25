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
 *
 *   Guard Channel:
 *    - decide if we even need one...
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
const PinName TOUCH_CHANGE = p13;  // CHANGE interrupt line (active low)

/* Low-Power Mode:
 *  - set multiple of 8ms between key measurements, default 2 (16ms)
 *  - using 0, for best response time
 */
const uint8_t TOUCH_LP_MODE = 0;

/* Max On:
 *  - default is 180 steps of 160ms (28.8 seconds)
 *  - might want this to be off (0)
 */
const uint8_t TOUCH_MAX_ON = 0;

/* Negative Threshold:
 *  - default is 20
 *  - unsure what units these are, but more is less sensitive
 */
const uint8_t TOUCH_NTHRESHOLD = 40;

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
const uint8_t[] TOUCH_AKS = {0, 0, 0, 0, 0, 0, 0};

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

class TouchSensor {
public:
    /*
     * Default constructor
     */
    TouchSensor();

    /*
     * Write the configuration values defined above
     */
    bool writeStaticConfig();

private:
    key_states_t keys;
    AT42QT1070 qt;
}

/*
 * Callback to update the in memory state for the four touch sensors,
 */
void updateTouchSensors(touch_sensor_t *sensor);

#endif /* TOUCH_SENSOR_H_ */
