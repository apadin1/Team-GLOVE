#include "mbed.h"
#include "string"
#include "ble/BLE.h"
#include "ble/DiscoveredCharacteristic.h"
#include "ble/DiscoveredService.h"

#include "keyboard_mouse.h"

#define DEBUG true
//#define WRISTBANDS 3

#define LOG(args...) if (DEBUG) pc.printf(args)
#define SEND(args...) pc.printf(args)

static BLE ble;
static DigitalOut led(LED1, 1);
static DigitalOut led2(LED2, 1);
static InterruptIn button(BUTTON1);
static Serial pc(USBTX, USBRX);

void buttonFallCallback();
void buttonRiseCallback();

/*
static bool lookup_table[WRISTBANDS + 1];
static bool foundWristbandCharacteristic = false;
static uint8_t connectedWristbandShortUUID = 0;
static int uartWristbandCommand = 0;
static int uartWristbandShortUUID = 0;

static char receive_buffer[256] = {0};
static uint8_t transmit_buffer = 0;

static Gap::Handle_t connectionHandle = 0xFFFF;
static DiscoveredCharacteristic wristbandCharacteristic;
*/

const static uint8_t GLOVE_ADDR[2] = {0xEF, 0xBE};
const static int MIN_DATA = 4;
static char key_to_press = 'B';
static KeyboardMouse * keyboard_ptr;

void advertisementCallback(const Gap::AdvertisementCallbackParams_t *params) {

    led2 = !led2;
    
    // Filter advertisements by length
    if (params->advertisingDataLen < MIN_DATA) {
        return;
    }
    
    // Filter advertisements by address
    if (params->advertisingData[2] != GLOVE_ADDR[0] ||
        params->advertisingData[3] != GLOVE_ADDR[1]) {
        return;
    }

    // Press a key if it has changed
    char next_key_to_press = params->advertisingData[4];
    if (next_key_to_press != key_to_press) {
        key_to_press = next_key_to_press;
        buttonFallCallback();
        buttonRiseCallback();
    }
    
    
    
    // Print data
    /*
    if (params->advertisingDataLen == 22) {
        LOG("peerAddr: %02x %02x %02x %02x %02x %02x\r\n", 
            params->peerAddr[0],
            params->peerAddr[1],
            params->peerAddr[2],
            params->peerAddr[3],
            params->peerAddr[4],
            params->peerAddr[5]);
        LOG("rssi: %d\r\n", params->rssi);
        LOG("isScanResponse: %d\r\n", params->isScanResponse);
        LOG("type: %d\r\n", params->type);
        LOG("length: %d\r\n", params->advertisingDataLen);
        for (int i = 0; i < params->advertisingDataLen; ++i) {
            LOG("[%d]: %d\r\n", i, params->advertisingData[i]);
            
        }
        wait(10);
    }
    */
}

/*
void serviceDiscoveryCallback(const DiscoveredService *service) {
#ifdef DEBUG
	LOG("serviceDiscoveryCallback\r\n");
	LOG("------------------------\r\n");

	if (service->getUUID().shortOrLong() == UUID::UUID_TYPE_SHORT) {
		LOG("S UUID-%x ", service->getUUID().getShortUUID());
		LOG("Attributes[%u %u]\r\n", service->getStartHandle(), service->getEndHandle());
	}
	else {
		LOG("S UUID-");

		const uint8_t *longUUID = service->getUUID().getBaseUUID();
		for (unsigned i = 0; i < UUID::LENGTH_OF_LONG_UUID; i++) {
			LOG("%02x ", longUUID[i]);
		}

		LOG(" Attributes[%u %u]\r\n", service->getStartHandle(), service->getEndHandle());
	}
	LOG("\r\n");
#endif
}

void characteristicDiscoveryCallback(const DiscoveredCharacteristic *characteristic) {
	LOG("characteristicDiscoveryCallback\r\n");
	LOG("-------------------------------\r\n");

	LOG("  C UUID-%x ", characteristic->getUUID().getShortUUID());
	LOG("Value Attribute[%u] ", characteristic->getValueHandle());
	LOG("Properties[%x]\r\n\r\n", (uint8_t) characteristic->getProperties().broadcast());

	if (characteristic->getUUID().getShortUUID() == 0xA001) {
		wristbandCharacteristic = *characteristic;
		foundWristbandCharacteristic = true;
	}
}

void discoveryTerminationCallback(Gap::Handle_t connectionHandle) {
	LOG("discoveryTerminationCallback: %u\r\n", connectionHandle);
	LOG("--------------------------------\r\n\r\n");

	if (foundWristbandCharacteristic) {
		ble_error_t error = ble.gattClient().write(
			GattClient::GATT_OP_WRITE_CMD,
			connectionHandle,
			wristbandCharacteristic.getValueHandle(),
			sizeof(uint8_t),
			(uint8_t *) &uartWristbandCommand);
		if (error != BLE_ERROR_NONE) {
			LOG("ERROR: %d\r\n\r\n", error);
		}
		else {
			lookup_table[connectedWristbandShortUUID] = false;
			LOG("Write Successfull!\r\n\r\n");
		}
	}

	ble.gap().disconnect(connectionHandle, Gap::REMOTE_USER_TERMINATED_CONNECTION);
}

void connectionCallback(const Gap::ConnectionCallbackParams_t *params) {
	LOG("connectionCallback\r\n");
	LOG("------------------\r\n\r\n");

	if (params->role == Gap::CENTRAL) {
		connectionHandle = params->handle;
		ble.gattClient().onServiceDiscoveryTermination(discoveryTerminationCallback);
		ble.gattClient().launchServiceDiscovery(
			params->handle,
			serviceDiscoveryCallback,
			characteristicDiscoveryCallback);
	}
}

void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *params) {
	LOG("Disconnected\r\n\r\n");
	ble.gap().startScan(advertisementCallback);
}

void hvxCallback(const GattHVXCallbackParams *params) {
	LOG("Received HVX Callback for Handle %u; ", params->handle);
	LOG("Type: %s\r\n", (params->type == BLE_HVX_NOTIFICATION) ? "Notification" : "Indication");

	for (unsigned index = 0; index < params->len; index++) {
		LOG(" %02x", params->data[index]);
	}
	LOG("\r\n\r\n");
}

void uartCallback() {
	uint8_t request = pc.getc() - '0';
	uartWristbandShortUUID = request;
	if (uartWristbandShortUUID > WRISTBANDS) {
		return;
	}
	else {
		lookup_table[uartWristbandShortUUID] = true;
	}
}
*/

void buttonFallCallback(void) {
    SEND("Pressing %c\r\n", key_to_press);
    keyboard_ptr->keyPress(key_to_press);
    keyboard_ptr->sendKeyboard();
}

void buttonRiseCallback(void) {
    SEND("Releasing %c\r\n", key_to_press);
    keyboard_ptr->keyRelease(key_to_press);
    keyboard_ptr->sendKeyboard();
}

int ble_scan_test(void) {
	pc.baud(9600);
	//pc.attach(&uartCallback);

    SEND("I turned on!\r\n");
    
    KeyboardMouse keyboard_mouse(ble);
    keyboard_ptr = &keyboard_mouse;
    
	//ble.init();
	ble.gap().setScanParams(500, 100);
	ble.gap().startScan(advertisementCallback);
    
    button.fall(buttonFallCallback);
    button.rise(buttonRiseCallback);
    
	while (true) {
        led = !led;
        ble.waitForEvent();
	}
}
