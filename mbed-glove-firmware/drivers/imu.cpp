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

IMU_BNO055::IMU_BNO055(Serial& pc)
    : imu(IMU_I2C_SDA, IMU_I2C_SCL, IMU_RST, BNO055_G_CHIP_ADDR, MODE_NDOF) {

    imu.set_mounting_position(IMU_MOUNT_POSITION);

    if (imu.chip_ready() == 0) {
        do {
            pc.printf("Bosch BNO055 is NOT avirable!!\r\n Reset\r\n");
        } while (imu.reset());
    }

    pc.printf("AXIS_REMAP_CONFIG:0x%02x, AXIS_REMAP_SIGN:0x%02x\r\n",
              imu.read_reg0(BNO055_AXIS_MAP_CONFIG),
              imu.read_reg0(BNO055_AXIS_MAP_SIGN));
    imu.read_id_inf(&bno055_id_inf);
    pc.printf("CHIP ID:0x%02x, ACC ID:0x%02x, MAG ID:0x%02x, GYR ID:0x%02x, ",
              bno055_id_inf.chip_id, bno055_id_inf.acc_id, bno055_id_inf.mag_id,
              bno055_id_inf.gyr_id);
    pc.printf("SW REV:0x%04x, BL REV:0x%02x\r\n", bno055_id_inf.sw_rev_id,
              bno055_id_inf.bootldr_rev_id);
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

void IMU_BNO055::print(Serial& pc) {
    pc.printf("[E] %+6.1f Yaw, %+6.1f Pitch, %+6.1f Roll, ",
            imu_data.orient_yaw, imu_data.orient_pitch, imu_data.orient_roll);
    pc.printf(" [L] %+6.1f X, %+6.1f Y, %+6.1f Z\r\n",
            imu_data.accel_x, imu_data.accel_y, imu_data.accel_z);
}
