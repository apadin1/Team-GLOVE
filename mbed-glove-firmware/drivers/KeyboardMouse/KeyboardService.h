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

#ifndef KEYBOARD_SVC_H
#define KEYBOARD_SVC_H

#include <errno.h>
#include "mbed.h"
#include "CircularBuffer.h"

#include "HIDServiceBase.h"
#include "Keyboard_types.h"

/* TODO: make this easier to configure by application (e.g. as a template parameter for
 * KeyboardService) */
#ifndef KEYBUFFER_SIZE
#define KEYBUFFER_SIZE 512
#endif

report_map_t KEYBOARD_REPORT_MAP = {
    USAGE_PAGE(1),      0x01,       // Generic Desktop Ctrls
    USAGE(1),           0x06,       // Keyboard
    COLLECTION(1),      0x01,       // Application
    USAGE_PAGE(1),      0x07,       //   Kbrd/Keypad
    USAGE_MINIMUM(1),   0xE0,
    USAGE_MAXIMUM(1),   0xE7,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x01,
    REPORT_SIZE(1),     0x01,       //   1 byte (Modifier)
    REPORT_COUNT(1),    0x08,
    INPUT(1),           0x02,       //   Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position
    REPORT_COUNT(1),    0x01,       //   1 byte (Reserved)
    REPORT_SIZE(1),     0x08,
    INPUT(1),           0x01,       //   Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position
    REPORT_COUNT(1),    0x05,       //   5 bits (Num lock, Caps lock, Scroll lock, Compose, Kana)
    REPORT_SIZE(1),     0x01,
    USAGE_PAGE(1),      0x08,       //   LEDs
    USAGE_MINIMUM(1),   0x01,       //   Num Lock
    USAGE_MAXIMUM(1),   0x05,       //   Kana
    OUTPUT(1),          0x02,       //   Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile
    REPORT_COUNT(1),    0x01,       //   3 bits (Padding)
    REPORT_SIZE(1),     0x03,
    OUTPUT(1),          0x01,       //   Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile
    REPORT_COUNT(1),    0x06,       //   6 bytes (Keys)
    REPORT_SIZE(1),     0x08,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x65,       //   101 keys
    USAGE_PAGE(1),      0x07,       //   Kbrd/Keypad
    USAGE_MINIMUM(1),   0x00,
    USAGE_MAXIMUM(1),   0x65,
    INPUT(1),           0x00,       //   Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position
    END_COLLECTION(0),
};

static uint8_t inputReportData[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static const uint8_t emptyInputReportData[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static uint8_t outputReportData[] = { 0 };

/**
 * @class KeyBuffer
 *
 * Buffer used to store keys to send.
 * Internally, it is a CircularBuffer, with the added capability of putting the last char back in,
 * when we're unable to send it (ie. when BLE stack is busy)
 */
class KeyBuffer: public CircularBuffer<uint8_t, KEYBUFFER_SIZE>
{
public:
    KeyBuffer() :
        CircularBuffer(),
        dataIsPending (false),
        keyUpIsPending (false)
    {
    }

    /**
     * Mark a character as pending. When a freshly popped character cannot be sent, because the
     * underlying stack is busy, we set it as pending, and it will get popped in priority by @ref
     * getPending once reports can be sent again.
     *
     * @param data  The character to send in priority. The second keyUp report is implied.
     */
    void setPending(uint8_t data)
    {
        MBED_ASSERT(dataIsPending == false);

        dataIsPending = true;
        pendingData = data;
        keyUpIsPending = true;
    }

    /**
     * Get pending char. Either from the high priority buffer (set with setPending), or from the
     * circular buffer.
     *
     * @param   data Filled with the pending data, when present
     * @return  true if data was filled
     */
    bool getPending(uint8_t &data)
    {
        if (dataIsPending) {
            dataIsPending = false;
            data = pendingData;
            return true;
        }

        return pop(data);
    }

    bool isSomethingPending(void)
    {
        return dataIsPending || keyUpIsPending || !empty();
    }

    /**
     * Signal that a keyUp report is pending. This means that a character has successfully been
     * sent, but the subsequent keyUp report failed. This report is of highest priority than the
     * next character.
     */
    void setKeyUpPending(void)
    {
        keyUpIsPending = true;
    }

    /**
     * Signal that no high-priority report is pending anymore, we can go back to the normal queue.
     */
    void clearKeyUpPending(void)
    {
        keyUpIsPending = false;
    }

    bool isKeyUpPending(void)
    {
        return keyUpIsPending;
    }

protected:
    bool dataIsPending;
    uint8_t pendingData;
    bool keyUpIsPending;
};

class KeyboardService : public HIDServiceBase, public Stream
{
public:
    KeyboardService(BLE &_ble) :
        HIDServiceBase(_ble,
                KEYBOARD_REPORT_MAP, sizeof(KEYBOARD_REPORT_MAP),
                inputReport         = emptyInputReportData,
                outputReport        = outputReportData,
                featureReport       = NULL,
                inputReportLength   = sizeof(inputReportData),
                outputReportLength  = sizeof(outputReportData),
                featureReportLength = 0,
                reportTickerDelay   = 24),
        failedReports(0)
    {
    }

    virtual ble_error_t send(const report_t report)
    {
        static unsigned int consecutiveFailures = 0;
        ble_error_t ret = HIDServiceBase::send(report);

        /*
         * Wait until a buffer is available (onDataSent)
         * TODO. This won't work, because BUSY error is not only returned when we're short of
         * notification buffers, but in other cases as well (e.g. when disconnected). We need to
         * find a reliable way of knowing when we actually need to wait for onDataSent to be called.
        if (ret == BLE_STACK_BUSY)
            stopReportTicker();
         */
        if (ret == BLE_STACK_BUSY)
            consecutiveFailures++;
        else
            consecutiveFailures = 0;

        if (consecutiveFailures > 20) {
            /*
             * We're not tranmitting anything anymore. Might as well avoid overloading the
             * system in case it can magically fix itself. Ticker will start again on next _putc
             * call. It could also be started on next connection, but we can't register a callback
             * for that, currently.
             */
            stopReportTicker();
            consecutiveFailures = 0;
        }

        return ret;
    }

    /**
     * Send an empty report, representing keyUp event
     */
    ble_error_t keyUpCode(void)
    {
        return send(emptyInputReportData);
    }

    ble_error_t keyDownCode(uint8_t key, uint8_t modifier)
    {
        inputReportData[0] = modifier;
        inputReportData[2] = keymap[key].usage;

        return send(inputReportData);
    }

    virtual int _putc(int c) {
        if (keyBuffer.full()) {
            return ENOMEM;
        }

        keyBuffer.push((unsigned char)c);

        if (!reportTickerIsActive)
            startReportTicker();

        return 0;
    }

    uint8_t lockStatus() {
        // TODO: implement numlock/capslock/scrolllock
        return 0;
    }



    unsigned long failedReports;

protected:
    virtual int _getc() {
        return 0;
    }

protected:
    KeyBuffer keyBuffer;

    //GattCharacteristic boot_keyboard_input_report;
    //GattCharacteristic boot_keyboard_output_report;
};

#endif /* KEYBOARD_SVC_H */
