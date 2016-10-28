/*
 * Filename:  flex_sensor.h
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
 *  Top-level interface to the flex sensors
 */

#ifndef FLEX_SENSOR_H_
#define FLEX_SENSOR_H_

#include <inttypes.h>

#include "mbed.h"

#define FLEX_SENSORS_COUNT 4

#if defined(TARGET_NRF51_DK)
const PinName FLEX_0 = A0;
const PinName FLEX_1 = A1;
const PinName FLEX_2 = A2;
const PinName FLEX_3 = A3;
#elif defined(TARGET_MCU_NRF51822)
const PinName FLEX_0 = p1;
const PinName FLEX_1 = p2;
const PinName FLEX_2 = p3;
const PinName FLEX_3 = p4;
#endif

/*
 * Update Period (in seconds)
 *
 * 0.01 s = 10 ms = 100 Hz
 */
const float FLEX_UPDATE_PERIOD = 0.01;

/* flex_sensor_t
 *
 * deflection: scaled analog-to-digital value read from the GPIO
 */
typedef uint16_t flex_sensor_t;


/* FlexSensors
 *
 * Single class to handle the flex sensor analog read objects,
 * with methods to update internally, and write into the
 * marshelled all sensors data structure
 */
class FlexSensors {
public:
    /*
     * Constructor initializes the AnalogIn objects
     */
    FlexSensors();

    /*
     * Update the deflection for all flex sensors
     */
    void update();

    /*
     * Calls the start() method on the periodic update task,
     * an internal timer is set up in the constructor
     */
    void startUpdateTask(float period_s=FLEX_UPDATE_PERIOD);

    /*
     * Calls the stop() method on the periodic update timer,
     */
    void stopUpdateTask();


    /*
     * Write the flex sensor values to the given array.
     * This assumes no ownership or locking of the given container
     */
    void writeSensors(flex_sensor_t* buf);

    /*
     * Alternative interface to both update each pin
     * And write it to the destination buffer
     */
    void updateAndWriteSensors(flex_sensor_t* buf);

    /*
     * Print the value of all the flex sensors for debugging
     */
    void print(Serial& debug_out);

    /*
     * Print the value of one sensor
     */
    void printSingle(Serial& pc, uint8_t index);



private:
    flex_sensor_t values[FLEX_SENSORS_COUNT];
    AnalogIn* pins[FLEX_SENSORS_COUNT];
    Mutex sensors_mutex;
    RtosTimer* update_task_timer;
};
#endif /* FLEX_SENSOR_H_ */
