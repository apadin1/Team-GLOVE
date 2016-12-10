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

