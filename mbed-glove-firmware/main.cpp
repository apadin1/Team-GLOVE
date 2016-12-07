#include <inttypes.h>
#include <utility>

#include "drivers/collector.h"
#include "drivers/ble_advert.h"
#include "drivers/dot_star_leds.h"

extern void blink(void); extern void boot_delay(uint8_t);
extern void sensors_to_lights(void);
extern void advert_test(void);
extern void touch_to_lights(void);
extern void imu_to_lights(void);

typedef pair<DotStarColor, DotStarColor> color_pair_t;
#if defined RIGHT_GLOVE
const uint8_t num_led_patterns_c = 2;
#elif defined LEFT_GLOVE
const uint8_t num_led_patterns_c = 3;
#endif

const color_pair_t led_patterns_c[num_led_patterns_c] = {
    //color_pair_t(Blue, Cyan),
    //color_pair_t(Red, Magenta),
    //color_pair_t(Green, Yellow),
#if defined RIGHT_GLOVE
    color_pair_t(Blue, Maize),
    color_pair_t(Maize, Blue),
#elif defined LEFT_GLOVE
    color_pair_t(White, Pink),
    color_pair_t(Maize, Blue),
#endif
};

/*
 * Use this to signal or very the lights based on
 * the glove data or return true to let the set
 * pattern continue.
 */
bool check_signal_conditions(const glove_sensors_raw_t& glove_data, DotStarLEDs& leds) {
    /*
     * Indicate IMU flatlining
     */
    float ubound = 0.001;
    float lbound = ubound - (2*ubound);
    if ((lbound < glove_data.imu.orient_pitch && glove_data.imu.orient_pitch < ubound)
         && (lbound < glove_data.imu.orient_roll && glove_data.imu.orient_roll < ubound)) {

        leds.set_color_all(Orange, 5);
        return false;
    }

    /*
     * Indicate the first flex sensor is pulled
     */
    if (glove_data.flex_sensors[0] < 450) {
        //leds.set_color_all(Red, 10);
        leds.set_color_all(Cyan, 8);
        return false;
    }

    /*
     * Indicate a touch has occured
     */
    if (glove_data.touch_sensor.a || glove_data.touch_sensor.b || glove_data.touch_sensor.c || glove_data.touch_sensor.d) {
        leds.set_color_all(Magenta, 8);
        return false;
    }

    return true;
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

    Collector collector(flex_sensors, imu, touch_sensor, adble);
    collector.startUpdateTask(20);
    const glove_sensors_raw_t& glove_data = collector.getGloveSensorData();

    /* The following main loop sets the light pattern on the
     * gloves after all the sensors have been initialized,
     * the BLE advertising set up, and the collector is running
     */
    for (;;) {
        //printf("f: %d, t: 0x%x\r\n", collector.flex_data[0], collector.touch_data->pack());
        //printf("comp: %d, fp: %f\r\n", compress_double(110.565), extract_double(11056));

        for (uint8_t i = 0; i < num_led_patterns_c; ++i) {
            l1 = !l1;
            for (int j=0; j < 50; ++j) {
                if (check_signal_conditions(glove_data, ds_leds)) {
                    ds_leds.set_color(0, led_patterns_c[i].first);
                    ds_leds.set_color(1, led_patterns_c[i].second);
                }
                Thread::wait(10);
            }
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
    launch();
}
