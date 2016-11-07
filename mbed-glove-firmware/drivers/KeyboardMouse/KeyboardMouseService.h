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
#include "HIDServiceBase.h"
#include "keyboard_types.h"


/******************** DEFINITIONS ********************/

#define REPORT_ID_KEYBOARD 1
#define REPORT_ID_MOUSE 2
#define REPORT_ID_VOLUME 3

#define MOUSE_REPORT_LENGTH 5
#define KBD_REPORT_LENGTH 9
#define MAX_REPORT_LENGTH 9

/* Useful definitions for sending and receiving */
static const uint8_t emptyInputReport[MAX_REPORT_LENGTH] = {0};
static uint8_t outputReportData[] = {0};


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



/******************** REPORT DESCRIPTOR ********************/

/* Initialize security manager, and set callback functions and
 * required security level
 */
void initializeSecurity(BLE &ble);

/* Initialize common GAP advertizement and services required by the
 * HID-over-GATT Profile.
 */
void initializeHOGP(BLE &ble);




/******************** REPORT DESCRIPTOR ********************/

report_map_t KB_MOUSE_REPORT_MAP = {
    // Keyboard
    USAGE_PAGE(1),          0x01,
    USAGE(1),               0x06,
    COLLECTION(1),          0x01,
    REPORT_ID(1),           REPORT_ID_KEYBOARD,
    USAGE_PAGE(1),          0x07,
    USAGE_MINIMUM(1),       0xE0,
    USAGE_MAXIMUM(1),       0xE7,
    LOGICAL_MINIMUM(1),     0x00,
    LOGICAL_MAXIMUM(1),     0x01,
    REPORT_SIZE(1),         0x01,
    REPORT_COUNT(1),        0x08,
    INPUT(1),               0x02,
    REPORT_COUNT(1),        0x01,
    REPORT_SIZE(1),         0x08,
    INPUT(1),               0x01,
    REPORT_COUNT(1),        0x05,
    REPORT_SIZE(1),         0x01,
    USAGE_PAGE(1),          0x08,
    USAGE_MINIMUM(1),       0x01,
    USAGE_MAXIMUM(1),       0x05,
    OUTPUT(1),              0x02,
    REPORT_COUNT(1),        0x01,
    REPORT_SIZE(1),         0x03,
    OUTPUT(1),              0x01,
    REPORT_COUNT(1),        0x06,
    REPORT_SIZE(1),         0x08,
    LOGICAL_MINIMUM(1),     0x00,
    LOGICAL_MAXIMUM(2),     0xff, 0x00,
    USAGE_PAGE(1),          0x07,
    USAGE_MINIMUM(1),       0x00,
    USAGE_MAXIMUM(2),       0xff, 0x00,
    INPUT(1),               0x00,
    END_COLLECTION(0),

    // Mouse
    USAGE_PAGE(1),          0x01,           // Generic Desktop
    USAGE(1),               0x02,           // Mouse
    COLLECTION(1),          0x01,           // Application
    USAGE(1),               0x01,           // Pointer
    COLLECTION(1),          0x00,           // Physical
    REPORT_ID(1),           REPORT_ID_MOUSE,
    REPORT_COUNT(1),        0x03,
    REPORT_SIZE(1),         0x01,
    USAGE_PAGE(1),          0x09,           // Buttons
    USAGE_MINIMUM(1),       0x1,
    USAGE_MAXIMUM(1),       0x3,
    LOGICAL_MINIMUM(1),     0x00,
    LOGICAL_MAXIMUM(1),     0x01,
    INPUT(1),               0x02,
    REPORT_COUNT(1),        0x01,
    REPORT_SIZE(1),         0x05,
    INPUT(1),               0x01,
    REPORT_COUNT(1),        0x03,
    REPORT_SIZE(1),         0x08,
    USAGE_PAGE(1),          0x01,
    USAGE(1),               0x30,           // X
    USAGE(1),               0x31,           // Y
    USAGE(1),               0x38,           // scroll
    LOGICAL_MINIMUM(1),     0x81,
    LOGICAL_MAXIMUM(1),     0x7f,
    INPUT(1),               0x06,
    END_COLLECTION(0),
    END_COLLECTION(0),

    // Media Control
    USAGE_PAGE(1),          0x0C,
    USAGE(1),               0x01,
    COLLECTION(1),          0x01,
    REPORT_ID(1),           REPORT_ID_VOLUME,
    USAGE_PAGE(1),          0x0C,
    LOGICAL_MINIMUM(1),     0x00,
    LOGICAL_MAXIMUM(1),     0x01,
    REPORT_SIZE(1),         0x01,
    REPORT_COUNT(1),        0x07,
    USAGE(1),               0xB5,         // Next Track
    USAGE(1),               0xB6,         // Previous Track
    USAGE(1),               0xB7,         // Stop
    USAGE(1),               0xCD,         // Play / Pause
    USAGE(1),               0xE2,         // Mute
    USAGE(1),               0xE9,         // Volume Up
    USAGE(1),               0xEA,         // Volume Down
    INPUT(1),               0x02,         // Input (Data, Variable, Absolute)
    REPORT_COUNT(1),        0x01,
    INPUT(1),               0x01,
    END_COLLECTION(0),
};


