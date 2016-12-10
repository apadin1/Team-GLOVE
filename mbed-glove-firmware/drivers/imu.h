/*
 * Filename:  imu.h
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
 *  Top-level interface for a task to get data from the IMU
 *
 *  Uses the BNO055_fusion mbed library by Kenji Arai
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

#ifndef IMU_H_
#define IMU_H_

#include "BNO055.h"
/*
 * PinName definitions for the BNO055 on the board
 */
const PinName IMU_RST = p20;

/*
 * Update Period (in milliseconds)
 */
const uint32_t IMU_UPDATE_PERIOD = 10;

/*
 * Section 3.4 Axis Remap, p.25
 * On prototype in P1
 * On PCB in P1
 */
const BNO055_MOUNT_POSITIONS_TypeDef IMU_MOUNT_POSITION = MT_P1;

/* bno_imu_t
 *
 * Euler orientation in degrees (pitch, roll, yaw)
 * Linear acceleration in m/s^2 (gravity not included!)
 *
 * This structure can be directly serialized
 * and sent over BLE
 */
typedef struct {
    float orient_pitch;
    float orient_roll;
    float orient_yaw;

    float accel_x;
    float accel_y;
    float accel_z;
} bno_imu_t;

class IMU_BNO055 {
public:
    /*
     * Constructor initializes the BNO055 and takes care
     * of the default configuration.
     */
    IMU_BNO055(I2C& i2c);

    /*
     * Update the orientation and acceleration information
     * Callback for the periodic mode
     */
    void update();

    /*
     * Write the imu orientation values to the given struct
     * This assumes no ownership or locking of the given container
     */
    void writeSensors(bno_imu_t*);

    /*
     * Single function call for manual polling
     * Calls update then writes to the provided
     */
    void updateAndWrite(bno_imu_t*);

    /*
     * For debugging
     */
    void print(Serial& debug_out);

    uint8_t hwDebugCheckVal () {
        return imu.read_calib_status();
    }

private:
    BNO055 imu;
    bno_imu_t imu_data;

    BNO055_ID_INF_TypeDef bno055_id_inf;
    BNO055_EULER_TypeDef euler_angles;
    BNO055_LIN_ACC_TypeDef linear_acc;
};

#endif /* IMU_H_ */
