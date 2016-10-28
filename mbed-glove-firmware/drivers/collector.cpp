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
         TouchSensor* _touch, Serial* _pc) {
     : flex(_flex), imu(_imu), touch(_touch), pc(_pc) {}
 }

 Collector::get_data() {
    flex.writeSensors(glove_data.flex_sensors);
    imu.writeSensors(glove_data.imu);
    touch.writeKeys(glove_data.touch_sensor);
  }

 Collector::transmitData() {
    //Check to see if lock is active
       //Transmit data to reciever
  }

 Collector::update() {
    glove_mutex.lock();
    flex.writeSensors(glove_data.flex_sensors);
    imu.writeSensors(glove_data.imu);
    touch.writeKeys(glove_data.touch_sensor);
    glove_mutex.unlock();
 }

 Collector::startUpdateTask(float period_s) {
    update_task_timer->start(period_s);
 }

 Collector::stopUpdateTask() {
    update_task_timer->stop();
 }

