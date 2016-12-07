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

// Size of data packet that is expected
const int PACKET_LENGTH =
    (2 + /* BLE advertisement overhead */
     2 + /* Vendor/Manufacturer ID */
     glove_sensors_compressed_size /* Data + checksum */
     );


// TODO: Testing
int left_count;
int right_count;

/************************* SCANNER CLASS ********************/
class Scanner {
public:

    // Scan period and duration are in milliseconds
    Scanner(BLE &_ble,
            glove_sensors_compressed_t* _left_compressed,
            glove_sensors_compressed_t* _right_compressed) :

        ble(_ble),
        left_compressed(_left_compressed),
        right_compressed(_right_compressed) {
    }

    // Stop and start scanning
    void startScan(int scan_period=100, int scan_duration=30) {
        ble.gap().setScanParams(scan_period, scan_duration);
        ble.gap().startScan(this, &Scanner::advertisementCallback);
    }

    void stopScan() {
        ble.gap().stopScan();
    }

    // Callback for when an advertisement is received
    // Filters the packets and copies glove data
    void advertisementCallback(
            const Gap::AdvertisementCallbackParams_t *params) {

        // Filter advertisements by length
        if (params->advertisingDataLen < (PACKET_LENGTH-4)) {
            return;
        }

        // Filter advertisements by ID number
        uint16_t id = params->advertisingData[2] << 8;
        id |= params->advertisingData[3];

        // Packet is a Left Glove
        if (id == LEFT_GLOVE_ID) {

            // Copy glove data
            memcpy(left_compressed,
                   (params->advertisingData + 4),
                   sizeof(glove_sensors_compressed_t));
            ++left_count;
        }

        // Packet is a Right Glove
        else if (id == RIGHT_GLOVE_ID) {

            // Copy glove data
            memcpy(right_compressed,
                   (params->advertisingData + 4),
                   sizeof(glove_sensors_compressed_t));
            ++right_count;
        }
    }

private:
    BLE &ble;
    //Translator* translator;

    // Pointers to both compressed structures
    glove_sensors_compressed_t* left_compressed;
    glove_sensors_compressed_t* right_compressed;

};

#endif // SCANNER_H_
