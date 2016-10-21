/*
 * Filename:  IMU.h
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
 */

#ifndef IMU_H_
#define IMU_H_

#include "Adafruit_BNO055.h" // will need to port

typedef struct {

    uint32_t orient_x;
    uint32_t orient_y;
    uint32_t orient_z;

    uint32_t accel_x;
    uint32_t accel_y;
    uint32_t accel_z;

} bno_imu_t;


/*
 * Need some way to save the calibration for the IMU
 * and then reload it on setup
 */
void saveIMUCalibration();
void loadIMUCalibration();

/*
 * Initialization requires setting some defaults for using
 * the sensor, handle all that here
 *
 * Return a pointer to the allocated imu
 */
bno_imu_t* initIMU();

/*
 * Update the orientation and acceleration information
 */
void updateIMU(bno_imu_t*);

#endif /* IMU_H_ */

