#include <inttypes.h>
#include "drivers/scanner.h"
#include "drivers/serial_com.h"
#include "drivers/translator.h"


extern void uart_test(void);

void launch() {
  DigitalOut l1(LED1);
  DigitalOut l2(LED2);
  DigitalOut l3(LED3);
  DigitalOut l4(LED4);

  l1 = 1;
  l2 = 1;
  l3 = 1;
  l4 = 1;

  /* Initialize KeyboardMouse object */
  KeyboardMouse input;

  /* Initialize glove_sensors_raw_t for each glove */
  glove_sensors_raw_t leftGlove;
  glove_sensors_raw_t rightGlove;

  /* Initialize Translator and Scanner objects */
  Translator translator(&leftGlove, &rightGlove, &input);
  Scanner scanner(&translator);

  /* Initialize Serial Interrupt */
  serialInit(&translator, &scanner);

  /* Begin Scanner Functionality */
  scanner.startUpdateTask(20);

  for (;;) {
    rightGlove.touch_sensor.a = 1;
    wait(1);
    rightGlove.touch_sensor.a = 0;
    wait(1);
    //Inifite loop
  }

}

int main() {
    /*
     * The current setup with main.cpp is to have single calls to
     * test functions so we can each have test code without having
     * to comment out/have multiple versions.
     * Just change your local one to call the test loop you need.
     */
    //sensors_to_lights();
    //blink();
    //launch_periodic();
    //keyboard_mouse_demo();
    //launch();
    uart_test();
}
