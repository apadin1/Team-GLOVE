#include <inttypes.h>
#include <vector>

#include "mbed.h"
#include "ble/BLE.h"

//#include "drivers/scanner.h"
#include "drivers/translator.h"
#include "drivers/glove_sensors.h"

#define LEFT_GLOVE_ID  0xBEEF
#define RIGHT_GLOVE_ID 0xBABE

#define MIN_PACKET_LENGTH 4

#include <string.h>
static glove_sensors_raw_t glove_sensors;
static glove_sensors_compressed_t glove_sensors_compressed;

static DigitalOut l1(LED1, 1);
static DigitalOut l2(LED2, 1);
static DigitalOut l3(LED3, 1);
static DigitalOut l4(LED4, 1);

static int packet_count = 0;
//static BLE* ble_p;
static BLE& ble = BLE::Instance(BLE::DEFAULT_INSTANCE);

void summarizeAdvertisement(const Gap::AdvertisementCallbackParams_t *params) {
    packet_count += 1;
    l2=0;l2=1;

    // Filter advertisements by length
    if (params->advertisingDataLen < MIN_PACKET_LENGTH) {
        return;
    }

    // Filter advertisements by ID number
    uint16_t id = params->advertisingData[2] << 8;
    id |= params->advertisingData[3];

    if (id == LEFT_GLOVE_ID || id == RIGHT_GLOVE_ID) {
        memcpy(&glove_sensors_compressed, params->advertisingData+4, sizeof(glove_sensors_compressed_t));
    }
}

void waitForEventAndNothingElse() {
    for (;;) {
        ble.waitForEvent();
        l4 = 0;
        l4 = 1;
    }
}

void calibration_summary() {
    // Turn off LEDs
    l1 = 1; l2 = 1; l3 = 1; l4 = 1;

    memset(&glove_sensors_compressed, 0, sizeof(glove_sensors_compressed_t));

    // Initialize ble
    ble.init();

    ble.gap().setScanParams(100, 100);
    ble.gap().startScan(summarizeAdvertisement);

    Thread waitForFuckingEvent(waitForEventAndNothingElse);

    for (;;) {
        //ble.waitForEvent();
        Thread::wait(2000);
        l1 = 0;
        printf("count: %d | ", packet_count);
        printf("f0: %d, f1: %d, f2: %d | t: 0x%x\r\n",
                glove_sensors_compressed.f0, glove_sensors_compressed.f1,
                glove_sensors_compressed.f2, glove_sensors_compressed.f3,
                glove_sensors_compressed.t);
        l1 = 1;
    }
}
