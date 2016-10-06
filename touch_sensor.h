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
 *   which uses I2C and can communicate the values of up to 8 sensors
 *
 */

#ifndef TOUCH_SENSOR_H_
#define TOUCH_SENSOR_H_

#include <inttypes.h>

#define TOUCH_SENSOR_NUM 4

/* touch_sensor_t
 *
 * deflection: scaled analog-to-digital value read from the GPIO
 */
typedef struct {
    uint8_t id;
    uint16_t capacitance;
} touch_sensor_t;

/*
 * Given an array of touch_sensor_t, update each
 * of those structs.  Assumes TOUCH_SENSOR_NUM sensors
 */
void updateTouchSensors(touch_sensor_t *sensor);

/*
 * Returns allocated array of [n] touch_sensor_t
 */
touch_sensor_t* initTouchSensorArray(uint8_t n);

#endif /* TOUCH_SENSOR_H_ */
