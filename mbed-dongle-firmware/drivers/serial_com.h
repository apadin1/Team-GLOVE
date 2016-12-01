// serial_com.h

#ifndef SERIAL_COM_H_
#define SERIAL_COM_H_

#include "mbed.h"

#define SEND(args...) pc.printf(args)
#define MAX_LEN 28

static Serial pc(USBTX, USBRX);
static char rx_buffer[MAX_LEN];

static DigitalOut led1(LED1, 1);


/******************** STATIC FUNCTIONS ********************/

static getTranslator(void * new_ptr=NULL) {
    static Translator * ptr;
    if (new_ptr != NULL) {
        ptr = (Translator *) new_ptr;
    }
    return ptr;
}

static getScanner(void * new_ptr=NULL) {
    static Scanner * ptr;
    if (new_ptr != NULL) {
        ptr = (Scanner *) new_ptr;
    }
    return ptr;
}


/******************** INTERRUPT FUNCTIONS ********************/

// Interupt to read data from serial port
void Rx_interrupt() {
    led1 = false;

    // STOP BLE SCANNING
    getScanner()->stopScan();

    // Read in data
    int len = 0;
    while (len < MAX_LEN) {
        rx_buffer[len++] = pc.getc();
    }

    // Configure the translator
    getTranslator->updateGestureMap((uint8_t *) rx_buffer);

    led1 = true;
}

// MAIN
void serialInit(Translator * translator, Scanner * scanner) {
    getTranslator(translator);
    getScanner(scanner);
    pc.attach(&Rx_interrupt, Serial::RxIrq);
    while(1);
}

#endif //SERIAL_COM_H_
