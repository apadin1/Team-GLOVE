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

/*
 * Convert double keeping only 2 decimal places
 */
int16_t compress_double(double num) {
    return int(num * 100);
}
double extract_double(int16_t num) {
    return double(num) / 100.0;
}

void compressGloveSensors(glove_sensors_raw_t* raw, glove_sensors_compressed_t* compressed) {
    compressed->f[0] = ((raw->flex_sensors[0] & 0x0FFF) << 4) | ((raw->flex_sensors[1] & 0x0FFF) >> 8);
    compressed->f[1] = ((raw->flex_sensors[1] & 0x00FF) << 8) | ((raw->flex_sensors[2] & 0x0FFF) >> 4);
    compressed->f[2] = ((raw->flex_sensors[2] & 0x000F) << 12) | (raw->flex_sensors[3] & 0x0FFF);

    compressed->t = 0x00FF & raw->touch_sensor.pack();

    compressed->roll = compress_double(raw->imu.orient_roll);
    compressed->pitch = compress_double(raw->imu.orient_pitch);
    compressed->yaw = compress_double(raw->imu.orient_yaw);
}

void extractGloveSensors(glove_sensors_raw_t* raw, glove_sensors_compressed_t* compressed) {
    raw->flex_sensors[0] = compressed->f[0];
    raw->flex_sensors[1] = compressed->f[1];
    raw->flex_sensors[2] = compressed->f[2];
    raw->flex_sensors[3] = compressed->f[3];

    raw->touch_sensor.a = (compressed->t & 0x8) >> 3;
    raw->touch_sensor.b = (compressed->t & 0x4) >> 2;
    raw->touch_sensor.c = (compressed->t & 0x2) >> 1;
    raw->touch_sensor.d = (compressed->t & 0x1);

    raw->imu.orient_roll = extract_double(compressed->roll);
    raw->imu.orient_pitch = extract_double(compressed->pitch);
    raw->imu.orient_yaw = extract_double(compressed->yaw);
}
