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

const PinName FLEX_DEBUG_PIN = p15;

FlexSensors::FlexSensors()
    : working(FLEX_DEBUG_PIN) {

    pins[0] = new AnalogIn(FLEX_0);
    pins[1] = new AnalogIn(FLEX_1);
    pins[2] = new AnalogIn(FLEX_2);
    pins[3] = new AnalogIn(FLEX_3);

    update_task_timer = new RtosTimer(this, &FlexSensors::update, osTimerPeriodic);
}

void FlexSensors::update() {
    mutex.lock();
    working = 1;
    for (uint8_t i = 0; i < FLEX_SENSORS_COUNT; i++) {
        values[i] = pins[i]->read_u16();
    }
    working = 0;
    mutex.unlock();
}

void FlexSensors::startUpdateTask(uint32_t ms) {
    update_task_timer->start(ms);
}

void FlexSensors::stopUpdateTask() {
    update_task_timer->stop();
}

void FlexSensors::writeSensors(flex_sensor_t* buf) {
    mutex.lock();
    for (uint8_t i = 0; i < FLEX_SENSORS_COUNT; i++) {
        buf[i] = values[i];
    }
    mutex.unlock();
}

void FlexSensors::updateAndWriteSensors(flex_sensor_t* buf) {
    mutex.lock();
    for (uint8_t i = 0; i < FLEX_SENSORS_COUNT; i++) {
        values[i] = pins[i]->read_u16();
        buf[i] = values[i];
    }
    mutex.unlock();
}

void FlexSensors::print(Serial& pc) {
    mutex.lock();
    pc.printf("Flex: %hu, %hu, %hu, %hu\r\n",
            values[0], values[1], values[2], values[3]);
    mutex.unlock();
}

void FlexSensors::printSingle(Serial& pc, uint8_t index) {
    if (index < FLEX_SENSORS_COUNT) {
        pc.printf("Flex %hu: %hu\r\n", index, values[index]);
    }
}
