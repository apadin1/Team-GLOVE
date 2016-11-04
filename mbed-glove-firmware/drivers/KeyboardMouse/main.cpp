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
#include "KeyboardMouseService.h"
#include "examples_common.h"

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
KeyboardMouseService * service_ptr;

static const char DEVICE_NAME[] = "THE GLOVE";
static const char SHORT_DEVICE_NAME[] = "kbd1";

// Disconnection Callback
static void onDisconnect(Gap::Handle_t a, Gap::DisconnectionReason_t)
{
    HID_DEBUG("disconnected\r\n");
    service_ptr->setConnected(false);
    led2 = LED_OFF;
    ble.gap().startAdvertising(); // restart advertising
    
}

// Connection Callback
static void onConnect(const Gap::ConnectionCallbackParams_t *params)
{
    HID_DEBUG("connected\r\n");
    service_ptr->setConnected(true);
    led1 = LED_OFF;
    led2 = LED_ON;
}

// Ticker callback, waiting for commands
static void waiting() {
    if (!service_ptr->isConnected())
        led1 = !led1; //blink led1
    else
        led2 = !led2; //blink led2
}

// Button Callbacks
void button1pressed() { service_ptr->keyDown(DOWN_ARROW, 0); }
void button1released() { service_ptr->keyUp(); }

void button2pressed() { service_ptr->setSpeed(20, 0, 0); }
void button2released() { service_ptr->setSpeed(0, 0, 0); }

void button3pressed() { service_ptr->setSpeed(-20, 0, 0); }
void button3released() { service_ptr->setSpeed(0, 0, 0); }

void button4pressed() { service_ptr->keyDown(UP_ARROW, 0); }
void button4released() { service_ptr->keyUp(); }


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
    ble.gap().onDisconnection(onDisconnect);
    ble.gap().onConnection(onConnect);

    initializeSecurity(ble);

    HID_DEBUG("adding hid service\r\n");
    KeyboardMouseService kbdService(ble);
    service_ptr = &kbdService;

    HID_DEBUG("adding device info and battery service\r\n");
    initializeHOGP(ble);

    HID_DEBUG("setting up gap\r\n");
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::KEYBOARD);
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME,
                                           (const uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::SHORTENED_LOCAL_NAME,
                                           (const uint8_t *)SHORT_DEVICE_NAME, sizeof(SHORT_DEVICE_NAME));

    ble.gap().setDeviceName((const uint8_t *)DEVICE_NAME);

    HID_DEBUG("advertising\r\n");
    ble.gap().startAdvertising();

    while (true) {
        ble.waitForEvent();
    }
}
