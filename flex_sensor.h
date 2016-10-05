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
 *
 */

#ifndef FLEX_SENSOR_H_
#define FLEX_SENSOR_H_

struct flex_sensor {
    int analog_pin;
    int deflection;
};

int get_deflection();

#endif /* FLEX_SENSOR_H_ */

