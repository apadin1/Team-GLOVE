/*
 * Copyright (c) 2016 by Nick Bertoldi, Ben Heckathorn, Ryan O'Keefe,
 *                       Adrian Padin, Timothy Schumacher
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef KEYBOARD_MOUSE_H
#define KEYBOARD_MOUSE_H

#include "mbed.h"
#include "ble/BLE.h"
#include "KeyboardMouseService.h"


/* Wrapper class for Keyboard Mouse BLE Service */
class KeyboardMouse {

public:

    /* Constructor and Destructor*/
    KeyboardMouse(BLE& _ble);
    ~KeyboardMouse();

    /******************** MOUSE INTERFACE ********************/

    /* Set a button to be pressed or released
     * Parameters:
     *   button - button selection (can be LEFT, RIGHT, or MIDDLE)
     *   state - button status (can be UP or DOWN)
     */
    void setMouseButton(MouseButton button, ButtonState state);

    /* Set the speed of the mouse cursor in the x direction
     * Parameters:
     *   speed - speed of the cursor (range -127 to 127)
     */
    void setMouseSpeedX(int8_t speed);

    /* Set the speed of the mouse cursor in the y direction
     * Parameters:
     *   speed - speed of the cursor (range -127 to 127)
     */
    void setMouseSpeedY(int8_t speed);

    /* Set the scroll value of the mouse scroll wheel
     * Parameters:
     *   speed - speed of the scroll wheel (range -127 to 127)
     */
    void setMouseScroll(int8_t speed);

    /* Set the x, y, and scroll speed of the mouse
     * Parameters:
     *   x - speed of the cursor in x (range -127 to 127)
     *   y - speed of the cursor in y (range -127 to 127)
     *   scroll - speed of the scroll wheel (range -127 to 127)
     */
    void setMouseSpeedAll(int8_t x, int8_t y, int8_t scroll);

    /******************** KEYBOARD INTERFACE ********************/

    /* Set a keyboard button to be 'pressed'
     * Parameters:
     *
     *   key - the key which is being pressed. This can either be
     *      an ASCII character (must be one which is represented
     *      on a standard keyboard) or a function key, such as
     *      RIGHT_ARROW or KEY_HOME.
     *
     *   modifier - a command which modifies the key being pressed.
     *      Modifiers which are currently supported are:
     *      - KEY_CTRL
     *      - KEY_SHIFT
     *      - KEY_ALT
     *
     *      See keyboard_types.h for more information on function
     *      keys and modifiers.
     */
    void keyPress(uint8_t key, uint8_t modifier=0);

    /* Set the keyboard to be all buttons released */
    void keyRelease(uint8_t key);


    /******************** BLE INTERFACE ********************/

    /* Check if the device is paired to a computer */
    bool isConnected() { return service_ptr->isConnected(); }

    /* Wait to be interrupted */
    void waitForEvent() { ble.waitForEvent(); }

    /* Send the keyboard and mouse reports */
    void sendKeyboard() { service_ptr->sendKeyboardReport(); }
    void sendMouse() { service_ptr->sendMouseReport(); }


private:

    /******************** PRIVATE VARIABLES ********************/
    KeyboardMouseService * service_ptr;
    BLE& ble;

    uint8_t keyboard_keys[KBD_USAGE_LENGTH];
    int len; /* Current number of keys pressed */

};

#endif /* KEYBOARD_MOUSE_H */
