// serial_com.h

#ifndef SERIAL_COM_H_
#define SERIAL_COM_H_

#include "mbed.h"
#include "scanner.h"
#include "translator.h"

static Serial pc(USBTX, USBRX);

#define SEND(args...) pc.printf(args)
#define CONFIG_LENGTH 28

static char rx_buffer[CONFIG_LENGTH];

static DigitalOut led2(LED2, 1);


/******************** STATIC FUNCTIONS ********************/

static Translator * getTranslator(void * new_ptr=NULL) {
    static Translator * ptr;
    if (new_ptr != NULL) {
        ptr = (Translator *) new_ptr;
    }
    return ptr;
}


static Scanner * getScanner(void * new_ptr=NULL) {
    static Scanner * ptr;
    if (new_ptr != NULL) {
        ptr = (Scanner *) new_ptr;
    }
    return ptr;
}


void print_config() {
    for (int i = 0; i < CONFIG_LENGTH; ++i) {
        printf("%d: %d\r\n", i, rx_buffer[i]);
    }
}


/******************** INTERRUPT FUNCTIONS ********************/

// Interupt to read data from serial port
void Rx_interrupt() {
    static int len = 0;

    // STOP BLE SCANNING
    getScanner()->stopScan();
    
    // Read in data
    while (pc.readable() && (len < CONFIG_LENGTH)) {
        rx_buffer[len] = pc.getc();
        len = ((len+1) % CONFIG_LENGTH);
    }
    
    // Configure the translator
    getTranslator()->updateGestureMap((uint8_t *) rx_buffer);

    // START BLE SCANNING
    getScanner()->startScan();
}

// MAIN
void serialInit(Translator * translator, Scanner * scanner) {
    getTranslator(translator);
    getScanner(scanner);
    pc.attach(&Rx_interrupt, Serial::RxIrq);
}

#endif //SERIAL_COM_H_
