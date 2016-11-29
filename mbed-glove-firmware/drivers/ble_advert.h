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
 */

#ifndef BLE_ADVERT_H_
#define BLE_ADVERT_H_

#include <cstring> // string.h
#include <inttypes.h>

#include "mbed.h"
#include "ble/BLE.h"

static const uint16_t ADVERT_ID = 0xFACE; // 0xFACE
static const uint8_t PAYLOAD_DATA_LENGTH = 19;
static const uint8_t PAYLOAD_CRC_LENGTH = 2;
static const uint8_t PAYLOAD_LENGTH = 2 + PAYLOAD_DATA_LENGTH + PAYLOAD_CRC_LENGTH;

class AdvertBLE {
public:

    AdvertBLE(uint32_t advertising_interval_ms=15);

    /*
     * Given pointer to data of length PAYLOAD_DATA_LENGTH,
     * update the advertisement data and the CRC
     */
    void update(uint8_t* data);

    /*
     * Calls ble.waitForEvent() in a for(;;) loop that never returns
     */
    void waitForEventLoop();

private:
    BLE& ble;
    GapAdvertisingData adv;
    uint8_t adv_payload[PAYLOAD_LENGTH];
}

#endif /* BLE_ADVERT_H_ */
