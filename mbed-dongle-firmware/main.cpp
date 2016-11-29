#include <inttypes.h>
#include "drivers/translator.h"

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

  /* Initialize Translator object */
  Translator translator(&leftGlove, &rightGlove, &input);
  //TODO: Initialize Scanner Object
  translator.startUpdateTask(20);


  for (;;) {
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
    launch();
}
