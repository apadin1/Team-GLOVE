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
 * See the License for the specific language governing permissions and *
 *limitations under the License.
 */

#include "mbed.h"

#include "HIDServiceBase.h"

#define REPORT_ID_JOYSTICK 4

/* Common usage */
enum JOY_BUTTON {
    JOY_B0 = 0x01,
    JOY_B1 = 0x02,
    JOY_B2 = 0x04,
    JOY_B3 = 0x08,
    JOY_B4 = 0x10,
    JOY_B5 = 0x20,
    JOY_B6 = 0x40,
    JOY_B7 = 0x80,
};

enum JOY_HAT {
    JOY_HAT_UP = 0,
    JOY_HAT_RIGHT = 1,
    JOY_HAT_DOWN = 2,
    JOY_HAT_LEFT = 3,
    JOY_HAT_NEUTRAL = 4,
};

/* X, Y and T limits */
/* These values do not directly map to screen pixels */
/* Zero may be interpreted as meaning 'no movement' */
#define JX_MIN_ABS (-127) /*!< The maximum value that we can move to the left on the x-axis */
#define JY_MIN_ABS (-127) /*!< The maximum value that we can move up on the y-axis */
#define JZ_MIN_ABS (-127) /*!< The maximum value that we can move up on the y-axis */
#define JT_MIN_ABS (-127) /*!< The minimum value for the throttle */
#define JX_MAX_ABS (127) /*!< The maximum value that we can move to the right on the x-axis */
#define JY_MAX_ABS (127) /*!< The maximum value that we can move down on the y-axis */
#define JZ_MAX_ABS (127) /*!< The maximum value that we can move down on the y-axis */
#define JT_MAX_ABS (127) /*!< The maximum value for the throttle */

DigitalOut l4(LED4);
DigitalOut l2(LED2);
extern Serial pc;

report_map_t JOYSTICK_REPORT_MAP = {

    USAGE_PAGE(1),       0x01,  // Generic Desktop
    USAGE(1),            0x04,  // Usage (Joystick)
    COLLECTION(1),       0x01,  // Application
        USAGE_PAGE(1),       0x02,  // Simulation Controls

        USAGE(1),            0xBB,  // Throttle
        LOGICAL_MINIMUM(1),  0x81,  // -127
        LOGICAL_MAXIMUM(1),  0x7f,  // 127
        REPORT_SIZE(1),      0x08,
        REPORT_COUNT(1),     0x02,
        INPUT(1),            0x02,  // Data, Variable, Absolute

        USAGE_PAGE(1),       0x01,  // Generic Desktop
        USAGE(1),            0x01,  // Pointer
        COLLECTION(1),       0x00,  // Physical (IMU/stick)
            USAGE(1),            0x30,  // X
            USAGE(1),            0x31,  // Y
            USAGE(1),            0x32,  // Z
            LOGICAL_MINIMUM(1),  0x81,  // -127
            LOGICAL_MAXIMUM(1),  0x7f,  // 127
            REPORT_SIZE(1),      0x08,
            REPORT_COUNT(1),     0x03,
            INPUT(1),            0x02,  // Data, Variable, Absolute
        END_COLLECTION(0),

        // Buttons
        USAGE_PAGE(1),       0x09,  // Buttons
        USAGE_MINIMUM(1),    0x01,  // 1
        USAGE_MAXIMUM(1),    0x08,  // 8
        LOGICAL_MINIMUM(1),  0x00,  // 0
        LOGICAL_MAXIMUM(1),  0x01,  // 1
        PHYSICAL_MINIMUM(1),  0x00,  // 0
        PHYSICAL_MAXIMUM(1),  0x01,  // 1
        REPORT_SIZE(1),      0x01,
        REPORT_COUNT(1),     0x08,
        UNIT(1),             0x00,  // Unit (None)
        INPUT(1),            0x02,  // Data, Variable, Absolute
    END_COLLECTION(0)
};

uint8_t report[] = { 0, 0, 0, 0, 0 };

class JoystickService : public HIDServiceBase {
public:
    JoystickService(BLE& _ble)
        : HIDServiceBase(_ble,
                JOYSTICK_REPORT_MAP,
                sizeof(JOYSTICK_REPORT_MAP),
                inputReport = report,
                outputReport = NULL,
                featureReport = NULL,
                inputReportLength = sizeof(inputReport),
                outputReportLength = 0,
                featureReportLength = 0,
                reportTickerDelay = 20),
          _t(-127),
          _x(0),
          _y(0),
          _z(0),
          _button(0x00),
          failedReports(0) {

        //startReportTicker();
    }

    void throttle(int16_t t) {
        _t = t;
    }

    void roll(int16_t z){
        _z = z;
    }
    void pitch(int16_t x) {
        _x = x;
    }

    void yaw(uint16_t y) {
        _y = y;
    }

    void button(uint8_t button) {
        _button = button;
    }

    void sendReport(void) {
        l4 = 1;
        if (!connected)
            return;
        // Fill the report according to the Joystick Descriptor
        report[0] = _t & 0xff;
        report[2] = _x & 0xff;
        report[3] = _y & 0xff;
        report[1] = _z & 0xff;
        //report[4] = ((_button & 0x0f) << 4) | (_hat & 0x0f);
        report[4] = _button;

        uint8_t ret = send(report);
        //if (send(report)) {
        //TODO need to figure out which error we've been getting from send
        if (ret) {
            failedReports++;

            for (uint8_t i=0; i<ret; ++i) {
                l2 = 0;
                wait_ms(250);
                l2 = 1;
            }
        }

        wait_ms(30);
        l4 = 0;
    }

    void sendCallback(void) {
        sendReport();
    }

private:
    int8_t _t;
    int8_t _x;
    int8_t _y;
    int8_t _z;
    uint8_t _button;

public:
    uint32_t failedReports;
};
