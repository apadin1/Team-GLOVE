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

const PinName COLLECTOR_DEBUG_PIN = p26;

Collector::Collector(FlexSensors* _flex, IMU_BNO055* _imu,
                       TouchSensor* _touch, AdvertBLE& _adble)
    : flex(_flex), imu(_imu), touch(_touch), adble(_adble),
      working(COLLECTOR_DEBUG_PIN) {

    update_task_timer =
          new RtosTimer(this, &Collector::updateAndAdvertise, osTimerPeriodic);

    flex_ptr = &glove_data.flex_sensors[0];
}

void Collector::updateAndAdvertise() {
    working = 1;

    touch->spawnUpdateThread();

    imu->updateAndWrite(&glove_data.imu); // TODO remove linear data collection
    flex->updateAndWrite(flex_ptr);
    touch->writeKeys(&glove_data.touch_sensor);

    // send the advertisement
    adble.update((uint8_t*)&glove_data);

    // because
    Thread::wait_ms(5);

    // this really needs to be later in the looop....
    touch->terminateUpdateThreadIfBlocking();

    working = 0;
    return;
}

void Collector::startUpdateTask(uint32_t ms) {
    period_ms = ms;
    update_task_timer->start(period_ms);
}

void Collector::stopUpdateTask() {
    update_task_timer->stop();
}
