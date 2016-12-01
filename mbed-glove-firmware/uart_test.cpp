// uart_test.cpp
#include "mbed.h"

#define SEND(args...) pc.printf(args)
#define MAX_LEN 28

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
    while (len < MAX_LEN) {
        rx_buffer[len++] = pc.getc();
    }
    led1 = true;
    
    if ((rx_buffer[0] == '2') &&
        (rx_buffer[1] == '1') &&
        (rx_buffer[2] == '1') &&
        (rx_buffer[3] == '1') &&
        (rx_buffer[4] == '1') &&
        (rx_buffer[5] == '1') &&
        (rx_buffer[6] == '1') &&
        (rx_buffer[7] == '1') &&
        (rx_buffer[8] == '1') &&
        (rx_buffer[9] == '1') &&
        (rx_buffer[10] == '1') &&
        (rx_buffer[11] == '1') &&
        (rx_buffer[12] == '1') &&
        (rx_buffer[13] == '1') &&
        (rx_buffer[14] == '1') &&
        (rx_buffer[15] == '4')) {
        
        led2 = !led2;
    }
    else {
        led3 = !led3;
    }
    /*
    if (rx_buffer[0] == '1') led1 = !led1;
    if (rx_buffer[0] == '2') led2 = !led2;
    if (rx_buffer[0] == '3') led3 = !led3;
    if (rx_buffer[0] == '4') led4 = !led4;
    */
    
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