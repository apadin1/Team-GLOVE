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

static const char DEVICE_NAME[] = "TeamGLOVE";
static const char SHORT_NAME[] = "glove1"; 

/* Wrapper class for Keyboard Mouse BLE Service */
class KeyboardMouse {

public:

    /* Constructor and destructor */
    KeyboardMouse();
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
    void setScrollSpeed(int8_t speed);
    
    
    /******************** KEYBOARD INTERFACE ********************/
    
    /* Send a given character to the keyboard
     * Parameters:
     *   c - character to send
     */
    void sendChar(char c);
    
    /* Set a keyboard button to be 'pressed' */
    void keyPress()
    
    /* Set the keyboard to be all buttons released */
    void keyRelease();
    
    
    /******************** BLE INTERFACE ********************/    

    /* Check if the device is paired to a computer */
    void isConnected() { return connected; }
    
    /* Wait to be interrupted */
    void waitForEvent() { ble.gap().waitForEvent(); }
    

private:

    /******************** HIDDEN VARIABLES ********************/
    KeyboardMouseService hid_service;
    BLE ble;
    bool connected;
};

