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
 * Copyright (c) 2016 by Nick Bertoldi, Ben Heckathorn, Ryan O'Keefe,
 *                       Adrian Padin, Timothy Schumacher
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef GLOVE_SENSORS_H_
#define GLOVE_SENSORS_H_

#include <inttypes.h>

#include "flex_sensor.h"
#include "touch_sensor.h"
#include "imu.h"
#include "crc.h"

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
 * compressed version of all the
 * sensor data that fits in a BLE advertisement
 */
const uint8_t glove_sensors_compressed_size = 22;
const uint8_t glove_sensors_compressed_size_no_crc = 20;
typedef struct {
    // flex sensors (12*4 = 48b = 6B)
    uint16_t f[3];

    // touch sensors (4*1 = 4b = 1B)
    uint8_t t;

    // pads the touch sensor byte, use this for whatevs
    uint8_t status;

    // imu (3*4B = 12B)
    float roll;
    float pitch;
    float yaw;

    // CRC
    uint16_t checksum;
} glove_sensors_compressed_t;

/*
 * Computes and includes the checksum
 *
 * NOTE: need to call crcInit() before using
 */
void compressGloveSensors(glove_sensors_raw_t* raw, glove_sensors_compressed_t* compressed);
/*
 * Runs the checksum and returns -1 without changing glove_sensors_raw if that fails
 */
int extractGloveSensors(glove_sensors_raw_t& raw, glove_sensors_compressed_t* compressed);

#endif /* GLOVE_SENSORS_H_ */
