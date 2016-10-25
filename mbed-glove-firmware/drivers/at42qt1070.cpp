/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

//#include <unistd.h>
#include <math.h>
#include <string>
#include <stdexcept>

#include "at42qt1070.h"

using namespace std;

const uint8_t AVE_KEY_MAX = 6;
const uint8_t AKS_KEY_MAX = 3;
const uint8_t NO_GUARD_KEY = 7;

AT42QT1070::AT42QT1070(PinName sda, PinName scl, uint8_t address)
    : _i2c(sda, scl), _addr(address << 1) {

    initialize();
}

AT42QT1070::AT42QT1070(I2C& i2c, uint8_t address)
    : _i2c(i2c), _addr(address << 1) {

    initialize();
}

void AT42QT1070::initialize() {
    _i2c.frequency(AT42QT1070_I2C_MAX_FREQUENCY);

    // spec list <230ms as boot up time, wait here to be sure
    wait_ms(230);

    if (readChipID() != 0x2E) {
        return;  // throw std::runtime_error("Chip ID does not match the
                 // expected value (2Eh)");
    }

    _buttonStates = 0;
    _calibrating = false;
    _overflow = false;
}

//--------------------------------------------------------------------------------
AT42QT1070::~AT42QT1070() { _i2c.stop(); }

//--------------------------------------------------------------------------------
bool AT42QT1070::writeByte(uint8_t reg, uint8_t byte) {
    const char cmd[] = { reg, byte };

    if (0 == _i2c.write(_addr, cmd, 2))
        return true;
    else
        return false;
}

//--------------------------------------------------------------------------------
bool AT42QT1070::writeWord(uint8_t reg, uint16_t word) {
    const char cmd[] = { reg, word & 0xff, (word & 0xff00) >> 8 };

    if (0 == _i2c.write(_addr, cmd, 3))
        return true;
    else
        return false;
}

//--------------------------------------------------------------------------------
uint8_t AT42QT1070::readByte(uint8_t reg) {
    char data = 0;
    const char cmd = reg;
    _i2c.write(_addr, &cmd, 1);
    _i2c.read(_addr, &data, 1);

    return data;
}

//--------------------------------------------------------------------------------
uint16_t AT42QT1070::readWord(uint8_t reg) {
    uint16_t res = 0;
    char data[] = { 0, 0 };
    const char cmd = reg;

    _i2c.write(_addr, &cmd, 1);
    _i2c.read(_addr, data, 2);
    res = data[1] << 8 & data[0];

    return res;
}

//--------------------------------------------------------------------------------
uint8_t AT42QT1070::readChipID(void) { return readByte(REG_CHIPID); }

//--------------------------------------------------------------------------------
void AT42QT1070::updateState() {
    uint8_t status = readByte(REG_DETSTATUS);

    // if we are calibrating, don't change anything
    if (status & DET_CALIBRATE) {
        _calibrating = true;
        return;
    } else {
        _calibrating = false;
    }

    if (status & DET_OVERFLOW)
        _overflow = true;
    else
        _overflow = false;

    // if a touch is occurring, read the button states
    if (status & DET_TOUCH) {
        uint8_t keys = readByte(REG_KEYSTATUS);
        // high bit is reserved, filter it out
        _buttonStates = keys & ~0x80;
    } else {
        _buttonStates = 0;
    }
}

//--------------------------------------------------------------------------------
uint8_t AT42QT1070::getButtonsState() {
    uint8_t status = readByte(REG_DETSTATUS);

    // if a touch is occurring, read the button states
    if (status & DET_TOUCH) {
        uint8_t keys = readByte(REG_KEYSTATUS);
        // high bit is reserved, filter it out
        _buttonStates = keys & ~0x80;
    } else {
        _buttonStates = 0;
    }

    return _buttonStates;
}

//--------------------------------------------------------------------------------
bool AT42QT1070::isButtonPressed(const uint8_t button) {
    uint8_t buttonsState = 0;

    if (button <= 6) {
        buttonsState = getButtonsState();
    }

    return (buttonsState & (0x1 << button));
}

