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
#include "drivers/translator.h"
#include "glove_sensors.h"
#include "keyboard_mouse.h"
#include "flex_sensor.h"
#include "touch_sensor.h"

#define LED_OFF 1
#define LED_ON 0

/* LEDs and Buttons */
static DigitalOut led1(LED1);
static DigitalOut led2(LED2);
static DigitalOut led3(LED3);
static DigitalOut led4(LED4);

static InterruptIn button1(BUTTON1);
static InterruptIn button2(BUTTON2);
static InterruptIn button3(BUTTON3);
static InterruptIn button4(BUTTON4);


static KeyboardMouse * keyboard_ptr;

/* Ticker callback, waiting for commands */
static void waiting() {
    if (!keyboard_ptr->isConnected()) led1 = !led1; //blink led1
    else led2 = !led2; //blink led2
}

// Button Callbacks
void button1pressed() {
    keyboard_ptr->keyPress('b');
    keyboard_ptr->sendKeyboard();
}

void button1released() {
    keyboard_ptr->keyRelease('b');
    keyboard_ptr->sendKeyboard();
}

void button2pressed()  {
    keyboard_ptr->keyPress(UP_ARROW);
    keyboard_ptr->sendKeyboard();
}
void button2released() {
    keyboard_ptr->keyRelease(UP_ARROW);
    keyboard_ptr->sendKeyboard();
}

void button3pressed()  {
    keyboard_ptr->setMouseSpeedAll(10, 0, 0);
    keyboard_ptr->sendMouse();
}
void button3released() {
    keyboard_ptr->setMouseSpeedAll(0, 0, 0);
    keyboard_ptr->sendMouse();
}

void button4pressed()  {
    keyboard_ptr->setMouseSpeedAll(-10, 0, 0);
    keyboard_ptr->sendMouse();
}
void button4released() {
    keyboard_ptr->setMouseSpeedAll(0, 0, 0);
    keyboard_ptr->sendMouse();
}



int keyboard_mouse_demo() {

    KeyboardMouse kbdMouse;
    keyboard_ptr = &kbdMouse;

    Callback<void()> wait_callback(waiting);
    RtosTimer wait_timer(wait_callback, osTimerPeriodic);
    wait_timer.start(100);

    led1 = LED_OFF;
    led2 = LED_OFF;
    led3 = LED_OFF;
    led4 = LED_OFF;

    button1.fall(button1pressed);
    button1.rise(button1released);
    button2.fall(button2pressed);
    button2.rise(button2released);
    button3.fall(button3pressed);
    button3.rise(button3released);
    button4.fall(button4pressed);
    button4.rise(button4released);

    while (true) {
        keyboard_ptr->waitForEvent();
    }
}
