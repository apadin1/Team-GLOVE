// serial_com.h

#ifndef SERIAL_COM_H_
#define SERIAL_COM_H_

#include "mbed.h"
#include "scanner.h"
#include "translator.h"
#include "gpio.h"

#define CONFIG_LENGTH 28

static Serial pc(USBTX, USBRX);
//static Semaphore config_sem;

static char rx_buffer[CONFIG_LENGTH];
bool config_changed = false;


/******************** INTERRUPT FUNCTIONS ********************/

// Wrapper class for serial communication
// Used for configuration
class SerialCom {
public:
    
    // Constructor
    SerialCom(Translator * _left_translator,
              Translator * _right_translator,
              Scanner * _scanner);
    
    // Start the configuration task
    void startConfigTask();

    // Send configuration information to the translators
    void gestureConfig();


private:
    Translator * left_translator;
    Translator * right_translator;
    Scanner * scanner;
    RtosTimer * config_task;
};


/******************** INTERRUPT FUNCTIONS ********************/

// Return pointer to serial device
SerialCom* getSerial(void* new_ptr=NULL) {
    SerialCom* serial_ptr = NULL;
    if (new_ptr != NULL) {
        serial_ptr = (SerialCom*) new_ptr;
    }
    return serial_ptr;
}

// Interupt to read data from serial port
void Rx_interrupt() {
    led2 = 0;
    config_changed = true;
    
    // Read in data
    int len = 0;
    while (len < CONFIG_LENGTH) {
        pin15 = 1;
        rx_buffer[len++] = pc.getc();
        pin15 = 0;
    }
    
    led2 = 1;
}


// Constructor
SerialCom::SerialCom(Translator * _left_translator,
              Translator * _right_translator,
              Scanner * _scanner) :
        left_translator(_left_translator),
        right_translator(_right_translator),
        scanner(_scanner) {

    // Set up the interrupt for when data is ready on the serial port
    pc.attach(&Rx_interrupt, Serial::RxIrq);
    getSerial(this);
    
    config_task = new RtosTimer(&serial_com, &SerialCom::gestureConfig, osTimerPeriodic);
    config_task->start(5000);
}

// Send configuration information to the translators
void SerialCom::gestureConfig() {

    if (config_changed) {
            
        led4 = 0;

        // STOP BLE SCANNING/TRANSLATING
        pin14 = 1;
        //scanner->stopScan();
        left_translator->stopUpdateTask();
        right_translator->stopUpdateTask();
        pin14 = 0;
       
        // Pointer to rx_buff
        uint8_t* rxptr = (uint8_t*) rx_buffer;

        // Configure the translators
        pin16 = 1;
        left_translator->updateGestureMap(rxptr);
        rxptr += 14;
        right_translator->updateGestureMap(rxptr);
        pin16 = 0;
        
        // START BLE SCANNING/TRANSLATING
        left_translator->startUpdateTask(20);
        right_translator->startUpdateTask(20);
        //scanner->startScan();
        
        config_changed = false;
    }
}


#endif //SERIAL_COM_H_
