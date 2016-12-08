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

/************************* SCANNER CLASS ********************/
class Scanner {
public:

    // Scan period and duration are in milliseconds
    Scanner(BLE &_ble,
            glove_sensors_compressed_t* left_data,
            glove_sensors_compressed_t* right_data) :
            //glove_sensors_raw_t& right_data) :

        ble(_ble),
        left_compressed(left_data),
        right_compressed(right_data)
        {}

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
        glove_sensors_compressed_t* compressed_data =
            (glove_sensors_compressed_t*)(params->advertisingData + 4);

        // Packet is a Left Glove
        if (id == LEFT_GLOVE_ID) {
            memcpy(left_compressed, compressed_data, glove_sensors_compressed_size);
            //extractGloveSensors(left_glove_data, compressed_data);
        }

        // Packet is a Right Glove
        else if (id == RIGHT_GLOVE_ID) {
            memcpy(right_compressed, compressed_data, glove_sensors_compressed_size);
            //extractGloveSensors(right_glove_data, compressed_data);
        }
    }

private:
    BLE &ble;

    glove_sensors_compressed_t* left_compressed;
    glove_sensors_compressed_t* right_compressed;

    // Pointers to both compressed structures
    //glove_sensors_raw_t& left_glove_data;
    //love_sensors_raw_t& right_glove_data;
};

#endif // SCANNER_H_
