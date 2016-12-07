#include "mbed.h"

// LEDs
DigitalOut led1(LED1, 1);
DigitalOut led2(LED2, 1);
DigitalOut led3(LED3, 1);
DigitalOut led4(LED4, 1);

// Buttons
InterruptIn button1(BUTTON1);
InterruptIn button2(BUTTON2);
InterruptIn button3(BUTTON3);
InterruptIn button4(BUTTON4);

// Output pins
DigitalOut pin10(P0_10, 0);
DigitalOut pin11(P0_11, 0);
DigitalOut pin12(P0_12, 0);
DigitalOut pin13(P0_13, 0);
DigitalOut pin14(P0_14, 0);
DigitalOut pin15(P0_15, 0);
DigitalOut pin16(P0_16, 0);
