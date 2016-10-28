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
#include "collector.h";

/*
 * Constructor initializes the Collector class
 */
 Collector::Collector(FlexSensors* flex, IMU_BNO055* imu, TouchSensor* touch) {
    flex_sensors = *flex;
    IMU = *imu;
    touch_sensors = *touch;
 }

/*
 * Collect data from sensors
 */
 Collector::get_data() {
    flex_sensors.writeSensors(glove_data.flex_sensors);
    IMU.writeSensors(glove_data.imu);
    touch_sensors.writeKeys(glove_data.touch_sensor);
  }

/*
 * Transmit collected data to reciever
 *
 * TODO: Complete Implementation w/ BLE
 */
 Collector::transmitData() {
    //Check to see if lock is active
       //Transmit data to reciever
  }

/*
 * Update the data contained in the collector
 */
 Collector::Update() {
    sensors_mutex.lock();
    flex_sensors.writeSensors(glove_data.flex_sensors);
    IMU.writeSensors(glove_data.imu);
    touch_sensors.writeKeys(glove_data.touch_sensor);
    sensors_mutex.unlock();
 }

/*
 * Calls the start() method on the periodic update task,
 * an internal timer is set up in the constructor
 */
 Collector::startUpdateTask(float period_s) {
    update_task_timer->start(period_s);
 }

/*
 * Calls the stop() method on the periodic update timer,
 */
 Collector::stopUpdateTask() {
    update_task_timer->stop();
 }
