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
 *  Implements the compression/decompression for glove sensors
 */

#include "glove_sensors.h"

void compressGloveSensors(glove_sensors_raw_t* raw, glove_sensors_compressed_t* compressed) {
    compressed->t = raw->touch_sensor.pack();

    compressed->f0 = raw->flex_sensors[0];
    compressed->f1 = raw->flex_sensors[1];
    compressed->f2 = raw->flex_sensors[2];
    compressed->f3 = raw->flex_sensors[3];

    compressed->r = raw->imu.orient_roll;
    compressed->p = raw->imu.orient_pitch;
    compressed->y = raw->imu.orient_yaw;
}

void extractGloveSensors(glove_sensors_raw_t* raw, glove_sensors_compressed_t* compressed) {
    raw->touch_sensor.a = (compressed->t & 0x8) >> 3;
    raw->touch_sensor.b = (compressed->t & 0x4) >> 2;
    raw->touch_sensor.c = (compressed->t & 0x2) >> 1;
    raw->touch_sensor.d = (compressed->t & 0x1);

    raw->flex_sensors[0] = compressed->f0;
    raw->flex_sensors[1] = compressed->f1;
    raw->flex_sensors[2] = compressed->f2;
    raw->flex_sensors[3] = compressed->f3;

    raw->imu.orient_roll = compressed->r;
    raw->imu.orient_pitch = compressed->p;
    raw->imu.orient_yaw = compressed->y;
}


