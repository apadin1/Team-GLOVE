#include "mbed.h"

#include "flex_sensor.h"
#include "imu.h"


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

void imu_test() {

    pc.printf("Starting IMU test\r\n");
    IMU_BNO055 imu(pc);

    for (;;) {
        imu.update();
        imu.print(pc);

        led = !led;
        Thread::wait(500);
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

    AnalogIn flex(A0);
    uint16_t val;

    for (;;) {
        led = !led;
        val = flex.read_u16();
        pc.printf("0x%hx | %hu\r\n", val, val);

        Thread::wait(500);
    }
}

int main() {

    /*
     * The current setup with main.cpp is to have single calls to
     * test functions so we can each have test code without having
     * to comment out/have multiple versions.
     * Just change your local one to call the test loop you need.
     */

    flex_read();
}
