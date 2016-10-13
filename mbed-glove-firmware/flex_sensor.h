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

#include "platform.h"
#include "analogin_api.h"

/* flex_sensor_t
 *
 * analog_pin: GPIO pin number, as determined by the GPIO lib
 *  NOTE: determine analog/ discrete ADC because of the NRF51 processor
 * deflection: scaled analog-to-digital value read from the GPIO
 */
typedef struct {
    analogin_t _adc;
    uint16_t deflection;
} flex_sensor_t;

/*
 * given a single flex sensor, update its deflection
 */
void updateFlexSensorDeflection(flex_sensor_t* f) {
    f->deflection = analogin_read_u16(&(f->_adc));
}

/*
 * initialize the gpio pins for the flex sensor,
 * allocate space for the flex_sensor
 */
flex_sensor_t* initFlexSensor(PinName analog_pin) {
    flex_sensor_t* f = malloc(sizeof(flex_sensor_t));
    analogin_init(*(f->_adc), analog_pin);
}

#endif /* FLEX_SENSOR_H_ */
