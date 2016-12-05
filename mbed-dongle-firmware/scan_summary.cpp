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

static glove_sensors_raw_t glove_sensors;
static glove_sensors_compressed_t glove_sensors_compressed;

static DigitalOut l1(LED1, 1);
static DigitalOut l2(LED2, 1);
static DigitalOut l3(LED3, 1);
static DigitalOut l4(LED4, 1);

static int packet_count = 0;

static void summarizeAdvertisement(const Gap::AdvertisementCallbackParams_t *params) {

    // Filter advertisements by length
    if (params->advertisingDataLen < MIN_PACKET_LENGTH) {
        return;
    }

    // Filter advertisements by ID number
    uint16_t id = params->advertisingData[2] << 8;
    id |= params->advertisingData[3];

    if (id == LEFT_GLOVE_ID || id == RIGHT_GLOVE_ID) {
    }
    packet_count += 1;
}

void calibration_summary() {
    // Turn off LEDs
    l1 = 1; l2 = 1; l3 = 1; l4 = 1;

    // Initialize ble
    BLE& ble = BLE::Instance(BLE::DEFAULT_INSTANCE);
    ble.init();

    ble.gap().setScanParams(10, 10);
    ble.gap().startScan(summarizeAdvertisement);

    for (;;) {
        Thread::wait(5000);
        printf("count: %d\r\n", packet_count);
        packet_count = 0;
    }
}
