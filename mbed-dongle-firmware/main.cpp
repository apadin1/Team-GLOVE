#include <inttypes.h>
#include "drivers/scanner.h"
#include "drivers/serial_com.h"
#include "drivers/translator.h"
#include "uart_test.h"

glove_sensors_raw_t leftGlove;
glove_sensors_raw_t rightGlove;
static KeyboardMouse * keyboard_ptr;
static Translator * translator_ptr;
static Scanner * scanner_ptr;

static DigitalOut l1(LED1, 1);
static DigitalOut l2(LED2, 1);
static DigitalOut l3(LED3, 1);
static DigitalOut l4(LED4, 1);

extern int left_count;
extern int right_count;

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

void printPacketCounts() {
    printf("left: %d\r\nright: %d\r\n", left_count, right_count);
}

void waitForEventAndNothingElse() {
    for (;;) {
        keyboard_ptr->waitForEvent();
    }
}

void launch() {

    // Turn off LEDs
    l1 = 1; l2 = 1; l3 = 1; l4 = 1;

    // Setup buttons for testing
    //InterruptIn button2(BUTTON2);
    //InterruptIn button3(BUTTON3);

    //button2.fall(press_a);
    //button2.rise(release_a);

    //button3.fall(printPacketCounts);

    // Initialize ble
    BLE& ble = BLE::Instance(BLE::DEFAULT_INSTANCE);
    ble.init();

    // Initialize KeyboardMouse object
    KeyboardMouse input(ble);
    keyboard_ptr = &input;

    while (!input.isConnected()) {
      ble.waitForEvent();
    }

    // Initialize Translator and Scanner objects
    Translator translator(&leftGlove, &input);
    translator.startUpdateTask(20);
    translator_ptr = &translator;
    Scanner scanner(&translator);

    //Thread waitForFuckingEvent(waitForEventAndNothingElse);

    // Initialize serial interrupt
    //serialInit(&translator, &scanner);

    leftGlove.touch_sensor.a = 1;
    //Inifite loop
    for (;;) {
        l4 = !l4;

        // Scan for packets
        //scanner.startScan();
        //wait( 0.02 );
        //scanner.stopScan();

        leftGlove.touch_sensor.a = !leftGlove.touch_sensor.a;

        Thread::wait(500);
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
