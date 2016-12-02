#include "mbed.h"
#include "rtos.h"

#include "drivers/flex_sensor.h"
#include "drivers/imu.h"
#include "drivers/touch_sensor.h"

#include "drivers/analog_button.h"
#include "drivers/dot_star_leds.h"
#define INCLUDE_TOUCH 1

const PinName GLOVE_I2C_SDA = p30; //I2C_SDA0; // p30
const PinName GLOVE_I2C_SCL = p7; //I2C_SCL0; // p7

static I2C i2c(GLOVE_I2C_SDA, GLOVE_I2C_SCL);

static Serial pc(USBTX, USBRX);

static DigitalOut led(LED1);
static DigitalOut l2(LED2);
static DigitalOut l3(LED3);
static DigitalOut l4(LED4);

void touch_to_lights() {
    key_states_t keys;
    DigitalOut led1(P0_15);
    DigitalOut led2(P0_14);
    DigitalOut led3(P0_13);
    DigitalOut led4(P0_12);
    I2C i2c(I2C_SDA0, I2C_SCL0);
    TouchSensor touch_sensor(i2c, TOUCH_INTERRUPT);
    for (;;) {
        touch_sensor.updateAndWrite(&keys);
        if (keys.a == 1)
            led1 = 0;
        else led1 = 1;
        if (keys.b == 1)
            led2 = 0;
        else led2 = 1;
        if (keys.c == 1)
            led3 = 0;
        else led3 = 1;
        if (keys.d == 1)
            led4 = 0;
        else led4 = 1;
        wait_ms(10);
    }
}

void imu_to_lights() {
  bno_imu_t data;
  DigitalOut led1(P0_15);
  DigitalOut led2(P0_14);
  DigitalOut led3(P0_13);
  DigitalOut led4(P0_12);
  I2C i2c(I2C_SDA0, I2C_SCL0);
  IMU_BNO055 imu(i2c);
  for (;;) {
    imu.updateAndWrite(&data);
    if (data.orient_pitch > 30) {
      led1 = 0;
    }
    else led1 = 1;
    if (data.orient_roll > 40) {
      led2 = 0;
    }
    else led2 = 1;
    if (data.orient_yaw > 15) {
      led3 = 0;
    }
    else led3 = 1;
    wait_ms(10);
  }
}

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

void calibration_mode() {
    /*
     * The idea here is to go into a special mode for a fixed time,
     * and measure the maximum and minimum values on all the sensors
     */

    // setup objects

    const uint16_t ms_period = 20;
    for (uint32_t i = 0; i < 8000 / (ms_period - 8); ++i) {

        // gather data

        // update max/mins

        // print results

        Thread::wait(ms_period - 8);
    }
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
    TouchSensor touch_sensor(i2c, p16);
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
    for (;;) {
        touch_sensor.spawnUpdateThread();

        imu.updateAndWrite(&imu_vals);
        flex_sensors.updateAndWrite(flex_vals);
        touch_sensor.writeKeys(&keys);

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

        touch_sensor.terminateUpdateThreadIfBlocking();
        Thread::wait(40);
    }
}
