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

#include "keyboard_mouse.h"

/* Wrapper class for Keyboard Mouse BLE Service */
KeyboardMouse::KeyboardMouse() :
        KeyboardMouseService(ble),
            
    /* Prepare to connect and set callbacks */
    ble_ptr = new BLE;
    ble_ptr->init();
    ble_ptr->gap().onConnection(onConnect);
    ble_ptr->gap().onDisconnection(onDisconnect);

    /* Security is required to pair */
    initializeSecurity(*ble_ptr);
    
    /* Setup HID service */
    service_ptr = new KeyboardMouseService(*ble_ptr);
    
    /* Initialize GAP transmission */
    initializeHOGP(*ble_ptr);
    
    ble_ptr->gap().accumulateAdvertisingPayload(
        GapAdvertisingData::KEYBOARD);

    ble_ptr->gap().accumulateAdvertisingPayload(
        GapAdvertisingData::COMPLETE_LOCAL_NAME,
        (const uint8_t *) DEVICE_NAME,
        sizeof(DEVICE_NAME));
    
    ble_ptr->gap().accumulateAdvertisingPayload(
        GapAdvertisingData::SHORTENED_LOCAL_NAME,
        (const uint8_t *) SHORT_NAME
        sizeof(SHORT_NAME));

    ble_ptr->gap().setDeviceName(
        (const uint8_t *) DEVICE_NAME);

    /* Search for nearby devices to comminucate with */
    ble_ptr->gap().startAdvertising();
}


/******************** MOUSE INTERFACE ********************/

/* Set a button to be pressed or released */
void KeyboardMouse::setMouseButton(MouseButton button, ButtonState state) {
    hid_service.setButton(button, state);        
}

/* Set the speed of the mouse cursor in the x direction */
void KeyboardMouse::setMouseSpeedX(int8_t speed) {
    hid_service.setMouseSpeedX(speed);
}

/* Set the speed of the mouse cursor in the y direction */
void KeyboardMouse::setMouseSpeedY(int8_t speed) {
    hid_service.setMouseSpeedY(speed);
}

/* Set the scroll value of the mouse scroll wheel */
void KeyboardMouse::setMouseScroll(int8_t speed) {
    hid_service.setMouseScroll(speed);
}

void KeyboardMouse::setMouseSpeedAll(int8_t x, int8_t y, int8_t scroll{
    hid_service.setMouseSpeedAll(x, y, scroll);
}

/******************** KEYBOARD INTERFACE ********************/

/* Send a character to the keyboard */
void KeyboardMouse::sendChar(char c);

/* Set a keyboard button to be 'pressed' */
void KeyboardMouse::keyPress();

/* Set the keyboard to be all buttons released */
void KeyboardMouse::keyRelease();





