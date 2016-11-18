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

TouchSensor::TouchSensor(PinName sda, PinName scl, PinName intr) : qt(sda, scl) {
    initialize(intr);
}

TouchSensor::TouchSensor(I2C& i2c, PinName intr)
    : qt(i2c) {
    initialize(intr);
}


void TouchSensor::initialize(PinName intr) {
    needs_restart = false;
    writeStaticConfig();
    qt.getButtonsState();

    // Associate the update function with the interrupt CHANGE line
    if (intr != NC) {
        if (TOUCH_DIGITALIN_CHANGE) {
            change_line = new DigitalIn(intr);
        }
        else {
            change_event = new InterruptIn(intr);
            change_event->fall(this, &TouchSensor::changeEventHandler);
        }
    }
}

void TouchSensor::writeStaticConfig() {
    qt.setGuard(TOUCH_GUARD_KEY);
    qt.setLowPowerMode(TOUCH_LP_MODE);
    qt.setMaxOn(TOUCH_MAX_ON);

    for (uint8_t k = 0; k < 7; ++k) {
        qt.setThreshold(k, TOUCH_NTHRESHOLD);
        qt.setDetectionIntegrator(k, TOUCH_DI);
        qt.setAVE(k, TOUCH_AVE);
        qt.setAKSGroup(k, TOUCH_AKS[k]);
    }
}

void TouchSensor::writeKeys(key_states_t* key_states) {
    key_states->a = keys.a;
    key_states->b = keys.b;
    key_states->c = keys.c;
    key_states->d = keys.d;
}

extern DigitalOut l4;
uint16_t faaiil = 0;
volatile uint8_t count;
void TouchSensor::update() {

    if (faaiil++ > 4) {
        faaiil = 0;
        for (;;) {count += 1;}
    }

    /* Use the change line to avoid unnessescary
     * I2C I/O, but without being an interrupt
     */
    if (*change_line == 1) {
        return;
    }
    uint8_t buttons = qt.getButtonsState();

    // Check overflow flag
    if (buttons & 0x80) {
        // do something about it
        return;
    }

    // just get the keys we want
    keys.a = (buttons & 0x01); // key 0
    keys.b = (buttons & 0x02) >> 1; // key 1
    keys.c = (buttons & 0x04) >> 2; // key 2
    keys.d = (buttons & 0x08) >> 3; // key 3
    //keys.e = buttons & 0x10; // key 4
    //keys.f = buttons & 0x20; // key 5
    //keys.g = buttons & 0x40; // key 6
}

void TouchSensor::updateAndWrite(key_states_t* key_states) {
    update();
    writeKeys(key_states);
}

void TouchSensor::reset() {
    qt.reset();
    writeStaticConfig();
    qt.getButtonsState();
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

void TouchSensor::singleUpdate() {
    if (needs_restart) {
        //reset();
    }

    l4 = 0;
    needs_restart = true;
    update();
    l4 = 1;
    needs_restart = false;

    update_thread->terminate();
}

void TouchSensor::spawnUpdateThread() {
    update_thread = new Thread;
    update_thread->start(this, &TouchSensor::singleUpdate);
    Thread::yield();
}

void TouchSensor::terminateUpdateThreadIfBlocking() {
    if (needs_restart) {
        update_thread->terminate();
        // Leave reset in the update funtion cuz don't block here
        // Might skip full reset??? Maybe count then do it after X hangs
    }
    delete update_thread;
    update_thread = NULL;
}

void TouchSensor::print(Serial& pc, key_states_t& keys_) {
    pc.printf("Keys: %hu %hu %hu %hu\r\n",
            keys_.a, keys_.b, keys_.c, keys_.d);
}

