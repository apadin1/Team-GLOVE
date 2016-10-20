#include "mbed.h"

#include "flex_sensor.h"

#include "BNO055_fusion/BNO055.h"

Serial pc(USBTX, USBRX);

DigitalOut led(LED1);

AnalogIn flex(A1);
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

    flex_sensor_t* f0 = initFlexSensor(A0);

    for (;;) {
        led = !led;
        pc.printf("Flex0 reading is: 0x%x\r\n", flex.read_u16());

        updateFlexSensorDeflection(f0);
        pc.printf("Flex1 reading is: 0x%x\r\n", f0->deflection);
        Thread::wait(500);
    }
}

int main() {

    flex_read();
}
