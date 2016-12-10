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
 *  Implementation of collector.h
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

#include "collector.h"
#include "string.h"

Collector::Collector(FlexSensors& _flex, IMU_BNO055& _imu,
                     TouchSensor& _touch, AdvertBLE& _adble)
    : flex(_flex), imu(_imu), touch(_touch), adble(_adble) {

    flex_data = glove_data.flex_sensors; // ptr to the first flex_sensor_t
    touch_data = &(glove_data.touch_sensor); // ptr to the key_states_t struct
    imu_data = &(glove_data.imu); // ptr to the bno_imu_t struct in glove data

    update_task_timer =
          new RtosTimer(this, &Collector::updateAndAdvertise, osTimerPeriodic);
}

void Collector::updateAndAdvertise() {

    imu.updateAndWrite(imu_data);
    flex.updateAndWrite(flex_data);

    touch.spawnUpdateThread();
    Thread::wait(8);
    touch.writeKeys(touch_data);

    compressGloveSensors(&glove_data, &glove_data_compressed);
    adble.update((uint8_t*)&glove_data_compressed, glove_sensors_compressed_size);

    touch.terminateUpdateThreadIfBlocking();

    adble.waitForEvent();
}

void Collector::startUpdateTask(uint32_t ms) {
    update_task_timer->start(ms);
}

void Collector::stopUpdateTask() {
    update_task_timer->stop();
}
