#include "mbed.h"

Serial pc(USBTX, USBRX);

DigitalOut led(LED1);

AnalogIn flex(A0);
float brightness = 0.0;


void blink() {
    // (note the calls to Thread::wait below for delays)
    while (true) {
        led = !led;
        pc.printf("This is a thing\r\n");
        Thread::wait(1000);
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

void flex_read() {

    for (;;) {
        led = !led;
        pc.printf("Flex reading is: 0x%x\r\n", flex.read_u16());
        Thread::wait(500);
    }
}

int main() {

    flex_read();
}
