#include "drivers/scanner.h"
#include "drivers/serial_com.h"
#include "drivers/translator.h"
#include "gpio.h"
#include "drivers/crc.h"

glove_sensors_raw_t leftGlove;
glove_sensors_raw_t rightGlove;
static KeyboardMouse * keyboard_ptr;
static Translator * translator_ptr;
static Scanner * scanner_ptr;

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
        pin10 = 1;
        ble.waitForEvent();
        pin10 = 0;
    }
}


// Driver for dongle
void launch() {

    // Turn off LEDs
    led1 = 1;
    led2 = 1;
    led3 = 1;
    led4 = 1;

    // needed for the extractGloveData to work
    crcInit();

    // Setup buttons for testing
    //button2.fall(press_a);
    //button2.rise(release_a);

    button3.fall(printPacketCounts);

    //Ticker still_alive;
    //still_alive.attach(stillAlive, 1.0);
    //button1.fall(stillAlive);

    // Initialize ble
    ble.init();

    // Initialize KeyboardMouse object
    KeyboardMouse input(ble);
    keyboard_ptr = &input;

    // Initialize translator and scanner
    Translator translator(&leftGlove, &input);
    translator_ptr = &translator;
    Scanner scanner(ble, &translator);
    scanner_ptr = &scanner;

    // Initialize serial interrupts for configuration
    serialInit(&translator, &scanner);

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

        // Wait for connection to take place
        Thread::wait(1000);

        // Start scanning and translating
        translator.startUpdateTask(30);

        // Scan for packets
        scanner.startScan();

        // CONNECTED STATE
        while (input.isConnected()) {
            led4 = 0;
            Thread::wait(50);
            leftGlove.touch_sensor.a = 1;

            led4 = 1;
            Thread::wait(500);
            leftGlove.touch_sensor.a = 0;
        }

        led4 = 1;
        translator.stopUpdateTask();
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
    //sensors_to_lights();
    //blink();
    //launch_periodic();
    //keyboard_mouse_demo();
    launch();
    //uart_test();
}
