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
 *
 */

#ifndef HEARTRATE_SENSOR_H_
#define HEARTRATE_SENSOR_H_

#include <inttypes.h>

#include <FreeRTOS.h>

#define HEARTRATE_SAMPLE_WINDOW 5

/* heartrate_sensor_t
 *
 * analog_pin: GPIO pin number, as determined by the GPIO lib
 * deflection: scaled analog-to-digital value read from the GPIO
 */
typedef struct {
    TickType_t last_pulse_ticks[HEARTRATE_SAMPLE_WINDOW];
    uint8_t analog_pin;
} heartrate_sensor_t;

/*
 * initialize the gpio pins for the heartrate sensor
 */
heartrate_sensor_t* initHeartRate(uint8_t analog_pin);

/*
 * read the current signal value on the heart rate monitor
 */
uint16_t readHeartRateSignalValue(heart_rate_sensor_t*);

/*
 * return the heart rate based on the period of pulses
 *
 * BPM as a uint16_t for now, for debugging, consider uint8_t later
 */
uint16_t getHeartRateBPM(heart_rate_sensor_t*);

#endif /* HEARTRATE_SENSOR_H_ */
