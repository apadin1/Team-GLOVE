/* BLE scan tests */
#include "mbed.h"
#include "ble/BLE.h"

#include "keyboard_mouse.h"
#include "glove_sensors.h"
#include "scanner.h"

#define SEND(args...) pc.printf(args)
#define GETC(args...) pc.getc()


/******************** VARIABLES ********************/

//static BLE ble;
//static Serial pc(USBTX, USBRX);

static DigitalOut led1(LED1, false);
static DigitalOut led2(LED2, false);
static DigitalOut led3(LED3, false);
static DigitalOut led4(LED4, false);

static InterruptIn button11(BUTTON1);
static InterruptIn button12(BUTTON2);
static InterruptIn button13(BUTTON3);
static InterruptIn button14(BUTTON4);

/*
const static uint16_t LEFT_GLOVE  = 0xBEEF;
const static uint16_t RIGHT_GLOVE = 0xBABE;
const static uint16_t HID_CONFIG  = 0xCAFE;

#define MIN_LENGTH 4
#define MAX_LENGTH 32

static char left_key = 'a';
static char right_key = 'b';
static KeyboardMouse * keyboard_ptr;
*/

/******************** FUNCTIONS ********************/

/*
static void buttonFallCallback();
static void buttonRiseCallback();

void advertisementCallback(const Gap::AdvertisementCallbackParams_t *params) {

    led2 = !led2;
    
    // Filter advertisements by length
    if (params->advertisingDataLen < MIN_LENGTH) {
        return;
    }
    
    // Filter advertisements by ID number
    uint16_t id = params->advertisingData[3] << 8;
    id |= params->advertisingData[2];
    if (id == LEFT_GLOVE) {
        char new_key = (char) params->advertisingData[4];
        if (left_key != new_key) {
            left_key = new_key;
            SEND("left_key set to %c\r\n", new_key);
        }
    }
    else if (id == RIGHT_GLOVE) {
        char new_key = (char) params->advertisingData[4];
        if (right_key != new_key) {
            right_key = new_key;
            SEND("right_key set to %c\r\n", new_key);
        }    }
    else if (id == HID_CONFIG) {
        char config_data[MAX_LENGTH];
        int len = params->advertisingDataLen - 4;
        memcpy(config_data, (params->advertisingData + 4), len);
        config_data[len] = '\0';        
        SEND(config_data);
        SEND("\r\n");
    }
    
    // Press a key if it has changed
    char next_key_to_press = params->advertisingData[4];
    if (next_key_to_press != key_to_press) {
        key_to_press = next_key_to_press;
        buttonFallCallback();
        buttonRiseCallback();
    }
}
*/

/*
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

/*
void press(char c) {
    SEND("Pressing %c\r\n", c);
    keyboard_ptr->keyPress(c);
    keyboard_ptr->sendKeyboard();
}

void release(char c) {
    SEND("Releasing %c\r\n", c);
    keyboard_ptr->keyRelease(c);
    keyboard_ptr->sendKeyboard();
}

void leftPress(void) {
    press(left_key);
}

void leftRelease(void) {
    release(left_key);
}

void rightPress(void) {
    press(right_key);
}

void rightRelease(void) {
    release(right_key);
}


void button2Callback(void) {
    SEND("Sizeof(glove_sensors): %d\r\n", sizeof(glove_sensors_raw_t));
}
*/

int ble_scan_test(void) {
    
    //pc.baud(9600);
	//pc.attach(&uartCallback);

    /*
    SEND("I turned on!\r\n");
    
    KeyboardMouse keyboard_mouse;
    keyboard_ptr = &keyboard_mouse;
    
	//ble.init();
	ble.gap().setScanParams(500, 100);
	ble.gap().startScan(advertisementCallback);
    
    button13.fall(rightPress);
    button13.rise(rightRelease);
    button12.fall(leftPress);
    button12.rise(leftRelease);
    */
    
    int scan_period = 15;
    int scan_duration = 10;
    
    Scanner scanner;
    scanner.startScan(scan_period, scan_duration);
    
	while (true) {
        led1 = !led1;
        scanner.waitForEvent();
	}
}
