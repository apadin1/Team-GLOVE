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
 */

#include "collector.h"
#include "string.h"

const PinName COLLECTOR_DEBUG_PIN = p15;
static DigitalOut l(LED4);

Collector::Collector(FlexSensors* _flex, IMU_BNO055* _imu,
                     TouchSensor* _touch, AdvertBLE& _adble)
    : flex(_flex), imu(_imu), touch(_touch), adble(_adble),
      working(COLLECTOR_DEBUG_PIN) {

    update_task_timer =
          new RtosTimer(this, &Collector::updateAndAdvertise, osTimerPeriodic);
}

void Collector::updateAndAdvertise() {
    working = 1; l = !l;

    touch->spawnUpdateThread();

    imu->updateAndWrite(&glove_data.imu);
    flex->updateAndWrite(&glove_data.flex_sensors[0]);
    touch->writeKeys(&glove_data.touch_sensor);

    compressGloveSensors(&glove_data, &glove_data_compressed);

    adble.update((uint8_t*)&glove_data_compressed);

    // because it works...
    wait_ms(5);

    touch->terminateUpdateThreadIfBlocking();

    working = 0;
    adble.waitForEvent();
}

void Collector::startUpdateTask(uint32_t ms) {
    update_task_timer->start(ms);
}

void Collector::stopUpdateTask() {
    update_task_timer->stop();
}
