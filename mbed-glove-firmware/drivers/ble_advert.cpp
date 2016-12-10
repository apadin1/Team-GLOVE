/*
 * Filename:  ble_advert.cpp
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

#include "ble_advert.h"

AdvertBLE::AdvertBLE() {

    memset(adv_payload, 0, PAYLOAD_LENGTH);
    adv_payload[0] = (ADVERT_ID >> 8) & 0x00FF;
    adv_payload[1] = ADVERT_ID & 0x00FF;

    ble.init();

    adv.addData(GapAdvertisingData::MANUFACTURER_SPECIFIC_DATA, adv_payload, PAYLOAD_LENGTH);
    ble.setAdvertisingData(adv);

    ble.setAdvertisingTimeout(0);
    ble.setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    ble.setAdvertisingInterval(20); // 20ms is the minimal adverting period for BLE

    ble.startAdvertising();
}

void AdvertBLE::update(uint8_t* data, uint8_t len) {
    // set data in the advertisement, after the ADVERT_ID bytes
    memcpy(adv_payload+2, data, len);

    // start the new advertisement
    adv.updateData(GapAdvertisingData::MANUFACTURER_SPECIFIC_DATA, adv_payload, PAYLOAD_LENGTH);
    ble.setAdvertisingData(adv);
}

void AdvertBLE::waitForEvent() {
    ble.waitForEvent();
}

