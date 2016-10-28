#include "mbed.h"
#include "rtos.h"

#include "drivers/flex_sensor.h"
#include "drivers/imu.h"
#include "drivers/touch_sensor.h"

Serial pc(USBTX, USBRX);

DigitalOut led(LED1);

float brightness = 0.0;

void blink() {
    // (note the calls to Thread::wait below for delays)
    while (true) {
        led = !led;
        pc.printf("This is a thing\r\n");
        Thread::wait(1000);
    }
}

void imu_test() {

    pc.printf("Starting IMU test\r\n");
    IMU_BNO055 imu(pc);

    for (;;) {
        imu.update();
        imu.print(pc);

        led = !led;
        Thread::wait(500);
    }
}

void echo_term() {
    pc.printf("Press 'u' to turn LED1 brightness up, 'd' to turn it down\n");
    for (;;) {
        char c = pc.getc();
        pc.putc(c);

        if ((c == 'u') && (brightness < 0.5)) {
            brightness += 0.01;
            led = brightness;
        }
        if ((c == 'd') && (brightness > 0.0)) {
            brightness -= 0.01;
            led = brightness;
        }

    }
}

void touch_sensor_complete() {
    key_states_t keys;
    key_states_t last_keys;

    TouchSensor touch_sensor;
    Thread touch_sensor_thread;
    touch_sensor_thread.start(&touch_sensor, &TouchSensor::updateTask);

    for (;;) {

        last_keys = keys;
        touch_sensor.writeKeys(keys);

        if (last_keys.pack() != keys.pack()) {
            pc.printf("Key States: %hu %hu %hu %hu",
                    keys.a, keys.b, keys.c, keys.d);
        }
        Thread::wait(200);
    }
}

void flex_read() {

    FlexSensors flex_sensors;
    //uint16_t flex_vals[FLEX_SENSORS_COUNT];

    for (;;) {
        led = !led; // just so we know its running

        flex_sensors.update();
        //flex_sensors.writeSensors(flex_vals);
        flex_sensors.printSingle(pc, 0);

        Thread::wait(600);
    }
}

void launch_periodic() {
    TouchSensor touch_sensor;
    Thread touch_sensor_thread;
    touch_sensor_thread.start(&touch_sensor, &TouchSensor::updateTask);

    FlexSensors flex_sensors;
    flex_sensors.startUpdateTask();

    //IMU_BNO055 imu;

    //Collector collector(flex_sensors, touch_sensor, imu);
    //collector.startTransmission();
    //Thread::wait(osWaitForever);
}

int main() {

    /*
     * The current setup with main.cpp is to have single calls to
     * test functions so we can each have test code without having
     * to comment out/have multiple versions.
     * Just change your local one to call the test loop you need.
     */

    launch_periodic();

    // Just in case the above returns
    Thread::wait(osWaitForever);
}
