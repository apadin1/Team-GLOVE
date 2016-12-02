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

static DigitalOut working(p14);
static DigitalOut err(p12);
static ble_error_t blerr;

AdvertBLE::AdvertBLE(uint32_t advertising_interval_ms) {

    adv_payload[0] = (ADVERT_ID >> 8) & 0x00FF;
    adv_payload[1] = ADVERT_ID & 0x00FF;
    working = 0;
    err = 0;

    ble.init();
    crcInit();

    adv.addData(GapAdvertisingData::MANUFACTURER_SPECIFIC_DATA, adv_payload, PAYLOAD_LENGTH);
    ble.setAdvertisingInterval(advertising_interval_ms);
}

void AdvertBLE::update(uint8_t* data, uint8_t len) {

    working = 1;
    /*
    if (memcmp(data, adv_payload+2, len) == 0) {
        return;
    }
    */
    //blerr = ble.stopAdvertising();

    // set data in the advertisement
    memcpy(adv_payload+2, data, len);

    // CRC
    crc_result = crcFast(data, len);
    adv_payload[PAYLOAD_LENGTH-2] = (crc_result >> 8) & 0x00FF;
    adv_payload[PAYLOAD_LENGTH-1] = crc_result & 0x00FF;

    // start the new advertisement
    adv.updateData(GapAdvertisingData::MANUFACTURER_SPECIFIC_DATA, adv_payload, PAYLOAD_LENGTH);
    blerr = ble.setAdvertisingData(adv);
    blerr = ble.startAdvertising();
    if (blerr) { for (uint8_t i = 0; i < blerr; ++i) { err=1; err=0; wait_ms(1); } err = 0; }
    working = 0;
}

void AdvertBLE::waitForEvent() {
    ble.waitForEvent();
}

