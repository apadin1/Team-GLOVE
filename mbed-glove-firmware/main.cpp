#include <inttypes.h>

#include "drivers/collector.h"
#include "drivers/ble_advert.h"

extern void blink(void);
extern void boot_delay(uint8_t);
extern void sensors_to_lights(void);
extern void thing_do(void);
extern void keyboard_mouse_demo(void);

class Blink {
public:
    Blink()
    : d(p14) {
    update_task_timer =
          new RtosTimer(this, &Blink::update, osTimerPeriodic);
    }

    void update() {
        d = 1;
        wait_ms(5);
        d = 0;
    }

    void startUpdateTask() {
        update_task_timer->start(30);
    }

private:
    DigitalOut d;
    RtosTimer* update_task_timer;
};

void launch() {
    DigitalOut l1(LED1);
    DigitalOut l2(LED2);
    DigitalOut l3(LED3);
    DigitalOut l4(LED4);
    l1 = 0;
    l2 = 1;
    l3 = 1;
    l4 = 1;

    I2C i2c(I2C_SDA0, I2C_SCL0); // Initialize i2c bus for imu and touch_sensor
    IMU_BNO055 imu(i2c);
    TouchSensor touch_sensor(i2c, TOUCH_INTERRUPT);
    FlexSensors flex_sensors;

    // This encapsulates the BLE stack
    AdvertBLE adble(50);

    Collector collector(&flex_sensors, &imu, &touch_sensor, adble);
    collector.startUpdateTask(30);

    //Blink blk; blk.startUpdateTask();

    //glove_sensors_raw_t glove_data;

    for (;;) {
        l2 = 0;
        adble.waitForEvent();
        l2 = 1;
    }

    DigitalOut d1(p12);
    for (;;) {
        d1 = 1;
        /*
        touch_sensor.spawnUpdateThread();
        imu.updateAndWrite(&glove_data.imu);
        flex_sensors.updateAndWrite(&glove_data.flex_sensors[0]);
        touch_sensor.writeKeys(&glove_data.touch_sensor);
        wait_ms(5);
        touch_sensor.terminateUpdateThreadIfBlocking();
        */
        //wait_ms(5);
        d1 = 0;
        Thread::wait(40);
    }
    Thread::wait(osWaitForever);
}

int main() {

    /*
     * The current setup with main.cpp is to have single calls to
     * test functions so we can each have test code without having
     * to comment out/have multiple versions.
     * Just change your local one to call the test loop you need.
     */
    //sensors_to_lights();
    //blink();
    //launch_periodic();
    //keyboard_mouse_demo();
    launch();
}
