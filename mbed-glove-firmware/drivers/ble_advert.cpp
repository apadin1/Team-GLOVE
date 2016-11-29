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
 */

#include "ble_advert.h"

AdvertBLE::AdvertBLE(uint32_t advertising_interval_ms) {
    memset(payload, 0, PAYLOAD_LENGTH);
    payload[0] = (ADVERT_ID >> 8) & 0x00FF;
    payload[1] = ADVERT_ID & 0x00FF;

    ble.init();
    adv.addData(GapAdvertisingData::MANUFACTURER_SPECIFIC_DATA,
            payload, PAYLOAD_LENGTH);

    ble.setAdvertisingInterval(advertising_interval_ms);
}

AdvertBLE::update(uint8_t* data) {

    // set data in the advertisement
    memcpy(payload+2, data, PAYLOAD_DATA_LENGTH);

    // CRC

    // start the new advertisement
    adv.updateData(GapAdvertisingData::MANUFACTURER_SPECIFIC_DATA,
            payload, PAYLOAD_LENGTH);
    ble.setAdvertisingData(adv);
    ble.startAdvertising();
}

AdvertBLE::waitForEventLoop() {
    for (;;) {
        ble.waitForEvent();
    }
}

