#include <inttypes.h>

#include "drivers/collector.h"
#include "drivers/ble_advert.h"
#include "drivers/dot_star_leds.h"
extern void blink(void); extern void boot_delay(uint8_t);
extern void sensors_to_lights(void);
extern void thing_do(void);
extern void keyboard_mouse_demo(void);
extern void advert_test(void);
extern void touch_to_lights(void);
extern void imu_to_lights(void);

class Blink {
public:
    Blink(AdvertBLE& _adble)
    : d(LED4), adble(_adble) {
        update_task_timer =
          new RtosTimer(this, &Blink::update, osTimerPeriodic);
        data.t = 0xff;
    }

    void update() {
        d = 0;
        wait_ms(5);
        data.t = ~data.t;
        adble.update((uint8_t*)&data, 19);
        d = 1;
    }

    void startUpdateTask() {
        update_task_timer->start(25);
    }

private:
    DigitalOut d;
    RtosTimer* update_task_timer;
    AdvertBLE& adble;
    glove_sensors_compressed_t data;
};

flex_sensor_t f[4];

bool button_leds(TouchSensor& touch_sensor, DotStarLEDs& ds_leds) {
    static key_states_t keys;
    touch_sensor.writeKeys(&keys);
    //if (keys.a == 1 || keys.b == 1 || keys.c == 1 || keys.d == 1) {
    if (f[0] < 350) {
        ds_leds.set_color(0, Yellow, 10);
        ds_leds.set_color(1, Blue, 10);
        return true;
    }
    return false;
}

void launch() {
    DigitalOut l1(LED1);
    DigitalOut l2(LED2);
    DigitalOut l3(LED3);
    DigitalOut l4(LED4);
    l1 = 1; l2 = 1; l3 = 1; l4 = 1;

    DotStarLEDs ds_leds(2);
    ds_leds.set_color_all(White);

    // used by glove sensors compressGloveData
    crcInit();

    I2C i2c(I2C_SDA0, I2C_SCL0); // Initialize i2c bus for imu and touch_sensor
    IMU_BNO055 imu(i2c);
    TouchSensor touch_sensor(i2c, TOUCH_INTERRUPT);
    FlexSensors flex_sensors;

    // This encapsulates the BLE stack
    AdvertBLE adble;

    Collector collector(&flex_sensors, &imu, &touch_sensor, adble);
    collector.startUpdateTask(20);

    //Blink blk(adble); blk.startUpdateTask();

    /* The following main loop sets the light pattern on the
     * gloves after all the sensors have been initialized,
     * the BLE advertising set up, and the collector is running
     */
    DigitalOut dso(p13); dso = 0;
    for (;;) {
        l1 = !l1;
        //printf("f: %d, t: 0x%x\r\n", collector.flex_data[0], collector.touch_data->pack());
        //printf("comp: %d, fp: %f\r\n", compress_double(110.565), extract_double(11056));

        int val = sizeof(glove_sensors_compressed_t);
        for (int i=0; i<val; ++i) {dso=1;dso=0;}

        for (int i=0; i < 50; ++i) {
            flex_sensors.writeSensors(f);
            if (!button_leds(touch_sensor, ds_leds)) {
                ds_leds.set_color(0, Red);
                ds_leds.set_color(1, Magenta);
            }
            Thread::wait(10);
        }

        for (int i=0; i < 50; ++i) {
            flex_sensors.writeSensors(f);
            if (!button_leds(touch_sensor, ds_leds)) {
                ds_leds.set_color(0, Green);
                ds_leds.set_color(1, Yellow);
            }
            Thread::wait(10);
        }

        for (int i=0; i < 50; ++i) {
            flex_sensors.writeSensors(f);
            if (!button_leds(touch_sensor, ds_leds)) {
                ds_leds.set_color(0, Blue);
                ds_leds.set_color(1, Cyan);
            }
            Thread::wait(10);
        }
    }
}

int main() {

    /*
     * The current setup with main.cpp is to have single calls to
     * test functions so we can each have test code without having
     * to comment out/have multiple versions.
     * Just change your local one to call the test loop you need.
     */
    //sensors_to_lights();
    launch();
    //touch_to_lights();
    //blink();
    //launch_periodic();
    //keyboard_mouse_demo();
    //launch();
    //imu_to_lights();
    //touch_to_lights();
    //advert_test();
}
