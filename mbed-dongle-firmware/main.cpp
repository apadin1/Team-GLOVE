#include <inttypes.h>
#include "drivers/translator.h"

extern void blink(void);
extern void boot_delay(uint8_t);
extern void sensors_to_lights(void);
extern void thing_do(void);
extern void keyboard_mouse_demo(void);

/*
void launch() {
    DigitalOut l1(LED1);
    DigitalOut l2(LED2);
    DigitalOut l3(LED3);
    DigitalOut l4(LED4);

    l1 = 1;
    l2 = 1;
    l3 = 1;
    l4 = 1;

    FlexSensors flex; // Initialize flex sensor object
    I2C i2c(I2C_SDA0, I2C_SCL0); // Initialize i2c bus for imu and touch_sensor
    IMU_BNO055 imu(i2c); // Initialize imu object

    // Start update thread for touch sensor
    TouchSensor touch_sensor(i2c, TOUCH_INTERRUPT); //Initialize touch sensor object

    // Initialize KeyboardMouse object
    KeyboardMouse input;

    Translator translator(&flex, &imu, &touch_sensor, &input);

    translator.startUpdateTask(20);


    for (;;) {
        l2 = 0;
        //input.waitForEvent();
        l2 = 1;
    }

}
*/

extern void keyboard_mouse_demo(void);

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
    keyboard_mouse_demo();
    //launch();
}
