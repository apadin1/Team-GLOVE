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
 
#include "KeyboardMouseService.h"


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



uint8_t emptyInputReport[MAX_REPORT_LENGTH] = {0};
uint8_t * outputReportData;

/******************** KEYBOARD/MOUSE FUNCTIONS ********************/

/* Constructor */
KeyboardMouseService::KeyboardMouseService(BLE &_ble) :
    HIDServiceBase(_ble,
        KB_MOUSE_REPORT_MAP, sizeof(KB_MOUSE_REPORT_MAP),
        inputReport         = emptyInputReport,
        outputReport        = outputReportData,
        featureReport       = NULL,
        inputReportLength   = sizeof(emptyInputReport),
        outputReportLength  = sizeof(outputReportData),
        featureReportLength = 0,
        reportTickerDelay   = REPORT_TICKER_PERIOD),
            
    /* Mouse variables */
    mouse_buttons(0),
    mouse_x(0),
    mouse_y(0),
    mouse_scroll(0),
    
    /* Keyboard variables */
    modifier(0)
{
    memset(usage, 0, KBD_USAGE_LENGTH);
}


/******************** SENDING REPORTS ********************/

/* Send a keyboard report */
ble_error_t KeyboardMouseService::sendKeyboardReport() {

    /* Build the report, specifying a keyboard report */
    hid_report.keyboard.report_id = REPORT_ID_KEYBOARD;
    hid_report.keyboard.modifier = modifier;
    hid_report.keyboard.reserved = 0;
    memcpy(hid_report.keyboard.usage, this->usage, KBD_USAGE_LENGTH);

    /* Send the report */
    inputReportLength = KBD_REPORT_LENGTH;
    ble_error_t ret = send(hid_report.report);
    return ret;
}

/* Send a mouse report */
ble_error_t KeyboardMouseService::sendMouseReport() {

    /* Build the report, specifying a mouse report */
    hid_report.mouse.report_id = REPORT_ID_MOUSE;
    hid_report.mouse.buttons = mouse_buttons;
    hid_report.mouse.x_speed = mouse_x;
    hid_report.mouse.y_speed = mouse_y;
    hid_report.mouse.scroll_speed = mouse_scroll;
    memset(hid_report.mouse.unused, 0, sizeof(hid_report.mouse.unused));

    /* Send the report */
    inputReportLength = MOUSE_REPORT_LENGTH;
    ble_error_t ret = send(hid_report.report);
    return ret;
}


/******************** KEYBOARD INTERFACE ********************/

/* Send a report indicating that a keybaord key is being pressed */
void KeyboardMouseService::keyPress(uint8_t * keys, int len, uint8_t modifier) {

    /* Set the defined usages */
    if (len > KBD_USAGE_LENGTH) { len = KBD_USAGE_LENGTH; }
    for (int i = 0; i < len; ++i) {
        this->usage[i] = keymap[keys[i]].usage;
    }
    /* Zero out the other usages */
    for (int i = len; i < KBD_USAGE_LENGTH; ++i) {
        this->usage[i] = 0;
    }
    this->modifier = modifier;
}

/* Send a key press without changing the modifier */
void KeyboardMouseService::keyPress(uint8_t * keys, int len) {
    return keyPress(keys, len, this->modifier);
}

/* Send an empty report, indicating all keys released */
void KeyboardMouseService::keyRelease() {

    /* Send an empy report (all zeros) */
    memset(this->usage, 0, KBD_USAGE_LENGTH);
    this->modifier = 0;
}

/******************** MOUSE INTERFACE ********************/

/* Set a button to be pressed or released */
void KeyboardMouseService::setMouseButton(MouseButton button, ButtonState state) {
    if (state == UP)
        this->mouse_buttons &= ~(button);
    else /* state == DOWN */
        this->mouse_buttons |= button;
}

/* Set the speed of the mouse cursor in the x direction */
void KeyboardMouseService::setMouseSpeedX(int8_t speed) {
    this->mouse_x = speed;
}

/* Set the speed of the mouse cursor in the y direction */
void KeyboardMouseService::setMouseSpeedY(int8_t speed) {
    this->mouse_y = speed;
}

/* Set the scroll value of the mouse scroll wheel */
void KeyboardMouseService::setMouseScroll(int8_t speed) {
    this->mouse_scroll = speed;
}

/* Set the x, y, and scroll speed of the mouse */
void KeyboardMouseService::setMouseSpeedAll(int8_t x, int8_t y, int8_t scroll) {
    this->mouse_x = x;
    this->mouse_y = y;
    this->mouse_scroll = scroll;
}

