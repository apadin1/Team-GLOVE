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
 */

#ifndef IMU_H_
#define IMU_H_

#include "BNO055.h"
/*
 * PinName definitions for the BNO055 on the board
 */
const PinName IMU_I2C_SCL = I2C_SCL0;  // = p7
const PinName IMU_I2C_SDA = I2C_SDA0;  // = p30
const PinName IMU_RST = p12;


/*
 * Update Period (in seconds)
 *
 * 0.01 s = 10 ms = 100 Hz
 */
const float IMU_UPDATE_PERIOD = 0.01;

/*
 * TODO depending on layout for the PCB, figure this out
 * RTD
 */
const BNO055_MOUNT_POSITIONS_TypeDef IMU_MOUNT_POSITION = MT_P6;

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
     *
     * TODO Determine pins vs pass ref to I2C0
     *
     * XXX Using Serial& for debug
     */
    IMU_BNO055(Serial& debug_out);

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

    /*
     * For debuggs
     */
    void print(Serial& debug_out);

private:
    BNO055 imu;
    bno_imu_t imu_data;

    BNO055_ID_INF_TypeDef bno055_id_inf;
    BNO055_EULER_TypeDef euler_angles;
    BNO055_LIN_ACC_TypeDef linear_acc;
};

/*
 * Need some way to save the calibration for the IMU
 * and then reload it on setup
 */
void saveIMUCalibration();
void loadIMUCalibration();

#endif /* IMU_H_ */
