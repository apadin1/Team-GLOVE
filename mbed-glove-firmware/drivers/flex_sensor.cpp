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

#include "flex_sensor.h"

const PinName FLEX_DEBUG_PIN = p15;

FlexSensors::FlexSensors() {
    pins[0] = new AnalogIn(FLEX_0);
    pins[1] = new AnalogIn(FLEX_1);
    pins[2] = new AnalogIn(FLEX_2);
    pins[3] = new AnalogIn(FLEX_3);
}

void FlexSensors::update() {
    for (uint8_t i = 0; i < FLEX_SENSORS_COUNT; i++) {
        values[i] = pins[i]->read_u16();
    }
}

void FlexSensors::writeSensors(flex_sensor_t* buf) {
    for (uint8_t i = 0; i < FLEX_SENSORS_COUNT; i++) {
        buf[i] = values[i];
    }
}

void FlexSensors::updateAndWrite(flex_sensor_t* buf) {
    for (uint8_t i = 0; i < FLEX_SENSORS_COUNT; i++) {
        values[i] = pins[i]->read_u16();
        buf[i] = values[i];
    }
}

void FlexSensors::print(Serial& pc) {
    pc.printf("Flex: %hu, %hu, %hu, %hu\r\n",
            values[0], values[1], values[2], values[3]);
}

void FlexSensors::printSingle(Serial& pc, uint8_t index) {
    if (index < FLEX_SENSORS_COUNT) {
        pc.printf("Flex %hu: %hu\r\n", index, values[index]);
    }
}
