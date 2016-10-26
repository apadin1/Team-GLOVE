/*
 * Filename:  touch_sensor.cpp
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
 *   Top-level interface for working with capacitive sensor chip,
 *   which uses I2C and can communicate the values of up to 7 sensors
 *
 *   The AT42QT1070 in I2C comms mode can use a single interrupt line
 *   to indicate a change in state of the sensors.
 */

#include "touch_sensor.h"

TouchSensor::TouchSensor(PinName sda, PinName scl, PinName intr)
    : qt(sda, scl), change_event(intr) {

        writeStaticConfig();

        // Assosciate the update function with the interrupt CHANGE line
        event.fall(this, &changeEventHandler); // TODO check syntax
    }

bool TouchSensor::writeStaticConfig() {
    // reset?
    // calibrate???

    qt.setGuard();
    qt.setLowPowerMode(TOUCH_LP_MODE);
    qt.setMaxOn(TOUCH_MAX_ON);

    for (uint8_t k = 0; k < 7; ++k) {
        qt.setThreshold(k, TOUCH_NTHRESHOLD);
        qt.setDetectionIntegrator(k, TOUCH_DI);
        qt.setAVE(k, TOUCH_AVE);
        qt.setAKSGroup(k, TOUCH_AKS[k]);
    }
}

void TouchSensor::writeKeys(key_states_t& key_states) {
    keys_mutex.lock();
    key_states = keys;
    keys_mutex.unlock();
}

void TouchSensor::update() {

    uint8_t buttons = qt.getButtonsState();

    // Check overflow flag
    if (buttons & 0x80) {
        // do something about it
        return;
    }

    // just get the keys we want
    keys_mutex.lock();
    keys.a = buttons & 0x01; // key 0
    keys.b = buttons & 0x02; // key 1
    keys.c = buttons & 0x04; // key 2
    keys.d = buttons & 0x08; // key 3
    //keys.x = buttons & 0x10; // key 4
    //keys.x = buttons & 0x20; // key 5
    //keys.x = buttons & 0x40; // key 6
    keys_mutex.unlock();
}

void TouchSensor::changeEventHandler() {
    do_update.release();
}

void TouchSensor::updateTask() {
    for (;;) {
        do_update.wait(osWaitForever);
        update();
    }
}

