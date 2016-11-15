/*
 * Filename:  imu.cpp
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
 *  Implement the top-level interface with the specific configuration
 *  and core functionality for the GLOVE
 *
 *  Uses the BNO055_fusion mbed library by Kenji Arai
 */

#include "imu.h"

IMU_BNO055::IMU_BNO055(I2C& i2c)
    : imu(i2c, IMU_RST, BNO055_G_CHIP_ADDR, MODE_NDOF) {

    imu.set_mounting_position(IMU_MOUNT_POSITION);

    if (imu.chip_ready() == 0) {
        do {
            wait_ms(10);
        } while (imu.reset());
    }

    imu.read_id_inf(&bno055_id_inf);

    //update_task_timer = new RtosTimer(this, &IMU_BNO055::update, osTimerPeriodic);
}

void IMU_BNO055::update() {
    imu.get_Euler_Angles(&euler_angles);
    imu.get_linear_accel(&linear_acc);

    imu_data.orient_pitch = euler_angles.p;
    imu_data.orient_roll = euler_angles.r;
    imu_data.orient_yaw = euler_angles.h;

    imu_data.accel_x = linear_acc.x;
    imu_data.accel_y = linear_acc.y;
    imu_data.accel_z = linear_acc.z;
}

/*
void IMU_BNO055::startUpdateTask(uint32_t ms) {
    update_task_timer->start(ms);
}

void IMU_BNO055::stopUpdateTask() {
    update_task_timer->stop();
}
*/

void IMU_BNO055::writeSensors(bno_imu_t* imu_) {
    imu_->orient_pitch = imu_data.orient_pitch;
    imu_->orient_roll = imu_data.orient_roll;
    imu_->orient_yaw = imu_data.orient_yaw;
    imu_->accel_x = imu_data.accel_x;
    imu_->accel_y = imu_data.accel_y;
    imu_->accel_z = imu_data.accel_z;
}

void IMU_BNO055::updateAndWriteSensors(bno_imu_t* imu_) {
    update();
    writeSensors(imu_);
}

void IMU_BNO055::print(Serial& pc) {
    pc.printf("Y%+6.1f, P%+6.1f, R%+6.1f\r\n",
            imu_data.orient_yaw, imu_data.orient_pitch, imu_data.orient_roll);

    /*
    pc.printf("[E] %+6.1f Yaw, %+6.1f Pitch, %+6.1f Roll, ",
            imu_data.orient_yaw, imu_data.orient_pitch, imu_data.orient_roll);
    pc.printf(" [L] %+6.1f X, %+6.1f Y, %+6.1f Z\r\n",
            imu_data.accel_x, imu_data.accel_y, imu_data.accel_z);
    */
}
