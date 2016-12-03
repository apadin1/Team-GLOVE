#include <inttypes.h>
#include "drivers/scanner.h"
#include "drivers/serial_com.h"
#include "drivers/translator.h"
#include "uart_test.h"

static glove_sensors_raw_t leftGlove;
static glove_sensors_raw_t rightGlove;
static KeyboardMouse * keyboard_ptr;
static Translator * translator_ptr;
static Scanner * scanner_ptr;

static BLE& ble = BLE::Instance(BLE::DEFAULT_INSTANCE);

static DigitalOut l1(LED1, 1);
static DigitalOut l2(LED2, 1);
static DigitalOut l3(LED3, 1);
static DigitalOut l4(LED4, 1);


static void flex_on() {
    l1 = !l1;
    leftGlove.touch_sensor.a = 1;
}

static void flex_off() {
    l1 = !l1;
    leftGlove.touch_sensor.a = 0;
}

static void press_a() {
    l2 = !l2;
    keyboard_ptr->keyPress('a');
    keyboard_ptr->sendKeyboard();
}

static void release_a() {
    l2 = !l2;
    keyboard_ptr->keyRelease('a');
    keyboard_ptr->sendKeyboard();
}


//static void gestureCheck() {
//    l2 = !l2;
//    translator_ptr->gestureCheck();
//}


void launch() {

    // Setup buttons for testing
    InterruptIn button1(BUTTON1);
    InterruptIn button2(BUTTON2);

    button1.fall(flex_on);
    button1.rise(flex_off);
    
    button2.fall(press_a);
    button2.rise(release_a);

    // Initialize ble
    ble.init();
    
    /* Initialize KeyboardMouse object */
    KeyboardMouse input(ble);
    keyboard_ptr = &input;

    /* Initialize glove_sensors_raw_t for each glove */

    /* Initialize Translator and Scanner objects */
    Translator translator(&leftGlove, &rightGlove, &input);
    translator_ptr = &translator;
    Scanner scanner(&translator);

    /* Initialize Serial Interrupt */
    //serialInit(&translator, &scanner);
    //scanner.startScan();
    //scanner.waitForEvent();
    
    // Translator ticker
    //Ticker translate_ticker;
    //translate_ticker.attach(gestureCheck, 0.1);
    
    while (!input.isConnected()) {
        l3 = !l3;
        ble.waitForEvent();
    }
    
    for (;;) {
        l3 = !l3;
        //if (check_gestures) {
        translator.gestureCheck();
        //ble.waitForEvent();
        wait(0.2);
        //Inifite loop
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
