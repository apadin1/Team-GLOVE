#include "mbed.h"

DigitalOut led(LED1);
Serial debug(USBTX, USBRX);

AnalogIn flex(A0);
float brightness = 0.0;


void blink() {
    // (note the calls to Thread::wait below for delays)
    while (true) {
        led = !led;
        debug.printf("This is a thing\r\n");
        Thread::wait(1000);
    }
}

void echo_term() {
    debug.printf("Press 'u' to turn LED1 brightness up, 'd' to turn it down\n");
    for (;;) {
        char c = debug.getc();
        debug.putc(c);

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

void flex_read() {

    for (;;) {
        debug.printf("Flex reading is: %f\n", flex);
        Thread::wait(1000);
        /*
        if (c == 'r') {
            debug.printf("\nFlex reading is: %f\n", flex);
        }
        */
    }
}

int main() {

    flex_read();
}
