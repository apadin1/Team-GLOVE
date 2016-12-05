#include <inttypes.h>
//#include "drivers/scanner.h"
//#include "drivers/serial_com.h"
//#include "drivers/translator.h"

extern void calibration_summary(void);

/*
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
*/

/*
static void flex_on() {
    //l1 = !l1;
    leftGlove.touch_sensor.a = 1;
    translator_ptr->gestureCheck();
}

static void flex_off() {
    //l1 = !l1;
    leftGlove.touch_sensor.a = 0;
    translator_ptr->gestureCheck();
}

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
*/

//static void gestureCheck() {
//    l2 = !l2;
//    translator_ptr->gestureCheck();
//}



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
    //launch();
    //uart_test();
    calibration_summary();
}
