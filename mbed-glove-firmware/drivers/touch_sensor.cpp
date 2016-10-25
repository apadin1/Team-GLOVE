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

TouchSensor::TouchSensor()
    : qt(TOUCH_I2C_SDA, TOUCH_I2C_SCL, TOUCH_CHANGE) {}

bool TouchSensor::writeStaticConfig();
// reset?
// calibrate???
// setGuard (none)
// setLP
// setAVE
// setAKS per key
// setDI
// setThresh
