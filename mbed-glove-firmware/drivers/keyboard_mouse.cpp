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


/******************** HELPER FUNCTIONS ********************/

/* Most of these functions are for initializing the GAP protocol
 * and doing the security handshake. I have no idea how any of
 * this works, I literally just copied it from the mbed example
 * and I know that we need it so here it is.
 */

static void passkeyDisplayCallback(
    Gap::Handle_t handle,
    const SecurityManager::Passkey_t passkey) { return; }

static void securitySetupCompletedCallback(
    Gap::Handle_t handle,
    SecurityManager::SecurityCompletionStatus_t status) { return; }

static void securitySetupInitiatedCallback(
    Gap::Handle_t,
    bool allowBonding,
    bool requireMITM,
    SecurityManager::SecurityIOCapabilities_t iocaps) { return; }

void initializeSecurity(BLE &ble) {
    bool enableBonding = true;
    bool requireMITM = HID_SECURITY_REQUIRE_MITM;
    ble.securityManager().onSecuritySetupInitiated(securitySetupInitiatedCallback);
    ble.securityManager().onPasskeyDisplay(passkeyDisplayCallback);
    ble.securityManager().onSecuritySetupCompleted(securitySetupCompletedCallback);
    ble.securityManager().init(enableBonding, requireMITM, HID_SECURITY_IOCAPS);
}

void initializeHOGP(BLE &ble)
{
    static const uint16_t uuid16_list[] =  {GattService::UUID_HUMAN_INTERFACE_DEVICE_SERVICE,
        GattService::UUID_DEVICE_INFORMATION_SERVICE,
        GattService::UUID_BATTERY_SERVICE};

    DeviceInformationService deviceInfo(ble, "ARM", "m1", "abc", "def", "ghi", "jkl");

    BatteryService batteryInfo(ble, 80);

    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED |
            GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS,
            (uint8_t *)uuid16_list, sizeof(uuid16_list));

    // see 5.1.2: HID over GATT Specification (pg. 25)
    ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    // 30ms to 50ms is recommended (5.1.2)
    ble.gap().setAdvertisingInterval(50);
}


/******************** CALLBACKS ********************/
static void connectedCallback(KeyboardMouseService * new_ptr, bool connected) {
    static KeyboardMouseService * my_ptr;
    if (new_ptr != NULL) {
        my_ptr = new_ptr;
    }
    else {
        my_ptr->setConnected(connected);
    }
}

static void onConnect(const Gap::ConnectionCallbackParams_t* params=NULL) {
    connectedCallback(NULL, true);
}
static void onDisconnect(const Gap::DisconnectionCallbackParams_t* params=NULL) {
    connectedCallback(NULL, false);
}

/* Wrapper class for Keyboard Mouse BLE Service */
KeyboardMouse::KeyboardMouse() {

    /* Prepare to connect and set callbacks */
    printf("init ble\r\n");
    ble.init();
    ble.gap().onConnection((Gap::ConnectionEventCallback_t) onConnect);
    ble.gap().onDisconnection((Gap::DisconnectionEventCallback_t) onDisconnect);

    /* Security is required to pair */
    printf("init security\r\n");
    initializeSecurity(ble);

    /* Initialize service pointer and connection callbacks */
    service_ptr = new KeyboardMouseService(ble);
    connectedCallback(service_ptr, true);

    /* Initialize GAP transmission */
    printf("init gap\r\n");
    initializeHOGP(ble);

    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::KEYBOARD);

    ble.gap().accumulateAdvertisingPayload(
        GapAdvertisingData::COMPLETE_LOCAL_NAME,
        (const uint8_t *) DEVICE_NAME,
        sizeof(DEVICE_NAME));

    ble.gap().accumulateAdvertisingPayload(
        GapAdvertisingData::SHORTENED_LOCAL_NAME,
        (const uint8_t *) SHORT_NAME,
        sizeof(SHORT_NAME));

    ble.gap().setDeviceName((const uint8_t *) DEVICE_NAME);

    /* Search for nearby devices to comminucate with */
    printf("start advertising\r\n");
    ble.gap().startAdvertising();
}

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

/* Send a character to the keyboard */
void KeyboardMouse::sendChar(char c) {
    service_ptr->sendChar(c);
}

/* Set a keyboard button to be 'pressed' */
void KeyboardMouse::keyPress(uint8_t key, uint8_t modifier) {
    service_ptr->keyPress(key, modifier);
}


/* Set the keyboard to be all buttons released */
void KeyboardMouse::keyRelease() {
    service_ptr->keyRelease();
}
