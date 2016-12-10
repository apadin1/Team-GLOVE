/*
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

#include "gpio.h"
#include "string.h"

#include "drivers/scanner.h"
#include "drivers/serial_com.h"
#include "drivers/translator.h"
#include "drivers/translate_task.h"
#include "drivers/glove_sensors.h"

BLE& ble = BLE::Instance(BLE::DEFAULT_INSTANCE);

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
#define IS_LEFT true
#define IS_RIGHT false

// Driver for dongle
void launch() {

    // Turn off LEDs
    led1 = 1; led2 = 1; led3 = 1; led4 = 1;

    // Glove data structs
    static glove_sensors_raw_t left_glove_data;
    static glove_sensors_raw_t right_glove_data;
    static glove_sensors_compressed_t left_compressed;
    static glove_sensors_compressed_t right_compressed;

    // Initialize ble
    ble.init();

    // Initialize KeyboardMouse object
    KeyboardMouse HIDinput(ble);

    flexToHID flex_sensors_L[FLEX_COUNT];
    touchToHID touch_sensors_L[TOUCH_COUNT];
    imuToHID imu_axis_L[IMU_COUNT];
    flexToHID flex_sensors_R[FLEX_COUNT];
    touchToHID touch_sensors_R[TOUCH_COUNT];
    imuToHID imu_axis_R[IMU_COUNT];

    // Initialize translators
    Translator leftTranslator(left_glove_data, HIDinput, IS_LEFT, flex_sensors_L, touch_sensors_L, imu_axis_L);
    Translator rightTranslator(right_glove_data, HIDinput, IS_RIGHT, flex_sensors_R, touch_sensors_R, imu_axis_R);
    TranslateTask combinedTask(leftTranslator, rightTranslator, HIDinput);

    // Init scanner
    crcInit();
    //Scanner scanner(ble, left_glove_data, right_glove_data);
    Scanner scanner(ble, &left_compressed, &right_compressed, left_glove_data, right_glove_data);

    // Initialize serial interrupts for configuration
    //serialInit(&leftTranslator, &rightTranslator, &scanner);

    // Setup the waitForEvent loop in a different thread
    Thread bleWaitForEvent(bleWaitForEventLoop);

    // Infinite loop with two states
    // Either the keyboard is connected or unconnected
    while (true) {
#if 0
        scanner.startScan(100, 25);
        for (;;) {
            led1 != led1;
            extractGloveSensors(left_glove_data, &left_compressed);
            extractGloveSensors(right_glove_data, &right_compressed);
            printf("%d, %d, | %f %f || %d, %d | %f, %f\r\n",
                    left_glove_data.flex_sensors[0], left_glove_data.flex_sensors[3],
                    left_glove_data.imu.orient_pitch, left_glove_data.imu.orient_roll,
                    right_glove_data.flex_sensors[0], right_glove_data.flex_sensors[3],
                    right_glove_data.imu.orient_pitch, right_glove_data.imu.orient_roll);
            Thread::wait(500);
        }
#endif

        // UNCONNECTED STATE
        while (!HIDinput.isConnected()) {
            led4 = !led4;
            Thread::wait(10);
        }

        // Wait for connection to take place
        Thread::wait(3000);

        // Start scanning and translating
        combinedTask.startUpdateTask(50);
        scanner.startScan(100, 20);
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

void blink() {
    led2 = 1;
    led1 = 0;
    for (;;) {
        led1 = !led1;
        led2 = !led2;
        printf("Hi!\r\n");
        Thread::wait(520);
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
