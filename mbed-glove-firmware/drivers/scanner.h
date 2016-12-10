/* scanner.h
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

#include "mbed.h"
#include "ble/BLE.h"

#include "glove_sensors.h"

#ifndef SCANNER_H_
#define SCANNER_H_

#define LEFT_GLOVE_ID  0xBEEF
#define RIGHT_GLOVE_ID 0xBABE
#define HID_CONFIG_ID  0xCAFE

#define MIN_PACKET_LENGTH 4


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
    else if (id == HID_CONFIG_ID) {
        hid_config();
    }
}

/************************* SCANNER CLASS ********************/
class Scanner {
public:

    // Scan period and duration are in milliseconds
    Scanner() {
        ble.init();
    }

    // Stop and start scanning
    void startScan(int scan_period, int scan_duration) {
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
};

#endif // SCANNER_H_
