/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"
#include "drivers/keyboard_mouse.h"

#define LED_OFF 1
#define LED_ON 0

const char* ble_errors[] = {
    "BLE_ERROR_NONE",
    "BLE_ERROR_BUFFER_OVERFLOW",
    "BLE_ERROR_NOT_IMPLEMENTED",
    "BLE_ERROR_PARAM_OUT_OF_RANGE",
    "BLE_ERROR_INVALID_PARAM",
    "BLE_STACK_BUSY",
    "BLE_ERROR_INVALID_STATE",
    "BLE_ERROR_NO_MEM",
    "BLE_ERROR_OPERATION_NOT_PERMITTED",
    "BLE_ERROR_INITIALIZATION_INCOMPLETE",
    "BLE_ERROR_ALREADY_INITIALIZED",
    "BLE_ERROR_UNSPECIFIED",
    "BLE_ERROR_INTERNAL_STACK_FAILURE"
};

/* LEDs and Buttons */
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);
DigitalOut db(p12);

InterruptIn button1(BUTTON1);
InterruptIn button2(BUTTON2);
InterruptIn button3(BUTTON3);
InterruptIn button4(BUTTON4);


KeyboardMouse * keyboard_ptr;

/* Ticker callback, waiting for commands */
static void waiting() {
    if (!keyboard_ptr->isConnected())
        led1 = !led1; //blink led1
    else
        led2 = !led2; //blink led2
}

// Button Callbacks
void button1pressed() {
    db = 1;
    keyboard_ptr->keyPress('a', 0);
    db = 0;
}
void button1released() {
    db = 1;
    keyboard_ptr->keyRelease();
    db = 0;
}

void button2pressed() {
    db = 1;
    keyboard_ptr->keyPress('b', 0);
    db = 0;
}
void button2released() {
    db = 1;
    keyboard_ptr->keyRelease();
    db = 0;
}

void button3pressed() {
    db = 1;
    keyboard_ptr->setMouseSpeedAll(10, 0, 0);
    db = 0;
}
void button3released() { keyboard_ptr->setMouseSpeedAll(0, 0, 0); }

void button4pressed() {
    db = 1;
    keyboard_ptr->setMouseSpeedAll(-10, 0, 0);
    db = 0;
}
void button4released() {
    db = 1;
    keyboard_ptr->setMouseSpeedAll(0, 0, 0);
    db = 0;
}


ble_error_t err;
Serial dbg(USBTX, USBRX);

void auto_button() {
    db = 1;
    err = keyboard_ptr->keyPress('x', 0);
    db = 0;
    dbg.printf("%s\r\n", ble_errors[err]);

    wait_ms(500);

    db = 1;
    err = keyboard_ptr->keyRelease();
    db = 0;
    dbg.printf("%s\r\n", ble_errors[err]);
}


// MAIN
int keyboard_mouse_demo() {

    wait(5);

    db = 0;
    //printf("start here\r\n");
    KeyboardMouse kbdMouse;
    keyboard_ptr = &kbdMouse;

    //printf("init ticker\r\n");
    Ticker waiting_tick;
    waiting_tick.attach(waiting, 1);

    //printf("init buttons\r\n");
    led1 = LED_OFF;
    led2 = LED_OFF;
    led3 = LED_OFF;
    led4 = LED_OFF;

    Ticker keyboard_test;
    keyboard_test.attach(auto_button, 3);

    /*
    button1.fall(button1pressed);
    button1.rise(button1released);
    button2.fall(button2pressed);
    button2.rise(button2released);
    button3.fall(button3pressed);
    button3.rise(button3released);
    button4.fall(button4pressed);
    button4.rise(button4released);
    */

    while (true) {
        keyboard_ptr->waitForEvent();
    }
}
