#include <inttypes.h>
#include "drivers/scanner.h"
#include "drivers/serial_com.h"
#include "drivers/translator.h"
#include "uart_test.h"

glove_sensors_raw_t leftGlove;
glove_sensors_raw_t rightGlove;
KeyboardMouse * keyboard_ptr;


static void touch_on() {
    keyboard_ptr->keyPress('a');
}

static void touch_off() {
    keyboard_ptr->keyRelease('a');
}


void launch() {
    DigitalOut l1(LED1, 1);
    DigitalOut l2(LED2, 1);
    DigitalOut l3(LED3, 1);
    DigitalOut l4(LED4, 1);

    InterruptIn button(BUTTON1);
    button.fall(touch_on);
    button.rise(touch_off);

    // Initialize ble
    BLE& ble = BLE::Instance(BLE::DEFAULT_INSTANCE);
    ble.init();
    
    /* Initialize KeyboardMouse object */
    KeyboardMouse input;
    keyboard_ptr = &input;

    /* Initialize glove_sensors_raw_t for each glove */

    /* Initialize Translator and Scanner objects */
    //Translator translator(&leftGlove, &rightGlove, &input);
    //Scanner scanner(&translator);

    /* Initialize Serial Interrupt */
    //serialInit(&translator, &scanner);
    //scanner.startScan();
    //scanner.waitForEvent();

    for (;;) {
        ble.waitForEvent();
        //translator.gestureCheck();
        //wait(0.5);
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
