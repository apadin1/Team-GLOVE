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
 *  Collect sensor data on the glove, send over ble advertisement
 */

#ifndef COLLECTOR_H_
#define COLLECTOR_H_

#include "mbed.h"

#include "glove_sensors.h"
#include "ble_advert.h"

/*
 * Default Update Period (in milliseconds)
 */
const uint32_t COLLECTOR_UPDATE_PERIOD = 20;

/* Collector
 *
 * Single class to handle data interpretation between
 * glove data and HID input. Methods are written
 * to update internally, and to write all methods into a
 * data structure
 */
class Collector {
public:
    /*
     * Constructor for collector
     *
     * TODO try to use refrences instead of pointers
     */
    Collector(FlexSensors* _flex, IMU_BNO055* _imu, TouchSensor* _touch, AdvertBLE& _adble);

    /*
     * Update gesture mapping via new configuration vector.
     * Transciever to send the new Vector to bluetooth class,
     * which should then call this function
     */
    void updateAndAdvertise();

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
    // Sensor classes (consider &refs)
    FlexSensors* flex;
    IMU_BNO055* imu;
    TouchSensor* touch;
    AdvertBLE& adble;

    glove_sensors_raw_t glove_data;
    glove_sensors_compressed_t glove_data_compressed;

    RtosTimer* update_task_timer;
    DigitalOut working;
};

#endif /* TRANSLATOR_H_ */
