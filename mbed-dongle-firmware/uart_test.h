// uart_test.cpp
#include "mbed.h"
#include "serial_com.h"

extern int left_count;
extern int right_count;


static InterruptIn button111(BUTTON1);
static InterruptIn button222(BUTTON2);
static InterruptIn button333(BUTTON3);

void printAdvertisementCounts() {
    printf("Left: %d\r\n", left_count);
    printf("Right: %d\r\n", right_count);
}

extern void print_config();

// MAIN
void uart_test(void) {
    
    left_count = 0;
    right_count = 0;
    button111.fall(printAdvertisementCounts);
    button222.fall(print_config);
    

    Scanner scanner((Translator * ) NULL);
    serialInit((Translator * ) NULL, &scanner);
        
    scanner.startScan();
    while(1) {
        scanner.waitForEvent();
    }
}