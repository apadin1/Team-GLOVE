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
 * Usage: needs Thread and TouchSensor objects
 *   TouchSensor touch_sensor;
 *   Thread touch_sensor_thread;
 *   touch_sensor_thread.start(&touch_sensor, &TouchSensor::updateTask);
 *
 ************************
 *  NOTES
 *   Calibrate:
 *    - can send into "calibration cycle" with any non-zero value
 *
 *   Reset:
 *    - soft reset with any non-zero value
 *    - hard reset with minimum 5us pulse to reset line
 */

#ifndef TOUCH_SENSOR_H_
#define TOUCH_SENSOR_H_

#include <inttypes.h>

#include "mbed.h"
#include "rtos.h"

#include "at42qt1070.h"

const PinName TOUCH_I2C_SCL = I2C_SCL0;  // = p7
const PinName TOUCH_I2C_SDA = I2C_SDA0;  // = p30
const PinName TOUCH_INTERRUPT = p13;  // CHANGE interrupt line (active low)

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
const uint8_t TOUCH_AKS[] = {0, 0, 0, 0, 0, 0, 0}; //{0, 1, 2, 1, 2, 0, 0};

/*
 * Unpacks the _buttonStates byte in AT42QT1070 to
 * the desired keys as struct members
 */
typedef struct {
    uint8_t a : 1;
    uint8_t b : 1;
    uint8_t c : 1;
    uint8_t d : 1;

    uint8_t pack() {
        return a << 3 | b << 2 | c << 1 | d;
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
                PinName intr=TOUCH_INTERRUPT);

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
     * Update the in memory state for the keys
     */
    void update();

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

private:
    AT42QT1070 qt;
    InterruptIn change_event;
    Semaphore do_update;
    key_states_t keys;
    Mutex keys_mutex;
};

#endif /* TOUCH_SENSOR_H_ */
