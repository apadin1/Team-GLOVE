#include "mbed.h"
#include "rtos.h"

#include "drivers/flex_sensor.h"
#include "drivers/imu.h"
#include "drivers/touch_sensor.h"
#include "drivers/collector.h"

#include "drivers/analog_button.h"
#include "drivers/dot_star_leds.h"

#define INCLUDE_TOUCH 1

const PinName GLOVE_I2C_SDA = p30; //I2C_SDA0; // p30
const PinName GLOVE_I2C_SCL = p7; //I2C_SCL0; // p7

I2C i2c(GLOVE_I2C_SDA, GLOVE_I2C_SCL);

Serial pc(USBTX, USBRX);

DigitalOut led(LED1);

void blink() {
    for (;;) {
        led = !led;
        Thread::wait(250);
    }
}

void imu_test() {

    IMU_BNO055 imu(i2c);

    imu.startUpdateTask(500);

    for (;;) {
        imu.print(pc);

        led = !led;
        Thread::wait(1000);
    }
}

void touch_sensor_test() {
    key_states_t keys;
    key_states_t last_keys;

    TouchSensor touch_sensor(i2c);
    Thread touch_sensor_thread;
    touch_sensor_thread.start(&touch_sensor, &TouchSensor::updateTask);

    for (;;) {

        last_keys = keys;
        touch_sensor.writeKeys(&keys);

        if (last_keys.pack() != keys.pack()) {
            TouchSensor::print(pc, keys);
        }
        led = !led;
        Thread::wait(200);
    }
}

void flex_test() {

    FlexSensors flex_sensors;
    flex_sensors.startUpdateTask(200);

    for (;;) {

        //flex_sensors.update();
        flex_sensors.printSingle(pc, 0);


        led = !led;
        Thread::wait(1000);
    }
}

void boot_delay(uint8_t t) {
    // this loop is to prevent the strange fatal state
    led = 1;
    DigitalOut l2(LED2); l2 = 1;
    DigitalOut l3(LED3); l3 = 1;
    DigitalOut l4(LED4); l4 = 1;
    for (uint8_t i = 0; i < t; ++i) {
        led = 0;
        l2 = 0;
        l3 = 0;
        l4 = 0;
        wait(0.25);
        led = 1;
        l2 = 1;
        l3 = 1;
        l4 = 1;
        wait(0.75);
    }
}

float convert_analog_percent(uint16_t min_, uint16_t max_, uint16_t val) {
    return (val - min_) / float(max_ - min_);
}

float map_float(float min_, float max_, float val) {
    return (val - min_) / float(max_ - min_);
}

void analog_to_rgb(float analog, uint8_t& red, uint8_t& green, uint8_t& blue) {
    red = 254*analog;
    green = green;
    blue = 254*(1-analog);
}

void flex_to_lights() {

    DotStarLEDs ds_leds(2);
    FlexSensors flex_sensors;
    flex_sensor_t flex_vals[4];

    uint8_t red, green, blue;
    float analog_val;

    uint16_t flex_min = 500;
    uint16_t flex_max = 700;

    IMU_BNO055 imu(i2c);
    bno_imu_t imu_vals;


    while (true) {

        flex_sensors.updateAndWriteSensors(flex_vals);
        imu.update();

        imu.writeSensors(&imu_vals);
        green = 255*map_float(-90.0, 90.0, imu_vals.orient_roll);

        for (uint8_t i = 0; i < 2; i++) {
            if (flex_vals[i] < flex_min) {
                flex_min = flex_vals[i];
            }
            else if (flex_vals[i] > flex_max) {
                flex_max = flex_vals[i];
            }

            analog_val = convert_analog_percent(flex_min, flex_max, flex_vals[i]);
            analog_to_rgb(analog_val, red, green, blue);
            ds_leds.set_RGB(i, red, green, blue);
        }

        Thread::wait(50);
    }
}

void launch_periodic() {
    /*
    TouchSensor touch_sensor;
    Thread touch_sensor_thread;
    touch_sensor_thread.start(&touch_sensor, &TouchSensor::updateTask);
    key_states_t keys;
    key_states_t last_keys;
    */

    FlexSensors flex_sensors;
    IMU_BNO055 imu(i2c);

    flex_sensors.startUpdateTask(10);
    wait_ms(2); // to offset the timers
    imu.startUpdateTask(10);

    //Collector collector(&flex_sensors, &imu, &touch_sensor, &pc);
    //collector.startUpdateTask(1); // 1 sec period for serial out

    boot_delay(5);
    uint8_t print_limit = 0;
    for (;;) {
        led = !led; // just so we know its running

        // Flex
        //flex_sensors.printSingle(pc, 0);
        flex_sensors.print(pc);

        // Touch
        /*
        last_keys = keys;
        touch_sensor.writeKeys(&keys);
        if (last_keys.pack() != keys.pack()) {
            TouchSensor::print(pc, keys);
        }
        */

        if (print_limit++ == 2) {
            imu.print(pc);
            print_limit = 0;
        }

        Thread::wait(1000);
    }
}

int main() {

    /*
     * The current setup with main.cpp is to have single calls to
     * test functions so we can each have test code without having
     * to comment out/have multiple versions.
     * Just change your local one to call the test loop you need.
     */
    flex_to_lights();

    //launch_periodic();

}
