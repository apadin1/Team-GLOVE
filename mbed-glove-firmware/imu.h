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
 *  Uses the BNO055_fusion mbed library by Kenji Arai
 */

#ifndef IMU_H_
#define IMU_H_

#include "BNO055.h"

/*
 * PinName definitions for the BNO055 on the board
 */
typedef enum {
    IMU_I2C_SCL = I2C_SCL0; // = p7
    IMU_I2C_SDA = I2C_SDA0; // = p30
    IMU_RST = p12;
} IMUPins;

/* bno_imu_t
 *
 * Euler orientation in degrees (pitch, roll, yaw)
 * Linear acceleration in m/s^2 (gravity not included!)
 *
 * This structure can be directly serialized
 * and sent over BLE
 */
typedef struct {
    uint32_t orient_x;
    uint32_t orient_y;
    uint32_t orient_z;

    uint32_t accel_x;
    uint32_t accel_y;
    uint32_t accel_z;
} bno_imu_t;


class IMU_BNO055 {
public:
    /*
     * Constructor initializes the BNO055 and takes care
     * of the default configuration.
     */
    IMU_BNO055();

    /*
     * something something start periodic task
     */
    // .begin()

    /*
     * something something do manual calibration
     */
    // .manualCalibration()

    /*
     * Write the imu orientation values to the given struct
     * This assumes no ownership or locking of the given container
     */
    void writeSensors(bno_imu_t&);

    /*
     * Update the orientation and acceleration information
     * Callback for the periodic mode
     */
    void update();

private:
    BNO055 imu;
    bno_imu_t imu_data;
}


/*
 * Need some way to save the calibration for the IMU
 * and then reload it on setup
 */
void saveIMUCalibration();
void loadIMUCalibration();

#endif /* IMU_H_ */