//--------------------------------------------------------------------------------
uint8_t AT42QT1070::getLowPowerMode(void) {
    return readByte(REG_LP);
}

//--------------------------------------------------------------------------------
uint8_t AT42QT1070::setLowPowerMode(uint8_t mode) {
    writeByte(REG_LP, mode);

    return getLowPowerMode();
}

//--------------------------------------------------------------------------------
uint8_t AT42QT1070::getAVE(uint8_t key) {
    uint8_t value = 0;
    uint8_t ave = 0;

    if (key <= AVE_KEY_MAX) {
        value = readByte(REG_AVE0 + key);
        ave = (value & 0xFC) >> 2;
    }

    return ave;
}

//--------------------------------------------------------------------------------
uint8_t AT42QT1070::setAVE(uint8_t key, uint8_t ave) {
    uint8_t value = 0;

    //    if (key > AVE_KEY_MAX) {
    //        throw std::invalid_argument("Only keys 0-6 are allowed");
    //    }

    //    switch (ave) {
    //        case 1:
    //        case 2:
    //        case 4:
    //        case 8:
    //        case 16:
    //        case 32:
    //            break;

    //        default:
    //            throw std::invalid_argument("Invalid averaging factor");
    //    }

    value = readByte(REG_AVE0 + key);
    value = value & 0x03;
    value = value | (ave << 2);
    writeByte(REG_AVE0 + key, value);

    return getAVE(key);
}

//--------------------------------------------------------------------------------
uint8_t AT42QT1070::getAKSGroup(uint8_t key) {
    uint8_t value = 0;
    uint8_t aks = 0;

    if (key <= AKS_KEY_MAX) {
        value = readByte(REG_AVE0 + key);
        aks = value & 0x03;
    }

    return aks;
}

//--------------------------------------------------------------------------------
uint8_t AT42QT1070::setAKSGroup(uint8_t key, uint8_t group) {
    uint8_t value = 0;

    if (key <= AVE_KEY_MAX && group <= AKS_KEY_MAX) {
        value = readByte(REG_AVE0 + key);
        value = value & 0xFC;
        value = value | group;
        writeByte(REG_AVE0 + key, value);
    }

    return getAKSGroup(key);
}

//--------------------------------------------------------------------------------
uint8_t AT42QT1070::getDetectionIntegrator(uint8_t key) {
    if (key <= AKS_KEY_MAX) {
        return readByte(REG_DI0 + key);
    }
}

//--------------------------------------------------------------------------------
uint8_t AT42QT1070::setDetectionIntegrator(uint8_t key, uint8_t di) {
    if (key <= AVE_KEY_MAX) {
        writeByte(REG_DI0 + key, di);
    }

    return getDetectionIntegrator(key);
}

//--------------------------------------------------------------------------------
uint8_t AT42QT1070::getThreshold(uint8_t key) {
    if (key <= AKS_KEY_MAX) {
        return readByte(REG_NTHR0 + key);
    }
}

//--------------------------------------------------------------------------------
uint8_t AT42QT1070::setThreshold(uint8_t key, uint8_t nthr) {
    if (key <= AVE_KEY_MAX) {
        writeByte(REG_NTHR0 + key, nthr);
    }

    return getThreshold(key);
}

//--------------------------------------------------------------------------------
uint8_t AT42QT1070::setGuard(uint8_t key=NO_GUARD_KEY) {
    // TODO add setters for FO/MaxCal instead of clearing here
    if (key <= AVE_KEY_MAX || key == NO_GUARD_KEY) {
        writeByte(REG_GUARD, (0x0f) & key);
    }

    return (0x0f) & readByte(REG_GUARD);
}

//--------------------------------------------------------------------------------
bool AT42QT1070::reset() {
    // write a non-zero value to the reset register
    return writeByte(REG_RESET, 0xff);
}

//--------------------------------------------------------------------------------
bool AT42QT1070::calibrate() {
    // write a non-zero value to the calibrate register
    return writeByte(REG_CALIBRATE, 0xff);
}
