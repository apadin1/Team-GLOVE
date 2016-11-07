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
 * Update Period (in milliseconds)
 */
const uint32_t COLLECTOR_UPDATE_PERIOD = 10;

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
     * Transmit data from the glove
     */
    void transmitData();

    //NOTE: Proposed changes to collector class for
    //      translator class
    flex_sensor_t* readFlex(uint8_t flexNum);
    key_states_t* readTouch();
    bno_imu_t* readIMU();

    /*
     * Update the data contained in the collector
     */
    void update();

    /*
     * Calls the start() method on the periodic update task,
     * an internal timer is set up in the constructor
     */
    void startUpdateTask(uint32_t ms=COLLECTOR_UPDATE_PERIOD);

    /*
     * Calls the stop() method on the periodic update timer,
     */
    void stopUpdateTask();

private:
    FlexSensors* flex;
    IMU_BNO055* imu;
    TouchSensor* touch;

    glove_sensors_raw_t glove_data;
    RtosTimer* update_task_timer;

    DigitalOut working;
    Serial* pc;  // for debugging
};
#endif /* COLLECTOR_H_ */
