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
DigitalOut l2(LED2);
DigitalOut l3(LED3);
DigitalOut l4(LED4);

void blink() {
    l2 = 1;
    led = 0;
    for (;;) {
        led = !led;
        l2 = !l2;
        Thread::wait(520);
    }
}

void boot_delay(uint8_t t) {
    // this loop is to prevent the strange fatal state
    // happening with serial debug
    led = 1;
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

/*
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
    boot_delay(2);
    TouchSensor touch_sensor(i2c);
    Thread touch_sensor_thread;
    touch_sensor_thread.start(&touch_sensor, &TouchSensor::updateTask);
    DigitalOut l2(LED2);
    DigitalOut l3(LED3);
    DigitalOut l4(LED4);
    boot_delay(2);
    for (;;) {
        last_keys = keys;
        touch_sensor.writeKeys(&keys);
        led = !keys.a;
        l2 = !keys.b;
        l3 = !keys.c;
        l4 = !keys.d;
        //if (last_keys.pack() != keys.pack()) {
            //TouchSensor::print(pc, keys);
        //}
        Thread::wait(60);
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
*/
/*
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

    boot_delay(5);
    uint8_t print_limit = 0;
    for (;;) {
        led = !led; // just so we know its running

        // Flex
        //flex_sensors.printSingle(pc, 0);
        flex_sensors.print(pc);

        // Touch
        last_keys = keys;
        touch_sensor.writeKeys(&keys);
        if (last_keys.pack() != keys.pack()) {
            TouchSensor::print(pc, keys);
        }

        if (print_limit++ == 2) {
            imu.print(pc);
            print_limit = 0;
        }

        Thread::wait(1000);
    }
}
*/
Thread* touch_sensor_thread = new Thread;
TouchSensor touch_sensor(i2c, TOUCH_NO_INTERRUPT);

void touch_term() {
    l2=0;
    if (l4 == 0) {
        touch_sensor_thread->terminate();
        l4 = 1;
        wait_ms(400);
    }

    /*
    switch (touch_sensor_thread.get_state()) {
    case (Thread::Running):
    case (Thread::Ready):
    case (Thread::WaitingDelay):
    case (Thread::WaitingInterval):
    case (Thread::WaitingOr):
    case (Thread::WaitingAnd):
    case (Thread::WaitingSemaphore):
    case (Thread::WaitingMailbox):
    case (Thread::WaitingMutex):
        touch_sensor_thread.terminate();
        l4 = 1;
        wait_ms(400);
        break;
    case (Thread::Inactive):
    case (Thread::Deleted):
    default:
        break;
    }
    */

    //touch_sensor_thread.set_priority(osPriorityLow);
    // set a do_restart for the sem
    wait_ms(3);
    l2=1;
}

void sensors_to_lights() {
    led = 1;
    l2 = 1;
    l3 = 1;
    l4 = 1;

    DotStarLEDs ds_leds(2);
    uint8_t red, green, blue;

    IMU_BNO055 imu(i2c);
    bno_imu_t imu_vals;

    FlexSensors flex_sensors;
    flex_sensor_t flex_vals[4];

    //touch_sensor_thread.start(&touch_sensor, &TouchSensor::updateTask);
    key_states_t keys;
    //touch_sensor_thread.set_priority(osPriorityBelowNormal);

    float flex_val;

    uint16_t flex_min = 250;
    uint16_t flex_max = 750;

    /*
     * Flex zero sets led 0 red/blue
     *
     * Any touch sets both lights to bright white
     *
     * Light one is the combined IMU status
     */
    Timeout kill_touch;
    for (;;) {
        led = 0;
        touch_sensor_thread = new Thread;
        kill_touch.attach_us(&touch_term, 350000);
        touch_sensor_thread->start(&touch_sensor, &TouchSensor::singleUpdate);
        //Thread::yield();
        //touch_sensor_thread.join();

        touch_sensor.writeKeys(&keys);
        flex_sensors.updateAndWrite(flex_vals);
        imu.updateAndWrite(&imu_vals);

        if (flex_vals[0] < flex_min) {
            flex_min = flex_vals[0];
        }
        if (flex_vals[0] > flex_max) {
            flex_max = flex_vals[0];
        }

        if (keys.pack()) {
            ds_leds.set_RGB(0,0,255,0);
        }
        else {

            // set flex light
            flex_val = map_unsigned_analog_to_percent(flex_min, flex_max, flex_vals[0]);
            red = 255*flex_val;
            green = 0;
            blue = 255*(1-flex_val);
            ds_leds.set_RGB(0, red, green, blue);

            // set imu light
            blue = 255*map_float_analog_to_percent(-45.0, 45.0, imu_vals.orient_pitch);
            red = 255*map_float_analog_to_percent(-45.0, 45.0, imu_vals.orient_roll);
            green = 255*map_float_analog_to_percent(0.0, 360.0, imu_vals.orient_yaw);
            ds_leds.set_RGB(1, red, green, blue, 3);
        }

        led = 1;
        Thread::wait(1000);
        delete touch_sensor_thread;
    }
}
