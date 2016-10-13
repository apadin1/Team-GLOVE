#include "mbed.h"

DigitalOut led1(LED1);
Serial debug(USBTX, USBRX);

AnalogIn flex(A0);
float brightness = 0.0;

int main() {
    debug.printf("Press 'u' to turn LED1 brightness up, 'd' to turn it down\n");

    /*
    // (note the calls to Thread::wait below for delays)
    while (true) {
        led1 = !led1;
        debug.printf("This is a thing\r\n");
        Thread::wait(1000);
    }
    */

    while(1) {
        char c = debug.getc();
        debug.putc(c);

        if((c == 'u') && (brightness < 0.5)) {
            brightness += 0.01;
            led = brightness;
        }
        if((c == 'd') && (brightness > 0.0)) {
            brightness -= 0.01;
            led = brightness;
        }

        if ((c == 'r') {
                debug.printf("\nFlex reading is: %f\n", flex);
        }

    }
}
