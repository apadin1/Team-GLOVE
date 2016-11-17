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
#include "keyboard_mouse.h"
#include "flex_sensor.h"
#include "touch_sensor.h"

#define LED_OFF 1
#define LED_ON 0

/* LEDs and Buttons */
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

DigitalOut pin15(P0_15);
DigitalOut pin16(P0_16);

InterruptIn button1(BUTTON1);
InterruptIn button2(BUTTON2);
InterruptIn button3(BUTTON3);
InterruptIn button4(BUTTON4);

FlexSensors flex_sensors_class;
flex_sensor_t flex_sensor_array[4];

TouchSensor touch_sensor_class;
key_states_t keys;
key_states_t last_keys;

KeyboardMouse * keyboard_ptr;

/* Ticker callback, waiting for commands */
static void waiting() {
    if (!keyboard_ptr->isConnected())
        led1 = !led1; //blink led1
    else
        return;
        led2 = !led2; //blink led2
}

// Button Callbacks
void button1pressed() { keyboard_ptr->keyPress('b'); }
void button1released() { keyboard_ptr->keyRelease('b'); }

void button2pressed() { keyboard_ptr->keyPress(UP_ARROW); }
void button2released() { keyboard_ptr->keyRelease(UP_ARROW); }

void button3pressed() { keyboard_ptr->setMouseSpeedAll(10, 0, 0); }
void button3released() { keyboard_ptr->setMouseSpeedAll(0, 0, 0); }

void button4pressed() { keyboard_ptr->setMouseSpeedAll(-10, 0, 0); }
void button4released() { keyboard_ptr->setMouseSpeedAll(0, 0, 0); }

void translate() {
    pin15 = 1;

    /* Flex Sensors */
    flex_sensors_class.update();
    flex_sensors_class.writeSensors(flex_sensor_array);

    if (flex_sensor_array[0] < 250) { keyboard_ptr->keyPress(' '); }
    else { keyboard_ptr->keyRelease(' '); }

    touch_sensor_class.update();
    touch_sensor_class.writeKeys(&keys);

    /* Touch Sensors */

    //if (last_keys.a != keys.a) {
    //    if (keys.a) keyboard_ptr->keyPress('d');
    //    else        keyboard_ptr->keyRelease('d');
    //}

    if (last_keys.b != keys.b) {
        if (keys.b) keyboard_ptr->keyPress('d');
        else        keyboard_ptr->keyRelease('d');
    }

    if (last_keys.c != keys.c) {
        if (keys.c) keyboard_ptr->keyPress('s');
        else        keyboard_ptr->keyRelease('s');
    }

    if (last_keys.d != keys.d) {
        if (keys.d) keyboard_ptr->keyPress('a');
        else        keyboard_ptr->keyRelease('a');
    }

    last_keys = keys;
    pin15 = 0;

    keyboard_ptr->sendKeyboard();
    keyboard_ptr->sendMouse();
}


// MAIN
int keyboard_mouse_demo() {

    KeyboardMouse kbdMouse;
    keyboard_ptr = &kbdMouse;

    //Ticker waiting_tick;
    //waiting_tick.attach(waiting, 1);

    Callback<void()> wait_callback(waiting);
    RtosTimer wait_timer(wait_callback, osTimerPeriodic);
    wait_timer.start(100);

    led1 = LED_OFF;
    led2 = LED_OFF;
    led3 = LED_OFF;
    led4 = LED_OFF;

    button1.fall(button1pressed);
    button1.rise(button1released);
    //button2.fall(button2pressed);
    //button2.rise(button2released);
    //button3.fall(button3pressed);
    //button3.rise(button3released);
    //button4.fall(button4pressed);
    //button4.rise(button4released);

    Callback<void()> translate_callback(translate);
    RtosTimer translate_timer(translate_callback, osTimerPeriodic);
    translate_timer.start(10);

    while (true) {
        keyboard_ptr->waitForEvent();
    }
}