/******************** STRUCTURES AND CLASSES ********************/

/* Represent the report descriptor as an array of bytes and
 * in a human-readable format for easy modification */
typedef union {

    /* Keyboard */
    typedef struct {
        uint8_t report_id;      // byte 0
        uint8_t modifier;       // byte 1
        uint8_t unused;         // byte 2
        uint8_t usage;          // byte 3
        uint8_t unused2[5];     // bytes 4-8
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
    KeyboardMouseService(BLE &_ble) :
        HIDServiceBase(_ble,
                KB_MOUSE_REPORT_MAP, sizeof(KB_MOUSE_REPORT_MAP),
                inputReport         = emptyInputReport,
                outputReport        = outputReportData,
                featureReport       = NULL,
                inputReportLength   = sizeof(emptyInputReport),
                outputReportLength  = sizeof(outputReportData),
                featureReportLength = 0,
                reportTickerDelay   = 24)
    {
        
    }
    
    /******************** KEYBOARD INTERFACE ********************/
    
    /* Send a keyboard report */
    ble_error_t sendKbd(uint8_t usage, uint8_t modifier) {
        hid_report.keyboard.report_id = REPORT_ID_KEYBOARD;
        hid_report.keyboard.modifier = modifier;
        hid_report.keyboard.unused = 0;
        hid_report.keyboard.usage = usage;
        memset(hid_report.keyboard.unused2, 0, sizeof(hid_report.keyboard.unused2));
        inputReportLength = KBD_REPORT_LENGTH;
        return send(hid_report.report);
    }
    
    /* Send an empty report, indicating a key was released */
    ble_error_t keyboardRelease(void) { return sendKbd(0, 0); }

    /* Send a report indicating that a keybaord key is being pressed */
    ble_error_t keyboardPress(uint8_t key, uint8_t modifier) {
        return sendKbd(keymap[key].usage, keymap[key].modifier);
    }

    /* Send a single keyboard character */
    void sendChar(char c) {
        sendKbd(keymap[c].usage, keymap[c].modifier); // pressed
        sendKbd(0, 0); // released
    }
    

    /******************** MOUSE INTERFACE ********************/

    /* Send a mouse button press or release */
    virtual void sendCallback() {
        hid_report.mouse.report_id = REPORT_ID_MOUSE;
        hid_report.mouse.buttons = mouse_buttons;
        hid_report.mouse.x_speed = x_speed;
        hid_report.mouse.y_speed = y_speed;
        hid_report.mouse.scroll_speed = scroll_speed;
        memset(hid_report.mouse.unused, 0, sizeof(hid_report.mouse.unused));
        inputReportLength = MOUSE_REPORT_LENGTH;
        send(hid_report.report);
    }
    
    /* Set a button to be pressed or released */
    void setMouseButton(MouseButton button, ButtonState state);
    
    /* Set the speed of the mouse cursor in the x direction */
    void setMouseSpeedX(int8_t speed) { x_speed = speed; }
    
    /* Set the speed of the mouse cursor in the y direction */
    void setMouseSpeedY(int8_t speed) { y_speed = speed; }
    
    /* Set the scroll value of the mouse scroll wheel */
    void setMouseScroll(int8_t speed) { scroll_speed = speed; }
    
    /* Set the x, y, and scroll speed of the mouse */
    void setMouseSpeedAll(int8_t x, int8_t y, int8_t scroll) {
        x_speed = x;
        y_speed = y;
        scroll_speed = scroll;
    }

    /* Set mouse button status */
    void setMouseButton(MouseButton button, ButtonState state) {
        if (state == UP)
            mouse_buttons &= ~(button);
        else /*state == DOWN */
            mouse_buttons |= button;
    }
    
    /* Start report ticker (for mouse only currently) */
    virtual void setConnected(bool connected) {
        connected ? startReportTicker() : stopReportTicker();
        this->connected = connected;
    }
    
protected:
    /* HID report which gets sent over BLE */
    hid_report_t hid_report;

    /* Mouse variables */
    uint8_t mouse_buttons;
    int8_t x_speed;
    int8_t y_speed;
    int8_t scroll_speed;
};

#endif /* KEYBOARD_MOUSE_SVC_H */