#include "mbed.h"

#include "flex_sensor.h"

Serial pc(USBTX, USBRX);

DigitalOut led(LED1);

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

    FlexSensors flex_sensors;
    uint16_t flex_vals[FLEX_SENSORS_COUNT];

    for (;;) {
        led = !led; // just so we know its running

        flex_sensors.updateSensors();
        flex_sensors.writeSensors(flex_vals);

        //pc.printf("0x%x\r\n", flex.read_u16());
        pc.printf("%hu 0x%hx, %hu 0x%hx, %hu 0x%hx, %hu 0x%hx\r\n",
                flex_vals[0], flex_vals[0], flex_vals[1], flex_vals[1],
                flex_vals[2], flex_vals[2], flex_vals[3], flex_vals[3]);

        Thread::wait(800);
    }
}

int main() {

    flex_read();
}
