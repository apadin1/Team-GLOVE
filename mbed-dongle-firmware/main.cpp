#include "drivers/scanner.h"
#include "drivers/serial_com.h"
#include "drivers/translator.h"
#include "glove_sensors.h"
#include "gpio.h"

//static KeyboardMouse * keyboard_ptr;
//static Translator * translator_ptr;
//static Scanner * scanner_ptr;

BLE& ble = BLE::Instance(BLE::DEFAULT_INSTANCE);

extern int left_count;
extern int right_count;


/*
static void press_a() {
    //l2 = !l2;
    keyboard_ptr->keyPress('a');
    //keyboard_ptr->sendKeyboard();
}

static void release_a() {
    //l2 = !l2;
    keyboard_ptr->keyRelease('a');
    //keyboard_ptr->sendKeyboard();
}
*/

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
    glove_sensors_compressed_t rightGloveCompressed;
    glove_sensors_raw_t leftGlove;
    glove_sensors_raw_t rightGlove;
    
    // Initialize ble
    ble.init();

    // Initialize KeyboardMouse object
    KeyboardMouse input(ble);
    //keyboard_ptr = &input;

    // Initialize translators
    Translator leftTranslator(&leftGlove,
                              &leftGloveCompressed,
                              &input);

    Translator rightTranslator(&rightGlove,
                               &rightGloveCompressed,
                               &input);

    // Init scanner
    crcInit();
    Scanner scanner(ble,
                    &leftGloveCompressed,
                    &rightGloveCompressed);

    // Initialize serial interrupts for configuration
    //serialInit(&translator, &scanner);

    // Setup the waitForEvent loop in a different thread
    Thread bleWaitForEvent(bleWaitForEventLoop);

    // Infinite loop with two states
    // Either the keyboard is connected or unconnected
    while (true) {

        // UNCONNECTED STATE
        while (!input.isConnected()) {
            led1 = !led1;
            Thread::wait(10);
        }
        
        led1 = 1;
        // Wait for connection to take place
        Thread::wait(1000);

        // Start scanning and translating
        rightTranslator.startUpdateTask(50);
        leftTranslator.startUpdateTask(50);

        // Scan for packets
        scanner.startScan();

        // CONNECTED STATE
        while (input.isConnected()) {
            led4 = 0;
            //Thread::wait(500);
            //leftGlove.touch_sensor.a = 1;
            //input.keyPress('a');
            //input.sendKeyboard();
            
            led4 = 1;
            //Thread::wait(500);
            //leftGlove.touch_sensor.a = 0;
            //input.keyRelease('a');
            //input.sendKeyboard();
        }

        led4 = 1;
        rightTranslator.stopUpdateTask();
        leftTranslator.stopUpdateTask();
        scanner.stopScan();
    }
}

/*
void decompress_and_print() {
    static glove_sensors_raw_t raw_data;
    static int count = 0;
    
    count += 1;
    
    //printf("%d: ", right_count);
    
    printf("[%d]: ", count);
    
    uint8_t * compressed_ptr = (uint8_t *) &compressed_data;
    for (int i = 0; i < sizeof(compressed_data); ++i) {
        printf("%d, ", compressed_ptr[i]);
    }
    
    int error = extractGloveSensors(&raw_data, &rightGloveCompressed);
    //printf("Error: %d", error);
        
    for (int i = 0; i < FLEX_SENSORS_COUNT; ++i) {
        printf("%d, ", raw_data.flex_sensors[i]);
    }

    printf("%d, %d, %d, %d, ", 
            raw_data.touch_sensor.a,
            raw_data.touch_sensor.b,
            raw_data.touch_sensor.c,
            raw_data.touch_sensor.d);
    
    printf("%.2f, %.2f, %.2f, %.2f, %.2f, %.2f",
            raw_data.imu.orient_pitch,
            raw_data.imu.orient_roll,
            raw_data.imu.orient_yaw,
            raw_data.imu.accel_x,
            raw_data.imu.accel_y,
            raw_data.imu.accel_z);
            
    printf("\r\n");
}


// Tested compressing and decompressing
void scan_sensor_data(void) {
    
    led1 = 1;
    led2 = 1;
    led3 = 1;
    led4 = 1;

    // Initialize scanning
    ble.init();
    Scanner scanner(ble, NULL);
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
*/



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
