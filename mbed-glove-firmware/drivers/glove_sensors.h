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

int16_t compress_double(double);
double extract_double(int16_t);

/*
 * Needs to be, like, <26 B
 *
 * With no linear accelerations, 19B
 */
const size_t glove_sensors_compressed_size = 24;
typedef struct {
    // flex sensors (12*4 = 48b = 6B)
    uint16_t f[3];

    // touch sensors (4*1 = 4b = 1B)
    uint16_t t;

    // imu (3*4B = 12B | 6*4B = 24B)
    // convert by *100, store as decimal
    float roll;
    float pitch;
    float yaw;

    // CRC
    uint16_t checksum;
} glove_sensors_compressed_t;

void compressGloveSensors(glove_sensors_raw_t* raw, glove_sensors_compressed_t* compressed);
void extractGloveSensors(glove_sensors_raw_t* raw, glove_sensors_compressed_t* compressed);

#endif /* GLOVE_SENSORS_H_ */
