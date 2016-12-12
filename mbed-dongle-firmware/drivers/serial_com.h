/* serial_com.h
 *
 * Copyright (c) 2016 by Nick Bertoldi, Ben Heckathorn, Ryan O'Keefe,
 *                       Adrian Padin, Timothy Schumacher
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

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

/*
// Deferred interrupt implementation
void gestureConfig(void const *argument) {

    static int len = 0;

    // STOP BLE SCANNING/TRANSLATING
    getScanner()->stopScan();
    getleftTranslator()->stopUpdateTask();
    getrightTranslator()->stopUpdateTask();

    // Read in data
    while (pc.readable() && (len < CONFIG_LENGTH)) {
        rx_buffer[len] = pc.getc();
        len = ((len+1) % CONFIG_LENGTH);
    }

    // Pointer to rx_buff
    uint8_t* rxptr = (uint8_t*) &rx_buffer;

    // Configure the translators
    getleftTranslator()->updateGestureMap(rxptr);
    rxptr += 14;
    getrightTranslator()->updateGestureMap(rxptr);

    // START BLE SCANNING/TRANSLATING
    getleftTranslator()->startUpdateTask(20);
    getrightTranslator()->startUpdateTask(20);
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

*/
#endif //SERIAL_COM_H_
