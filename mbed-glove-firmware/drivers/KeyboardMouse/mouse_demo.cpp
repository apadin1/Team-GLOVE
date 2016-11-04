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

#include "ble/BLE.h"
#include "MouseService.h"
#include "KeyboardService.h"

#include "examples_common.h"

/**
 * This program implements a complete HID-over-Gatt Profile:
 *  - HID is provided by KeyboardService
 *  - Battery Service
 *  - Device Information Service
 *
 * Complete strings can be sent over BLE using printf. Please note, however, than a 12char string
 * will take about 500ms to transmit, principally because of the limited notification rate in BLE.
 * KeyboardService uses a circular buffer to store the strings to send, and calls to putc will fail
 * once this buffer is full. This will result in partial strings being sent to the client.
 */


#define LED_ON 0
#define LED_OFF 1

/* LEDs and Buttons */
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

InterruptIn button1(BUTTON1);
InterruptIn button2(BUTTON2);
InterruptIn button3(BUTTON3);
InterruptIn button4(BUTTON4);

BLE ble;
MouseService * mousePtr;
KeyboardService * keyboardPtr;
bool connected = false;

static const char KEYBOARD_DEVICE_NAME[] = "GloveKeyboard";
static const char KEYBOARD_SHORT_NAME[] = "kbd1";
//static const char MOUSE_DEVICE_NAME[] = "GloveMouse";
//static const char MOUSE_SHORT_NAME[] = "mouse1";


// Disconnection Callback
static void onDisconnect(Gap::Handle_t a, Gap::DisconnectionReason_t)
{
    HID_DEBUG("disconnected\r\n");
    connected = false;
    led2 = LED_OFF;
    ble.gap().startAdvertising(); // restart advertising
}

// Connection Callback
static void onConnect(const Gap::ConnectionCallbackParams_t *params)
{
    HID_DEBUG("connected\r\n");
    connected = true;
    led1 = LED_OFF;
    led2 = LED_ON;
}

// Ticker callback, waiting for commands
static void waiting() {
    if (connected)
        led1 = !led1; //blink led1
    else
        led2 = !led2; //blink led2
}

// Button Callbacks
void button1pressed()  { mousePtr->setButton(MOUSE_BUTTON_LEFT, BUTTON_DOWN); }
void button1released() { mousePtr->setButton(MOUSE_BUTTON_LEFT, BUTTON_UP); }

void button2pressed()  { keyboardPtr->keyDownCode('a', 0); }
void button2released() { keyboardPtr->keyUpCode(); }

void button3pressed()  { mousePtr->setSpeed(10, 0, 0); }
void button3released() { mousePtr->setSpeed(0, 0, 0); }

void button4pressed()  { mousePtr->setSpeed(-10, 0, 0); }
void button4released() { mousePtr->setSpeed(0, 0, 0); }

// MAIN
int main() {
    
    HID_DEBUG("init ticker\r\n");
    Ticker waiting_tick;
    waiting_tick.attach(waiting, 1);

    HID_DEBUG("init buttons\r\n");
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
    
    HID_DEBUG("init ble\r\n");
    ble.init();
    ble.gap().onConnection(onConnect);
    ble.gap().onDisconnection(onDisconnect);

    initializeSecurity(ble);

    HID_DEBUG("adding hid services\r\n");
    KeyboardService keyboardService(ble);
    keyboardPtr = &keyboardService;
    MouseService mouseService(ble);
    mousePtr = &mouseService;

    HID_DEBUG("adding device info and battery service\r\n");
    initializeHOGP(ble);

    HID_DEBUG("setting up gap\r\n");
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::KEYBOARD);
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME,
                                           (const uint8_t *)KEYBOARD_DEVICE_NAME, sizeof(KEYBOARD_DEVICE_NAME));
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::SHORTENED_LOCAL_NAME,
                                           (const uint8_t *)KEYBOARD_SHORT_NAME, sizeof(KEYBOARD_SHORT_NAME));

    ble.gap().setDeviceName((const uint8_t *)KEYBOARD_DEVICE_NAME);

    HID_DEBUG("advertising\r\n");

    while (true) {
        ble.waitForEvent();
    }
}
