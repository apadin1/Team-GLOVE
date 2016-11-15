#include <inttypes.h>

extern void blink(void);
extern void boot_delay(uint8_t);
extern void sensors_to_lights(void);

void launch() {
  TouchSensor touch_sensor;


}

extern void keyboard_mouse_demo(void);

int main() {

    /*
     * The current setup with main.cpp is to have single calls to
     * test functions so we can each have test code without having
     * to comment out/have multiple versions.
     * Just change your local one to call the test loop you need.
     */
    //flex_to_lights();
    //imu_to_lights();
    //launch_periodic();
    //keyboard_mouse_demo();
    sensors_to_lights();
    //blink();
}
