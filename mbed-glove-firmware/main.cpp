#include <inttypes.h>

extern void blink(void);
extern void boot_delay(uint8_t);
extern void sensors_to_lights(void);

extern void keyboard_mouse_demo(void);

/*
int main() {

    /*
     * The current setup with main.cpp is to have single calls to
     * test functions so we can each have test code without having
     * to comment out/have multiple versions.
     * Just change your local one to call the test loop you need.
<<<<<<< HEAD
     /
    //sensors_to_lights();
=======
     */
    sensors_to_lights();
>>>>>>> 2bdec1d0c5be2372153dd1e33ef1a789dc1b930b
    //blink();
    //launch_periodic();
    //keyboard_mouse_demo();
}
*/