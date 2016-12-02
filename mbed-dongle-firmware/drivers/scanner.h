/* scanner.h */

#ifndef SCANNER_H_
#define SCANNER_H_

#include "mbed.h"
#include "ble/BLE.h"

#include "glove_sensors.h"
#include "translator.h"

#define LEFT_GLOVE_ID  0xBEEF
#define RIGHT_GLOVE_ID 0xBABE
#define HID_CONFIG_ID  0xCAFE

#define MIN_PACKET_LENGTH 4

const PinName SCANNER_DEBUG_PIN = p26;

/************************* FUNCTIONS ********************/

static void hid_config() {
    static int count = 0;
    count += 1;
    printf("config: %d\r\n", count);
}

static void left_glove() {
    static int count = 0;
    count += 1;
    printf("left: %d\r\n", count);
}

static void right_glove() {
    static int count = 0;
    count += 1;
    printf("right: %d\r\n", count);
}


// Does most of the work of filtering packets
static void advertisementCallback(const Gap::AdvertisementCallbackParams_t *params) {

    // Filter advertisements by length
    if (params->advertisingDataLen < MIN_PACKET_LENGTH) {
        return;
    }

    // Filter advertisements by ID number
    uint16_t id = params->advertisingData[3] << 8;
    id |= params->advertisingData[2];

    if (id == LEFT_GLOVE_ID) {
        left_glove();
    }
    else if (id == RIGHT_GLOVE_ID) {
        right_glove();
    }
}

/************************* SCANNER CLASS ********************/
class Scanner {
public:

    // Scan period and duration are in milliseconds
    Scanner(Translator* _translator) {
        ble.init();
        translator = _translator;
    }

    // Stop and start scanning
    void startScan(int scan_period=15, int scan_duration=10) {
        ble.gap().setScanParams(scan_period, scan_duration);
        ble.gap().startScan(advertisementCallback);
    }

    void stopScan() {
        ble.gap().stopScan();
    }

    // Convenience function - either one can be used
    void waitForEvent() {
        ble.waitForEvent();
    }

private:
    BLE ble;
    Translator* translator;

    //DigitalOut working;
};

#endif // SCANNER_H_
