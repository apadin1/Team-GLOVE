#include "mbed.h"
#include "ble/BLE.h"

static BLE  ble;

static const uint8_t AdvData[] = {0xEE, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11, 0x12, 0x13, 0x14,
                                              0x15, 0x16, 0x17, 0x18, 0x19, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28};

static DigitalOut led1(LED1);
static DigitalOut led2(LED2);
static DigitalOut led3(LED3);
static InterruptIn button1(BUTTON1);
static InterruptIn button2(BUTTON2);
static InterruptIn button3(BUTTON3);
static GapAdvertisingData adv;

void stopAdvertisement()
{
    led1 = false;
    ble.stopAdvertising();
    led1 = true;
}

void advertisement1()
{
    led2 = false;
    uint8_t newAdv[] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
    adv.updateData(GapAdvertisingData::MANUFACTURER_SPECIFIC_DATA, newAdv, sizeof(newAdv));
    ble.setAdvertisingData(adv);
    ble.startAdvertising();
    led2 = true;
}

void advertisement2()
{
    led3 = false;
    adv.updateData(GapAdvertisingData::MANUFACTURER_SPECIFIC_DATA, AdvData, sizeof(AdvData));
    ble.setAdvertisingData(adv);
    ble.startAdvertising();
    led3 = true;
}

void advert_test(void)
{
    led1 = true;
    led2 = true;
    led3 = true;
    ble.init();
    button1.rise(stopAdvertisement);
    button2.rise(advertisement1);
    button3.rise(advertisement2);

    //need to have some data in order to update in callbacks
    uint8_t def[] = {0x0};
    adv = GapAdvertisingData();
    adv.addData(GapAdvertisingData::MANUFACTURER_SPECIFIC_DATA, def, sizeof(def));

    ble.setAdvertisingInterval(160);

    while(1)
    {
        ble.waitForEvent();
    }
}
