#include "drivers/scanner.h"
#include "drivers/serial_com.h"
#include "drivers/translator.h"
#include "glove_sensors.h"
#include "gpio.h"
#include "string.h"
#include "drivers/translate_task.h"

BLE& ble = BLE::Instance(BLE::DEFAULT_INSTANCE);

// TODO: Debug function
extern int left_count;
extern int right_count;
void printPacketCounts() {
    printf("left: %d\r\nright: %d\r\n", left_count, right_count);
}

// Wait for events in a constant loop to make sure the BLE stack
// gets serviced in a reasonable time
void bleWaitForEventLoop() {
    while (true) {
        led3 = 1;
        ble.waitForEvent();
        led3 = 0;
    }
}

// Driver for dongle
void launch() {

    // Turn off LEDs
    led1 = 1; led2 = 1; led3 = 1; led4 = 1;

    // Glove data structs
    static glove_sensors_raw_t left_glove_data;
    static glove_sensors_raw_t right_glove_data;

    // Initialize ble
    ble.init();

    // Initialize KeyboardMouse object
    KeyboardMouse HIDinput(ble);

    // Initialize translators
    Translator leftTranslator(left_glove_data, HIDinput);
    Translator rightTranslator(right_glove_data, HIDinput);
    TranslateTask combinedTask(leftTranslator, rightTranslator, HIDinput);

    // Init scanner
    crcInit();
    Scanner scanner(ble, left_glove_data, right_glove_data);

    // Initialize serial interrupts for configuration
    //serialInit(&leftTranslator, &rightTranslator, &scanner);

    // Setup the waitForEvent loop in a different thread
    Thread bleWaitForEvent(bleWaitForEventLoop);

    // Infinite loop with two states
    // Either the keyboard is connected or unconnected
    while (true) {
        scanner.startScan(100, 25);
        for (;;) {
            printf("%d, %d, | %f %f || %d, %d | %f, %f\r\n",
                    left_glove_data.flex_sensors[0], left_glove_data.flex_sensors[3],
                    left_glove_data.imu.orient_pitch, left_glove_data.imu.orient_roll,
                    right_glove_data.flex_sensors[0], right_glove_data.flex_sensors[3],
                    right_glove_data.imu.orient_pitch, right_glove_data.imu.orient_roll);
            Thread::wait(500);
        }

        // UNCONNECTED STATE
        while (!HIDinput.isConnected()) {
            led4 = !led4;
            Thread::wait(10);
        }

        // Wait for connection to take place
        Thread::wait(5000);

        // Start scanning and translating
        //combinedTask.startUpdateTask(40);
        scanner.startScan(100, 25);
        led1 = 0;

        // CONNECTED STATE
        while (HIDinput.isConnected()) {
            led4 = 0;
            Thread::wait(300);
            led4 = 1;
            Thread::wait(300);
        }
        led2 = 0;

        led4 = 1;
        combinedTask.stopUpdateTask();
        scanner.stopScan();
    }
}

int main() {
    /*
     * The current setup with main.cpp is to have single calls to
     * test functions so we can each have test code without having
     * to comment out/have multiple versions.
     * Just change your local one to call the test loop you need.
     */
    launch();
}
