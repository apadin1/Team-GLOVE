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

#ifndef KEYBOARD_MOUSE_SVC_H
#define KEYBOARD_MOUSE_SVC_H

#include "mbed.h"
#include "ble/BLE.h"
#include "HIDServiceBase.h"
#include "Keyboard_types.h"


/******************** DEFINITIONS ********************/

#define REPORT_ID_KEYBOARD 1
#define REPORT_ID_MOUSE 2
#define REPORT_ID_VOLUME 3

#define MOUSE_REPORT_LENGTH 5
#define KBD_REPORT_LENGTH 9
#define KBD_USAGE_LENGTH 6
#define MAX_REPORT_LENGTH 9
#define REPORT_TICKER_PERIOD 100


/******************** ENUMERATIONS ********************/

/* Mouse button state */
enum ButtonState { UP, DOWN };

/* Mouse button select */
enum MouseButton {
    LEFT   = 0x1,
    RIGHT  = 0x2,
    MIDDLE = 0x4
};

/* Mouse direction select */
enum MouseDirection { X, Y, SCROLL };


/******************** STRUCTURES AND CLASSES ********************/

/* Represent the report descriptor as an array of bytes and
 * in a human-readable format for easy modification */
typedef union {

    /* Keyboard */
    typedef struct {
        uint8_t report_id;      // byte 0
        uint8_t modifier;       // byte 1
        uint8_t reserved;       // byte 2
        uint8_t usage[KBD_USAGE_LENGTH];       // bytes 3-8
    } keyboard_t;

    /* MOUSE */
    typedef struct {
        uint8_t report_id;      // byte 0
        uint8_t buttons;        // byte 1
        uint8_t x_speed;        // byte 2
        uint8_t y_speed;        // byte 3
        uint8_t scroll_speed;   // byte 4
        uint8_t unused[4];      // bytes 5-8
    }  mouse_t;

    /* Report sent over BLE */
    uint8_t report[MAX_REPORT_LENGTH];
    mouse_t mouse;
    keyboard_t keyboard;

} hid_report_t;


/* Class for using both mouse and keyboard services together */
class KeyboardMouseService : public HIDServiceBase {

public:

    /* Constructor */
    KeyboardMouseService(BLE &_ble);

    /******************** SENDING REPORTS ********************/
    ble_error_t sendKeyboardReport();
    ble_error_t sendMouseReport();

    /******************** KEYBOARD INTERFACE ********************/

    /* Set the values for which keys are pressed */
    void keyPress(uint8_t * keys, int len, uint8_t modifier);

    /* Set the key values without changing the modifier */
    void keyPress(uint8_t * keys, int len);

    /* Send an empty report, indicating all keys were released */
    void keyRelease(void);

    /******************** MOUSE INTERFACE ********************/

    /* Set a button to be pressed or released */
    void setMouseButton(MouseButton button, ButtonState state);

    /* Set the speed of the mouse cursor in any or all directions */
    void setMouseSpeedX(int8_t speed);
    void setMouseSpeedY(int8_t speed);
    void setMouseScroll(int8_t speed);
    void setMouseSpeedAll(int8_t x, int8_t y, int8_t scroll);


    /******************** OTHER FUNCTIONS ********************/

    /* Callback for periodic sending of packets */
    virtual void sendCallback(void) { return; }

    /* Setter for connected bool */
    virtual void setConnected(bool connected) {
        this->connected = connected;
    }


private:

    /******************** PRIVATE VARIABLES ********************/

    /* HID report which gets sent over BLE */
    hid_report_t hid_report;

    /* Mouse variables */
    uint8_t mouse_buttons;
    int8_t mouse_x;
    int8_t mouse_y;
    int8_t mouse_scroll;

    /* Keyboard variables */
    uint8_t usage[KBD_USAGE_LENGTH];
    uint8_t modifier;

};


#endif /* KEYBOARD_MOUSE_SVC_H */
