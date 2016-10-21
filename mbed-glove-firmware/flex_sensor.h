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
 *
 */

#ifndef FLEX_SENSOR_H_
#define FLEX_SENSOR_H_

#include <inttypes.h>

#include "mbed.h"

#define FLEX_SENSORS_COUNT 4

typedef enum {
    FLEX_0 = A0,
    FLEX_1 = A1,
    FLEX_2 = A2,
    FLEX_3 = A3
} FlexPins;

/* flex_sensor_t
 *
 * ain: mbed AnalogIn object
 * deflection: scaled analog-to-digital value read from the GPIO
 */
typedef struct {
    AnalogIn* ain;
    uint16_t deflection;
} flex_sensor_t;


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
    void updateSensors();

    /*
     * Write the flex sensor values to the given array.
     * This assumes no ownership or locking of the given container
     */
    void writeSensors(uint16_t*);

    /*
     * Alternative interface to both update each pin
     * And write it to the destination buffer
     */
    void updateAndWriteSensors(uint16_t* buf);

private:
    flex_sensor_t sensors[FLEX_SENSORS_COUNT];
};
#endif /* FLEX_SENSOR_H_ */
