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

/*
 * Needs to be, like, <26 B
 *
 * With no linear accelerations, 19B
 */
typedef struct {
    // flex sensors (12*4 = 48b = 6B)
    uint16_t f0 : 12;
    uint16_t f1 : 12;
    uint16_t f2 : 12;
    uint16_t f3 : 12;

    // touch sensors (4*1 = 4b = 1B)
    uint8_t t;

    // imu (3*4B = 12B | 6*4B = 24B)
    float r;
    float p;
    float y;
} glove_sensors_compressed_t;

void compressGloveSensors(glove_sensors_raw_t* raw, glove_sensors_compressed_t* compressed) {
    compressed->f0 = raw->flex_sensors[0];
    compressed->f1 = raw->flex_sensors[1];
    compressed->f2 = raw->flex_sensors[2];
    compressed->f3 = raw->flex_sensors[3];

    compressed->t = raw->touch_sensor.pack();

    compressed->r = raw->imu.orient_roll;
    compressed->p = raw->imu.orient_pitch;
    compressed->y = raw->imu.orient_yaw;
}

void extractGloveSensors(glove_sensors_raw_t* raw, glove_sensors_compressed_t* compressed) {
    raw->flex_sensors[0] = compressed->f0;
    raw->flex_sensors[1] = compressed->f1;
    raw->flex_sensors[2] = compressed->f2;
    raw->flex_sensors[3] = compressed->f3;

    raw->touch_sensor.a = (compressed->t & 0x8) >> 3;
    raw->touch_sensor.b = (compressed->t & 0x4) >> 2;
    raw->touch_sensor.c = (compressed->t & 0x2) >> 1;
    raw->touch_sensor.d = (compressed->t & 0x1);

    raw->imu.orient_roll = compressed->r;
    raw->imu.orient_pitch = compressed->p;
    raw->imu.orient_yaw = compressed->y;
}

#endif /* GLOVE_SENSORS_H_ */

