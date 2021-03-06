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
#include "ble/services/BatteryService.h"
#include "ble/services/DeviceInformationService.h"

/* IO capabilities of the device. */
#ifndef HID_SECURITY_IOCAPS
#define HID_SECURITY_IOCAPS (SecurityManager::IO_CAPS_NONE)
#endif

/* Security level. */
#ifndef HID_SECURITY_REQUIRE_MITM
#define HID_SECURITY_REQUIRE_MITM false
#endif

DigitalOut l1(LED1);

static const char DEVICE_NAME[] = "EliteMotion";//"Team GLOVE";
static const char SHORT_NAME[] = "glove";


/******************** CALLBACKS ********************/

/* Store the value of the service pointer for the callbacks to use */
static KeyboardMouseService * getServicePtr(KeyboardMouseService * new_ptr) {
    static KeyboardMouseService * service_ptr;
    if (new_ptr != NULL) {
        service_ptr = new_ptr;
    }
    return service_ptr;
}

/* When the device gets connected */
static void connectionCallback(const Gap::ConnectionCallbackParams_t *params) {
    KeyboardMouseService * service_ptr = getServicePtr(NULL);
    service_ptr->setConnected(true);
}

/* When the device gets disconnected */
static void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *params) {
    KeyboardMouseService * service_ptr = getServicePtr(NULL);
    service_ptr->setConnected(false);
    BLE::Instance(BLE::DEFAULT_INSTANCE).gap().startAdvertising(); // restart advertising
}

/******************** HELPER FUNCTIONS ********************/

/* Most of these functions are for initializing the GAP protocol
 * and doing the security handshake. I have no idea how any of
 * this works, I literally just copied it from the mbed example
 * and I know that we need it so here it is.
 */

void initializeSecurity(BLE &ble) {
    bool enableBonding = true;
    bool requireMITM = false;
    ble.securityManager().init(enableBonding, requireMITM, SecurityManager::IO_CAPS_NONE);
}

/* Initialize BLE */
void bleInitComplete(BLE& ble) {

    // Retrieve the parameters and check for errors
    //BLE &ble          = params->ble;
    //ble_error_t error = params->error;
    //if (error != BLE_ERROR_NONE) return;

    // Security is required to pair
    initializeSecurity(ble);

    // Initialize callbacks
    ble.gap().onConnection(connectionCallback);
    ble.gap().onDisconnection(disconnectionCallback);

    // Setup device services
    KeyboardMouseService * hid_service = new KeyboardMouseService(ble);
    BatteryService * battery_service = new BatteryService(ble, 80);
    DeviceInformationService * device_info_service =
        new DeviceInformationService(ble, "ARM", "Model1", "SN1", "hw-rev1", "fw-rev1", "soft-rev1");

    // Back door to let the KeyboardMouse class access the hid service
    getServicePtr(hid_service);

    // Continue building advertising payload
    static const uint16_t uuid16_list[] =  {
        GattService::UUID_HUMAN_INTERFACE_DEVICE_SERVICE,
        GattService::UUID_DEVICE_INFORMATION_SERVICE,
        GattService::UUID_BATTERY_SERVICE
    };

    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);

    // This is how the device will initially be known to the computer
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::KEYBOARD);

    // Add the device short and full name
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (const uint8_t *) DEVICE_NAME, sizeof(DEVICE_NAME));
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::SHORTENED_LOCAL_NAME, (const uint8_t *) SHORT_NAME, sizeof(SHORT_NAME));
    ble.gap().setDeviceName((const uint8_t *) DEVICE_NAME);

    // see 5.1.2: HID over GATT Specification (pg. 25)
    ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    ble.gap().setAdvertisingInterval(50);     // 30ms to 50ms is recommended (5.1.2)
}


/******************** CONSTRUCTOR/DESTRUCTOR ********************/

/* Constructor */
KeyboardMouse::KeyboardMouse(BLE& _ble) :
        ble(_ble),
        len(0) {

    /* Initialize keyboard variables */
    memset(keyboard_keys, 0, KBD_USAGE_LENGTH);

    /* Initialize the BLE communication scheme */
    bleInitComplete(ble);

    /* Initialize HID service pointer */
    service_ptr = getServicePtr(NULL);

    /* Start GAP transmission */
    ble.gap().startAdvertising();
}

/* Destructor */
KeyboardMouse::~KeyboardMouse() {
    delete service_ptr;
}


/******************** MOUSE INTERFACE ********************/

/* Set a button to be pressed or released */
void KeyboardMouse::setMouseButton(MouseButton button, ButtonState state) {
    service_ptr->setMouseButton(button, state);
}

/* Set the speed of the mouse cursor in the x direction */
void KeyboardMouse::setMouseSpeedX(int8_t speed) {
    service_ptr->setMouseSpeedX(speed);
}

/* Set the speed of the mouse cursor in the y direction */
void KeyboardMouse::setMouseSpeedY(int8_t speed) {
    service_ptr->setMouseSpeedY(speed);
}

/* Set the scroll value of the mouse scroll wheel */
void KeyboardMouse::setMouseScroll(int8_t speed) {
    service_ptr->setMouseScroll(speed);
}

void KeyboardMouse::setMouseSpeedAll(int8_t x, int8_t y, int8_t scroll) {
    service_ptr->setMouseSpeedAll(x, y, scroll);
}


/******************** KEYBOARD INTERFACE ********************/

/* Set a keyboard button to be 'pressed' */
void KeyboardMouse::keyPress(uint8_t key, uint8_t modifier) {

    /* If the array is full, there is nothing to do */
    if (len == KBD_USAGE_LENGTH) return;

    /* Make sure the key is not already pressed */
    for (int i = 0; i < len; ++i) {
        if (keyboard_keys[i] == key) return;
    }

    /* Not already pressed - add it to the list */
    keyboard_keys[len] = key;
    len += 1;
    service_ptr->keyPress(keyboard_keys, len, modifier);
}

/* Set the keyboard to be all buttons released */
void KeyboardMouse::keyRelease(uint8_t key) {

    /* Check if it is actually there */
    for (int i = 0; i < len; ++i) {

        /* If found, zero out and shift others down */
        if (keyboard_keys[i] == key) {
            len -= 1;
            for (int j = i; j < len; ++j) {
                keyboard_keys[j] = keyboard_keys[j+1];
            }
            service_ptr->keyPress(keyboard_keys, len);
            return;
        }
    }
    return; // If not found do nothing
}
