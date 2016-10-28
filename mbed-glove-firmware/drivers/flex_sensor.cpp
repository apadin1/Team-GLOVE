/*
 * Filename:  flex_sensor.c
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
 *  Implementation for the FlexSensors class in flex_sensor.h
 */
#include "flex_sensor.h"

FlexSensors::FlexSensors() {
    pins[0] = new AnalogIn(FLEX_0);
    pins[1] = new AnalogIn(FLEX_1);
    pins[2] = new AnalogIn(FLEX_2);
    pins[3] = new AnalogIn(FLEX_3);

    update_task_timer = new RtosTimer(this, &FlexSensors::update, osTimerPeriodic);
}

void FlexSensors::update() {
    sensors_mutex.lock();
    for (uint8_t i = 0; i < FLEX_SENSORS_COUNT; i++) {
        values[i] = pins[i]->read_u16();
    }
    sensors_mutex.unlock();
}

void FlexSensors::startUpdateTask(float period_s) {
    update_task_timer->start(period_s);
}

void FlexSensors::stopUpdateTask() {
    update_task_timer->stop();
}

void FlexSensors::writeSensors(flex_sensor_t* buf) {
    sensors_mutex.lock();
    for (uint8_t i = 0; i < FLEX_SENSORS_COUNT; i++) {
        buf[i] = values[i];
    }
    sensors_mutex.unlock();
}

void FlexSensors::updateAndWriteSensors(flex_sensor_t* buf) {
    sensors_mutex.lock();
    for (uint8_t i = 0; i < FLEX_SENSORS_COUNT; i++) {
        values[i] = pins[i]->read_u16();
        buf[i] = values[i];
    }
    sensors_mutex.unlock();
}

void FlexSensors::print(Serial& pc) {
    sensors_mutex.lock();
    pc.printf("%hu 0x%hx, %hu 0x%hx, %hu 0x%hx, %hu 0x%hx\r\n",
            values[0], values[0],
            values[1], values[1],
            values[2], values[2],
            values[3], values[3]);
    sensors_mutex.unlock();
}

void FlexSensors::printSingle(Serial& pc, uint8_t index) {
    sensors_mutex.lock();
    if (index < FLEX_SENSORS_COUNT) {
        pc.printf("Flex %hu: %hu 0x%hx\r\n", index,
                values[index], values[index]);
    }
    sensors_mutex.unlock();
}
