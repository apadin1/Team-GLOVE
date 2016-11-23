#include "mbed.h"
#include "string"
#include "ble/BLE.h"
#include "ble/DiscoveredCharacteristic.h"
#include "ble/DiscoveredService.h"

#define DEBUG false
#define WRISTBANDS 3

#define LOG(args...) if (DEBUG) pc.printf(args)
#define SEND(args...) pc.printf(args)

static BLE ble;
static DigitalOut led(LED1, 1);
static Serial pc(USBTX, USBRX);

static bool lookup_table[WRISTBANDS + 1];
static bool foundWristbandCharacteristic = false;
static uint8_t connectedWristbandShortUUID = 0;
static int uartWristbandCommand = 0;
static int uartWristbandShortUUID = 0;

static char receive_buffer[256] = {0};
static uint8_t transmit_buffer = 0;

static Gap::Handle_t connectionHandle = 0xFFFF;
static DiscoveredCharacteristic wristbandCharacteristic;

void advertisementCallback(const Gap::AdvertisementCallbackParams_t *params) {
	if (params->advertisingData[5] != 0x12 || params->advertisingData[6] != 0x34) {
		return;
	}

	LOG("Wristband Advertisement\r\n");
	LOG("-----------------------\r\n");
	for (uint8_t i = 0; i < params->advertisingDataLen; ++i) {
		LOG("%02x ", params->advertisingData[i]);
	}
	LOG("%d\r\n", lookup_table[params->advertisingData[24]]);
	LOG("\r\n\r\n");

	SEND("%d,%d\n", params->advertisingData[24], params->rssi);

	if (lookup_table[params->advertisingData[24]]) {
		connectedWristbandShortUUID = params->advertisingData[24];
		ble.gap().connect(
			params->peerAddr,
			Gap::ADDR_TYPE_RANDOM_STATIC,
			NULL,
			NULL);
	}
}

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

int ble_scan_test(void) {
	pc.baud(9600);
	pc.attach(&uartCallback);

	ble.init();
	ble.gap().onConnection(connectionCallback);
	ble.gap().onDisconnection(disconnectionCallback);

	ble.gap().setScanParams(500, 400);
	ble.gap().startScan(advertisementCallback);

	ble.gattClient().onHVX(hvxCallback);

	while (true) {
		if (foundWristbandCharacteristic && !ble.gattClient().isServiceDiscoveryActive()) {
			foundWristbandCharacteristic = false;

			uint16_t value = BLE_HVX_NOTIFICATION;
			ble.gattClient().write(
				GattClient::GATT_OP_WRITE_REQ,
				connectionHandle,
				wristbandCharacteristic.getValueHandle() + 1,
				sizeof(uint16_t),
				reinterpret_cast<const uint8_t *>(&value));
		}
		ble.waitForEvent();
	}
}
