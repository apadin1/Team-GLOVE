#include <inttypes.h>

extern void blink(void);
extern void boot_delay(uint8_t);
extern void sensors_to_lights(void);

int main() {

    /*
     * The current setup with main.cpp is to have single calls to
     * test functions so we can each have test code without having
     * to comment out/have multiple versions.
     * Just change your local one to call the test loop you need.
     */
    sensors_to_lights();
    //blink();
}
