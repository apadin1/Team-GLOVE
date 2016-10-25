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

TouchSensor::TouchSensor(PinName sda, PinName scl, PinName intr)
    : qt(sda, scl), changeEvent(intr) {

        writeStaticConfig();

        // Assosciate the update function with the interrupt CHANGE line
        event.fall(this, &update); // TODO check syntax
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

void TouchSensor::update() {

    uint8_t buttons = qt.getButtonsState();

    // Check overflow flag
    if (buttons & 0x80) {
        // do something

        return;
    }

    // just get the keys we want
}
