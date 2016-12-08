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
    led1 = 1;
    led2 = 1;
    led3 = 1;
    led4 = 1;

    // Debugging
    //button1.fall(stillAlive);
    //button2.fall(press_a);
    //button2.rise(release_a);
    button3.fall(printPacketCounts);
    //Ticker still_alive;
    //still_alive.attach(stillAlive, 1.0);

    // Glove data structs
    glove_sensors_compressed_t leftGloveCompressed;
    //memset(&leftGloveCompressed, 0, sizeof(glove_sensors_compressed_t));
    //leftGloveCompressed.checksum = 5;
    glove_sensors_compressed_t rightGloveCompressed;
    //memset(&rightGloveCompressed, 0, sizeof(glove_sensors_compressed_t));
    //rightGloveCompressed.checksum = 5;
    glove_sensors_raw_t leftGlove;
    glove_sensors_raw_t rightGlove;

    // Initialize ble
    ble.init();

    // Initialize KeyboardMouse object
    KeyboardMouse input(ble);
    //keyboard_ptr = &input;

    // Initialize translators
    Translator leftTranslator(&leftGlove, &leftGloveCompressed, &input);
    Translator rightTranslator(&rightGlove, &rightGloveCompressed, &input);
    TranslateTask combinedTask(&leftTranslator, &rightTranslator, &input);

    // Init scanner
    crcInit();
    Scanner scanner(ble, &leftGloveCompressed, &rightGloveCompressed);

    // Initialize serial interrupts for configuration
    //serialInit(&leftTranslator, &rightTranslator, &scanner);

    // Setup the waitForEvent loop in a different thread
    Thread bleWaitForEvent(bleWaitForEventLoop);
    // Infinite loop with two states
    // Either the keyboard is connected or unconnected
    while (true) {

        // UNCONNECTED STATE
        while (!input.isConnected()) {
            led4 = !led4;
            Thread::wait(10);
        }

        led1 = 1;
        // Wait for connection to take place
        Thread::wait(1000);

        // Start scanning and translating
        //rightTranslator.startUpdateTask(40);
        //Thread::wait(20);
        //leftTranslator.startUpdateTask(40);
        combinedTask.startUpdateTask(100);
        // Scan for packets
        scanner.startScan();

        // CONNECTED STATE
        while (input.isConnected()) {
            led4 = 0;
            Thread::wait(300);
            //leftGlove.touch_sensor.a = 1;
            //input.keyPress('a');
            //input.sendKeyboard();

            led4 = 1;
            Thread::wait(300);
            leftGlove.touch_sensor.a = 0;
            //input.keyRelease('a');
            //input.sendKeyboard();
        }

        led4 = 1;
        rightTranslator.stopUpdateTask();
        leftTranslator.stopUpdateTask();
        scanner.stopScan();
    }
}

glove_sensors_compressed_t left_compressed;
glove_sensors_compressed_t right_compressed;
glove_sensors_raw_t left_raw, left_max, left_min;
glove_sensors_raw_t right_raw, right_max, right_min;

void print_raw_data(glove_sensors_raw_t raw_data) {
    for (int i = 0; i < FLEX_SENSORS_COUNT; ++i) {
        printf("%d, ", raw_data.flex_sensors[i]);
    }

    printf("%d%d%d%d, ",
            raw_data.touch_sensor.a,
            raw_data.touch_sensor.b,
            raw_data.touch_sensor.c,
            raw_data.touch_sensor.d);

    printf("%.2f, %.2f, %.2f\r\n",
            raw_data.imu.orient_pitch,
            raw_data.imu.orient_roll,
            raw_data.imu.orient_yaw);
}

void update_max_min(glove_sensors_raw_t& g_raw,
                    glove_sensors_raw_t& g_max,
                    glove_sensors_raw_t& g_min) {

    // TODO: I could do rolling averages too
    for (int i = 0; i < FLEX_SENSORS_COUNT; ++i) {
        if (g_raw.flex_sensors[i] < g_min.flex_sensors[i]) {
            g_min.flex_sensors[i] = g_raw.flex_sensors[i];
        }
        if (g_raw.flex_sensors[i] > g_max.flex_sensors[i]) {
            g_max.flex_sensors[i] = g_raw.flex_sensors[i];
        }
    }
    if (g_raw.imu.orient_pitch < g_min.imu.orient_pitch) {
        g_min.imu.orient_pitch = g_raw.imu.orient_pitch;
    }
    if (g_raw.imu.orient_pitch > g_max.imu.orient_pitch) {
        g_max.imu.orient_pitch = g_raw.imu.orient_pitch;
    }
    if (g_raw.imu.orient_roll < g_min.imu.orient_roll) {
        g_min.imu.orient_roll = g_raw.imu.orient_roll;
    }
    if (g_raw.imu.orient_roll > g_max.imu.orient_roll) {
        g_max.imu.orient_roll = g_raw.imu.orient_roll;
    }
    if (g_raw.imu.orient_yaw < g_min.imu.orient_yaw) {
        g_min.imu.orient_yaw = g_raw.imu.orient_yaw;
    }
    if (g_raw.imu.orient_yaw > g_max.imu.orient_yaw) {
        g_max.imu.orient_yaw = g_raw.imu.orient_yaw;
    }
}

void decompress_and_print() {
    static int count = 0;
    count += 1;

    extractGloveSensors(&left_raw, &left_compressed);
    extractGloveSensors(&right_raw, &right_compressed);

    printf("Left:  ");
    print_raw_data(left_raw);

    Thread::wait(20);

    printf("Right: ");
    print_raw_data(right_raw);

    printf("\r\n");

    if ((count % 20) == 0) {
        printf("R Max: ");
        print_raw_data(right_max);
        printf("R Min: ", count);
        print_raw_data(right_min);
        printf("L Max: ");
        print_raw_data(left_max);
        printf("L Min: ", count);
        print_raw_data(left_min);
    }
}

// Tested compressing and decompressing
void scan_sensor_data(void) {

    led1 = 1;
    led2 = 1;
    led3 = 1;
    led4 = 1;

    // Initialize scanning
    ble.init();
    Scanner scanner(ble, &left_compressed, &right_compressed);
    scanner.startScan();
    //Thread bleWaitForEvent(bleWaitForEventLoop);

    // Decompress the raw data and print it
    crcInit();
    RtosTimer decompressTask(decompress_and_print);
    decompressTask.start(1000);

    // Spin
    while (true) {
        ble.waitForEvent();
    }
}

int main() {
    /*
     * The current setup with main.cpp is to have single calls to
     * test functions so we can each have test code without having
     * to comment out/have multiple versions.
     * Just change your local one to call the test loop you need.
     */
    //sensors_to_lights();
    //blink();
    //launch_periodic();
    //keyboard_mouse_demo();
    //uart_test();
    //scan_sensor_data();
    launch();
}
