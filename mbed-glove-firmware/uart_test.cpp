// uart_test.cpp
#include "mbed.h"

#define SEND(args...) pc.printf(args)
#define MAX_LEN 20

static Serial pc(USBTX, USBRX);
static char rx_buffer[MAX_LEN];

static DigitalOut led1(LED1, 1);
static DigitalOut led2(LED2, 1);
static DigitalOut led3(LED3, 1);
static DigitalOut led4(LED4, 1);
static InterruptIn button1(BUTTON1);


// Interupt to read data from serial port
void Rx_interrupt() {
    led1 = false;
    int len = 0;
    while (pc.readable() && len < MAX_LEN) {
        rx_buffer[len++] = pc.getc();
    }
    rx_buffer[len] = 0;
    SEND(rx_buffer);
    led1 = true;
}

// MAIN
void uart_test(void) {
    led1 = true;
    led2 = true;
    led3 = true;
    led4 = true;
    pc.attach(&Rx_interrupt, Serial::RxIrq);
    while(1);
}