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

#include "crc.h"

static const uint16_t ADVERT_ID = 0xBABE;
static const uint8_t PAYLOAD_DATA_LENGTH = 19;
static const uint8_t PAYLOAD_CRC_LENGTH = 2;
static const uint8_t PAYLOAD_LENGTH = 2 + PAYLOAD_DATA_LENGTH + PAYLOAD_CRC_LENGTH; // 23

/*
 * Structure of advertising packet data:
 *
 * 00 - ADVERT_ID MSB
 * 01 - ADVERT_ID LSB
 * 02-19 - glove_sensors_compressed_t
 * 21 - CRC MSB
 * 22 - CRC LSB
 */

class AdvertBLE {
public:

    AdvertBLE();

    /*
     * Given pointer to data of length PAYLOAD_DATA_LENGTH,
     * update the advertisement data and the CRC, if the data
     * differs from the payload being advertised
     */
    void update(uint8_t* data, uint8_t len=PAYLOAD_DATA_LENGTH);

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
