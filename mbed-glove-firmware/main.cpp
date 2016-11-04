#include "mbed.h"
#include "rtos.h"

#include "drivers/flex_sensor.h"
#include "drivers/imu.h"
#include "drivers/touch_sensor.h"
#include "drivers/collector.h"

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

void launch_periodic() {
    TouchSensor touch_sensor;
    Thread touch_sensor_thread;
    touch_sensor_thread.start(&touch_sensor, &TouchSensor::updateTask);
    key_states_t keys;
    key_states_t last_keys;

    FlexSensors flex_sensors;
    IMU_BNO055 imu(i2c);

    flex_sensors.startUpdateTask(10);
    wait_ms(2); // to offset the timers
    imu.startUpdateTask(10);

    //Collector collector(&flex_sensors, &imu, &touch_sensor, &pc);
    //collector.startUpdateTask(1); // 1 sec period for serial out

    uint8_t print_limit = 0;
    for (;;) {
        led = !led; // just so we know its running

        // Flex
        flex_sensors.printSingle(pc, 0);

        // Touch
        last_keys = keys;
        touch_sensor.writeKeys(&keys);

        if (last_keys.pack() != keys.pack()) {
            TouchSensor::print(pc, keys);
        }

        if (print_limit++ == 5) {
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
    launch_periodic();

    blink();
}
