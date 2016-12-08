// serial_com.h

#ifndef SERIAL_COM_H_
#define SERIAL_COM_H_

#include "mbed.h"
#include "scanner.h"
#include "translator.h"
#include "gpio.h"

static Serial pc(USBTX, USBRX);

#define SEND(args...) pc.printf(args)
#define CONFIG_LENGTH 28

static char rx_buffer[CONFIG_LENGTH];


/******************** STATIC FUNCTIONS ********************/

static Translator * getleftTranslator(void * new_ptr=NULL) {
    static Translator * leftptr;
    if (new_ptr != NULL) {
        leftptr = (Translator *) new_ptr;
    }
    return leftptr;
}

static Translator * getrightTranslator(void * new_ptr=NULL) {
  static Translator * rightptr;
  if (new_ptr != NULL) {
      rightptr = (Translator *) new_ptr;
  }
  return rightptr;
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

// Deferred interrupt implementation
void gestureConfig(void const *argument) {

    static int len = 0;

    // STOP BLE SCANNING/TRANSLATING
    getScanner()->stopScan();
    getleftTranslator()->stopUpdateTask();
    getrightTranslator()->stopUpdateTask();

    led1 = 1;
    led2 = 1;
    led3 = 1;
    led4 = 1;
    
    // Read in data
    while (pc.readable() && (len < CONFIG_LENGTH)) {
        rx_buffer[len] = pc.getc();
        len = ((len+1) % CONFIG_LENGTH);
    }
    
    led1 = 0;

    // Pointer to rx_buff
    uint8_t* rxptr = (uint8_t*) &rx_buffer;

    // Configure the translators
    getleftTranslator()->updateGestureMap(rxptr);
    
    led2 = 0;
    
    rxptr += 14;
    getrightTranslator()->updateGestureMap(rxptr);
    
    led3 = 0;

    // START BLE SCANNING/TRANSLATING
    getleftTranslator()->startUpdateTask(20);

    led4 = 0;

    getrightTranslator()->startUpdateTask(20);
    
    led1 = 1;
    led2 = 1;
    led3 = 1;
    led4 = 1;
    
    getScanner()->startScan();
}


//TODO: Deferred interrupt
// Interupt to read data from serial port
void Rx_interrupt() {

    // Setup deferred interrupt
    RtosTimer configTask(gestureConfig, osTimerOnce, NULL);
    configTask.start(1000);
}

// MAIN
void serialInit(Translator * leftTranslator, Translator * rightTranslator,
                Scanner * scanner) {
    getleftTranslator(leftTranslator);
    getrightTranslator(rightTranslator);
    getScanner(scanner);
    pc.attach(&Rx_interrupt, Serial::RxIrq);
}


#endif //SERIAL_COM_H_
