/* Copyright (c) 2010-2011 mbed.org, MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef MOUSE_SVC_H
#define MOUSE_SVC_H

#include "mbed.h"
#include "HIDServiceBase.h"

enum ButtonState
{
    BUTTON_UP,
    BUTTON_DOWN
};

enum MouseButton
{
    MOUSE_BUTTON_LEFT    = 0x1,
    MOUSE_BUTTON_RIGHT   = 0x2,
    MOUSE_BUTTON_MIDDLE  = 0x4,
};

report_map_t MOUSE_REPORT_MAP = {
    USAGE_PAGE(1),      0x01,         // Generic Desktop
    USAGE(1),           0x02,         // Mouse
    COLLECTION(1),      0x01,         // Application
    USAGE(1),           0x01,         //  Pointer
    COLLECTION(1),      0x00,         //  Physical
    USAGE_PAGE(1),      0x09,         //   Buttons
    USAGE_MINIMUM(1),   0x01,
    USAGE_MAXIMUM(1),   0x03,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x01,
    REPORT_COUNT(1),    0x03,         //   3 bits (Buttons)
    REPORT_SIZE(1),     0x01,
    INPUT(1),           0x02,         //   Data, Variable, Absolute
    REPORT_COUNT(1),    0x01,         //   5 bits (Padding)
    REPORT_SIZE(1),     0x05,
    INPUT(1),           0x01,         //   Constant
    USAGE_PAGE(1),      0x01,         //   Generic Desktop
    USAGE(1),           0x30,         //   X
    USAGE(1),           0x31,         //   Y
    USAGE(1),           0x38,         //   Wheel
    LOGICAL_MINIMUM(1), 0x81,         //   -127
    LOGICAL_MAXIMUM(1), 0x7f,         //   127
    REPORT_SIZE(1),     0x08,         //   Three bytes
    REPORT_COUNT(1),    0x03,
    INPUT(1),           0x06,         //   Data, Variable, Relative
    END_COLLECTION(0),
    END_COLLECTION(0),
};

uint8_t report[] = { 0, 0, 0, 0 };

class MouseService: public HIDServiceBase
{
public:
    MouseService(BLE &_ble) :
        HIDServiceBase(_ble,
                       MOUSE_REPORT_MAP, sizeof(MOUSE_REPORT_MAP),
                       inputReport          = report,
                       outputReport         = NULL,
                       featureReport        = NULL,
                       inputReportLength    = sizeof(inputReport),
                       outputReportLength   = 0,
                       featureReportLength  = 0,
                       reportTickerDelay    = 20),
        buttonsState (0),
        failedReports (0)
    {
        speed[0] = 0;
        speed[1] = 0;
        speed[2] = 0;

        startReportTicker();
    }

    int setSpeed(int8_t x, int8_t y, int8_t wheel)
    {
        speed[0] = x;
        speed[1] = y;
        speed[2] = wheel;

        return 0;
    }

    int setButton(MouseButton button, ButtonState state)
    {
        if (state == BUTTON_UP)
            buttonsState &= ~(button);
        else
            buttonsState |= button;

        return 0;
    }

    virtual void sendCallback(void) {
        if (!connected)
            return;

        report[0] = buttonsState & 0x7;
        report[1] = speed[0];
        report[2] = speed[1];
        report[3] = speed[2];

        if (send(report))
            failedReports++;
    }

protected:
    uint8_t buttonsState;
    uint8_t speed[3];

public:
    uint32_t failedReports;
};



#endif /* MOUSE_SVC_H */