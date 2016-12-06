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

    flex_data = glove_data.flex_sensors; // ptr to the first flex_sensor_t
    touch_data = &(glove_data.touch_sensor); // ptr to the key_states_t struct
    imu_data = &(glove_data.imu); // ptr to the bno_imu_t struct in glove data

    update_task_timer =
          new RtosTimer(this, &Collector::updateAndAdvertise, osTimerPeriodic);
}

void Collector::updateAndAdvertise() {
    static int count = 0;
    working = 1; l = 0;

    touch->spawnUpdateThread();

    /*
    imu->updateAndWrite(&glove_data.imu);
    flex->updateAndWrite(&glove_data.flex_sensors[0]);
    touch->writeKeys(&glove_data.touch_sensor);
    */
    imu->updateAndWrite(imu_data);
    flex->updateAndWrite(flex_data);
    touch->writeKeys(touch_data);

    if (count++ > 100) {
        printf("x f: %d, p: %f\r\n", flex_data[0], imu_data->orient_pitch);
        compressGloveSensors(&glove_data, &glove_data_compressed);
        printf("c f: %d, p: %f\r\n", glove_data_compressed.f[0], glove_data_compressed.pitch);
        count = 0;
    }

    adble.update((uint8_t*)&glove_data_compressed);

    // because it works...
    wait_ms(5);

    touch->terminateUpdateThreadIfBlocking();

    working = 0; l = 1;
    adble.waitForEvent();
}

void Collector::startUpdateTask(uint32_t ms) {
    update_task_timer->start(ms);
}

void Collector::stopUpdateTask() {
    update_task_timer->stop();
}
