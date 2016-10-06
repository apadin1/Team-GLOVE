/*
 * Filename:  heartrate_sensor.h
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

#ifndef HEARTRATE_SENSOR_H_
#define HEARTRATE_SENSOR_H_


#include <FreeRTOS.h>

#define SMOOTHING_WINDOW 5;

typedef struct {
    TickType_t last_pulse_ticks[SMOOTHING_WINDOW];
    uint8_t analog_pin;
} heart_rate_sensor_t;

void initHeartRate(heart_rate_sensor_t *, uint8_t analog_pin);
uint16_t readHeartRateValue(heart_rate_sensor_t *);
float getHeartRate(heart_rate_sensor_t *);

#endif /* HEARTRATE_SENSOR_H_ */

