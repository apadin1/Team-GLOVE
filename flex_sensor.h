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

/* flex_sensor_t
 *
 * analog_pin: GPIO pin number, as determined by the GPIO lib
 * deflection: scaled analog-to-digital value read from the GPIO
 */
typedef struct {
    uint8_t analog_pin;
    uint16_t deflection;
} flex_sensor_t;

int get_deflection(flex_sensor_t *sensor);

#endif /* FLEX_SENSOR_H_ */
