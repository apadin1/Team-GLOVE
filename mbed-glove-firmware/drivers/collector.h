/*
 * Filename:  collector.h
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
 *  Interface for data transmission
 *
 */

#ifndef COLLECTOR_H_
#define COLLECTOR_H_

#include <inttypes.h>

#include "mbed.h"
#include "glove_sensors.h"

/*
 * Update Period (in seconds)
 *
 * 0.01 s = 10 ms = 100 Hz
 */
const float COLLECTOR_UPDATE_PERIOD = 0.01;

/* Collector
 *
 * Single class to handle data collection and transmission
 * between the gloves and the receiver. Methods are written
 * to update internally, and to write all methods into a
 * data structure
 */
class Collector {
public:
    /*
     * Constructor for collector
     */
    Collector(FlexSensors* flex, IMU_BNO055* imu, TouchSensor* touch,
              Serial* pc = NULL);

    /*
     * Collect data from sensors
     */
    void get_data();

    /*
     * Transmit data from the glove
     */
    void transmitData();

    /*
     * Update the data contained in the collector
     */
    void update();

    /*
     * Calls the start() method on the periodic update task,
     * an internal timer is set up in the constructor
     */
    void startUpdateTask(float period_s = COLLECTOR_UPDATE_PERIOD);

    /*
     * Calls the stop() method on the periodic update timer,
     */
    void stopUpdateTask();

private:
    FlexSensors* flex;
    IMU_BNO055* imu;
    TouchSensor* touch;

    glove_sensors_raw_t glove_data;
    Mutex glove_mutex;
    RtosTimer* update_task_timer;

    Serial* pc;  // for debugging
};
#endif /* COLLECTOR_H_ */
