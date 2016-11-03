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

const PinName COLLECTOR_DEBUG_PIN = p26;
DigitalOut working(COLLECTOR_DEBUG_PIN);

Collector::Collector(FlexSensors* _flex, IMU_BNO055* _imu,
        TouchSensor* _touch, mbed::Serial* _pc)
     : flex(_flex), imu(_imu), touch(_touch), pc(_pc) {

     update_task_timer = new RtosTimer(this, &Collector::update, osTimerPeriodic);
 }


void Collector::transmitData() {
    // Check to see if lock is active
    // Transmit data to reciever

    if (pc != NULL)
    pc->printf("[T]: %hu %hu %hu %hu | [F]: %hu %hu %hu %hu | [O]: %f %f %f \r\n",
            //glove_data.touch_sensor.a, glove_data.touch_sensor.b,
            //glove_data.touch_sensor.c, glove_data.touch_sensor.d,
            1,1,1,1,
            glove_data.flex_sensors[0], glove_data.flex_sensors[1],
            glove_data.flex_sensors[2], glove_data.flex_sensors[3],
            glove_data.imu.orient_pitch, glove_data.imu.orient_roll,
            glove_data.imu.orient_yaw);

}

void Collector::update() {
    working = 1;
    flex->writeSensors(glove_data.flex_sensors);
    touch->writeKeys(&glove_data.touch_sensor);
    imu->writeSensors(&glove_data.imu);

    transmitData();
    working = 0;
}

void Collector::startUpdateTask(float period_s) {
    update_task_timer->start(period_s);
}

void Collector::stopUpdateTask() {
    update_task_timer->stop();
}
