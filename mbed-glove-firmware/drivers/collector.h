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
 *
 * Copyright (c) 2016 by Nick Bertoldi, Ben Heckathorn, Ryan O'Keefe,
 *                       Adrian Padin, Timothy Schumacher
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
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
     */
    Collector(FlexSensors& _flex, IMU_BNO055& _imu, TouchSensor& _touch, AdvertBLE& _adble);

    /*
     * Update gesture mapping via new configuration arrary.
     */
    void updateGestureMap(uint8_t* config);

    /*
     * Analyze sensors to determine if gesture
     * is occuring. If so, generate proper HID data to be sent to HID class.
     * This function designed to be set up as a periodic task.
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

    const glove_sensors_raw_t& getGloveSensorData() {
        return glove_data;
    }

private:
    // Sensor classes (consider &refs)
    FlexSensors& flex;
    IMU_BNO055& imu;
    TouchSensor& touch;
    AdvertBLE& adble;

    // glove sensor data struct(s)
    glove_sensors_raw_t glove_data;
    glove_sensors_compressed_t glove_data_compressed;

    // alias pointers into the glove_data struct
    // to be passed into the write() methods of sensors
    flex_sensor_t* flex_data;
    key_states_t* touch_data;
    bno_imu_t* imu_data;

    RtosTimer* update_task_timer;
};

#endif /* TRANSLATOR_H_ */
