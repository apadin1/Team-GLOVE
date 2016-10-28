/*
 * Filename:  collector.cpp
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
 *  Implementation for the Collector class in collector.h
 *
 */

#include "collector.h"

Collector::Collector(FlexSensors* _flex, IMU_BNO055* _imu,
        TouchSensor* _touch, mbed::Serial* _pc)
     : flex(_flex), imu(_imu), touch(_touch), pc(_pc) {


         // RtosTimer definition
     }


void Collector::transmitData() {
    // Check to see if lock is active
    // Transmit data to reciever

    pc->printf("[T]: %hu %hu %hu %hu | [F]: %hu %hu %hu %hu | [O]: %f %f %f \r\n",
            glove_data.touch_sensor.a, glove_data.touch_sensor.b,
            glove_data.touch_sensor.c, glove_data.touch_sensor.d,
            glove_data.flex_sensors[0], glove_data.flex_sensors[1],
            glove_data.flex_sensors[2], glove_data.flex_sensors[3],
            glove_data.imu.orient_pitch, glove_data.imu.orient_roll,
            glove_data.imu.orient_yaw);

}

void Collector::update() {
    flex->writeSensors(glove_data.flex_sensors);
    touch->writeKeys(&glove_data.touch_sensor);
    imu->writeSensors(&glove_data.imu);
}

void Collector::startUpdateTask(float period_s) {
    update_task_timer->start(period_s);
}

void Collector::stopUpdateTask() {
    update_task_timer->stop();
}
