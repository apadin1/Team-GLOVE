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

#include <FreeRTOS.h>
#include <semphr.h>

#include "flex_sensor.h"
#include "touch_sensor.h"
#include "imu.h"

#define GLOVE_N_FLEX_SENSORS 6


/*
 * This is the condenced sensor data struct used on either
 * end of the bluetooth connection, must remain in sync on both
 * ends of the connection
 */
typedef struct {

} glove_sensors_raw_t;

/*
 * This version is the container for all the sensors on the glove,
 * using their top-level interfaces.
 */
typedef struct {
    flex_sensor_t* flex_sensors[GLOVE_N_FLEX_SENSORS];
    touch_sensor_t* touch_sensor;
    bno_imu_t* imu;
} glove_sensors_interfaced_t;


#endif /* GLOVE_SENSORS_H_ */

