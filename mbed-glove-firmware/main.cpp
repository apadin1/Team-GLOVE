#include "mbed.h"
#include "rtos.h"

#include "drivers/flex_sensor.h"
#include "drivers/imu.h"
#include "drivers/touch_sensor.h"
#include "drivers/collector.h"

const PinName GLOVE_I2C_SDA = I2C_SDA0; // p30
const PinName GLOVE_I2C_SCL = I2C_SCL0; // p7
I2C i2c(GLOVE_I2C_SDA, GLOVE_I2C_SCL);

Serial pc(USBTX, USBRX);

DigitalOut led(LED1);

void imu_test() {

    IMU_BNO055 imu(i2c);

    imu.startUpdateTask(0.5);

    for (;;) {
        imu.print(pc);

        led = !led;
        Thread::wait(1000);
    }
}

void touch_sensor_test() {
    key_states_t keys;
    key_states_t last_keys;

    TouchSensor touch_sensor;
    Thread touch_sensor_thread;
    touch_sensor_thread.start(&touch_sensor, &TouchSensor::updateTask);

    for (;;) {

        last_keys = keys;
        touch_sensor.writeKeys(&keys);

        if (last_keys.pack() != keys.pack()) {
            pc.printf("Key States: %hu %hu %hu %hu",
                    keys.a, keys.b, keys.c, keys.d);
        }
        led = !led;
        Thread::wait(200);
    }
}

void flex_test() {

    FlexSensors flex_sensors;
    flex_sensors.startUpdateTask(0.5);

    for (;;) {

        flex_sensors.printSingle(pc, 0);

        led = !led;
        Thread::wait(1000);
    }
}

void launch_periodic() {
    TouchSensor touch_sensor;
    Thread touch_sensor_thread;
    touch_sensor_thread.start(&touch_sensor, &TouchSensor::updateTask);

    //FlexSensors flex_sensors;
    //flex_sensors.startUpdateTask();

    //IMU_BNO055 imu(i2c);
    //imu.startUpdateTask();

    //Collector collector(&flex_sensors, &imu, &touch_sensor, &pc);
    //collector.startUpdateTask(1); // 1 sec period for serial out

    for (;;) {
        led = !led; // just so we know its running
        Thread::wait(500);
    }
}

int main() {

    /*
     * The current setup with main.cpp is to have single calls to
     * test functions so we can each have test code without having
     * to comment out/have multiple versions.
     * Just change your local one to call the test loop you need.
     */

    imu_test();

    // Just in case the above returns
    Thread::wait(osWaitForever);
}
