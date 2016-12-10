/*
 * Filename:  ble_advert.h
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
 *   Encapsulate and simplify the sending of glove sensor
 *  data via BLE advertisements
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

#ifndef BLE_ADVERT_H_
#define BLE_ADVERT_H_

#include <cstring> // string.h
#include <inttypes.h>

#include "mbed.h"
#include "ble/BLE.h"

#include "glove_sensors.h"

#ifdef LEFT_GLOVE
static const uint16_t ADVERT_ID = 0xBEEF;
#elif defined RIGHT_GLOVE
static const uint16_t ADVERT_ID = 0xBABE;
#else
#error "Must define either LEFT_GLOVE or RIGHT_GLOVE"
#endif

static const uint8_t PAYLOAD_LENGTH = 2 + glove_sensors_compressed_size;

/*
 * Structure of advertising packet data:
 *
 * 00 - ADVERT_ID MSB
 * 01 - ADVERT_ID LSB
 * 02-20 - glove_sensors_compressed_t
 * 21 - CRC MSB
 * 22 - CRC LSB
 */

class AdvertBLE {
public:

    AdvertBLE();

    /*
     * Given pointer to data of length PAYLOAD_LENGTH,
     * update the advertisement data and the CRC, if the data
     * differs from the payload being advertised
     */
    void update(uint8_t* data, uint8_t len=PAYLOAD_LENGTH);

    /*
     * Calls ble.waitForEvent()
     */
    void waitForEvent();

private:
    BLE ble;
    GapAdvertisingData adv;
    uint8_t adv_payload[PAYLOAD_LENGTH];
    uint16_t crc_result;
};

#endif /* BLE_ADVERT_H_ */
