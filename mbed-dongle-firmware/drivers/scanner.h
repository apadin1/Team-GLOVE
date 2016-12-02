/* scanner.h */

#ifndef SCANNER_H_
#define SCANNER_H_

#include "mbed.h"
#include "ble/BLE.h"

#include "glove_sensors.h"
#include "translator.h"

#define LEFT_GLOVE_ID  0xBEEF
#define RIGHT_GLOVE_ID 0xBABE

#define MIN_PACKET_LENGTH 4

static DigitalOut led111(LED1);

// TODO: Testing
extern int left_count;
extern int right_count;

const PinName SCANNER_DEBUG_PIN = p26;

/*
 * Default Update Period (in milliseconds)
 */
const uint32_t SCANNER_UPDATE_PERIOD = 20;

/************************* FUNCTIONS ********************/


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

    led111 = !led111;
    // Filter advertisements by length
    if (params->advertisingDataLen < MIN_PACKET_LENGTH) {
        return;
    }

    // Filter advertisements by ID number
    uint16_t id = params->advertisingData[2] << 8;
    id |= params->advertisingData[3];

    if (id == LEFT_GLOVE_ID) {
        //left_glove();
        ++left_count;
    }
    else if (id == RIGHT_GLOVE_ID) {
        //right_glove();
        ++right_count;
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
    void startScan(int scan_period=97, int scan_duration=93) {
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

    /*
     * Calls the start() method on the periodic update task,
     * an internal timer is set up in the constructor
     */
     void startUpdateTask(uint32_t ms=SCANNER_UPDATE_PERIOD) {
        update_task_timer->start(ms);
     }

    /*
     * Calls the stop() method on the periodic update timer,
     */
     void stopUpdateTask() {
        update_task_timer->stop();
     }

private:
    BLE ble;
    Translator* translator;

    /* MBED Objects */
    RtosTimer* update_task_timer;
    //DigitalOut working;
};

#endif // SCANNER_H_
