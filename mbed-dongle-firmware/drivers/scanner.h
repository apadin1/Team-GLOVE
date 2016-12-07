/* scanner.h */

#ifndef SCANNER_H_
#define SCANNER_H_

#include "mbed.h"
#include "ble/BLE.h"

#include "glove_sensors.h"
#include "translator.h"
#include "gpio.h"

#define LEFT_GLOVE_ID  0xBEEF
#define RIGHT_GLOVE_ID 0xBABE

#define MIN_PACKET_LENGTH 4

// TODO: Testing
int left_count;
int right_count;

extern glove_sensors_compressed_t rightGloveCompressed;

/************************* FUNCTIONS ********************/

static void left_glove() {
    ++left_count;
}

static void right_glove() {
    ++right_count;
}


// Does most of the work of filtering packets
static void advertisementCallback(const Gap::AdvertisementCallbackParams_t *params) {

    // Filter advertisements by length
    if (params->advertisingDataLen < MIN_PACKET_LENGTH) {
        return;
    }

    // Filter advertisements by ID number
    uint16_t id = params->advertisingData[2] << 8;
    id |= params->advertisingData[3];

    if (id == LEFT_GLOVE_ID) {
        //memcpy(&compressed_data, (params->advertisingData + 4), sizeof(compressed_data));
        // this is the thing TODO
        //memcpy(&glove_sensors_compressed, params->advertisingData+4, sizeof(glove_sensors_compressed_t));
    }
    else if (id == RIGHT_GLOVE_ID) {
        memcpy(&rightGloveCompressed, (params->advertisingData + 4), sizeof(rightGloveCompressed));
        right_glove();
    }
}

/************************* SCANNER CLASS ********************/
class Scanner {
public:

    // Scan period and duration are in milliseconds
    Scanner(BLE &_ble, Translator* _translator) :
        translator(_translator),
        ble(_ble) {
    }

    // Stop and start scanning
    void startScan(int scan_period=10, int scan_duration=8) {
        ble.gap().setScanParams(scan_period, scan_duration);
        ble.gap().startScan(advertisementCallback);
    }

    void stopScan() {
        ble.gap().stopScan();
    }

private:
    BLE &ble;
    Translator* translator;
    // need both glove sensors raw TODO
};

#endif // SCANNER_H_
