/*
 * Filename:  glove_sensors.h
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
 *  This is the definition of the struct used on either end
 *  of the bluetooth connection, and it contains synchronization things.
 *
 */

#ifndef GLOVE_SENSORS_H_
#define GLOVE_SENSORS_H_

#include "flex_sensor.h"
#include "touch_sensor.h"
#include "imu.h"

/*
 * Condenced sensor data struct used on either
 * end of the bluetooth connection
 */
typedef struct {
    flex_sensor_t flex_sensors[FLEX_SENSORS_COUNT];
    key_states_t touch_sensor;
    bno_imu_t imu;
} glove_sensors_raw_t;

#endif /* GLOVE_SENSORS_H_ */

