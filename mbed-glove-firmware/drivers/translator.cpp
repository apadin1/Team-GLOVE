/*
 * Filename:  translator.cpp
 * Project:   EECS 473 - Team GLOVE
 * Date:      Fall 2016
 * Authors:
 *     Nick Bertoldi
 *     Ben Heckathorn
 *     Ryan O’Keefe
 *     Adrian Padin
 *     Tim Schumacher
 *
 * Purpose:
 *  Implementation of translator.h
 */

#include "translator.h"

Translator::translator() {
  //TODO: Initialization
  //TODO: Determine default glove mapping
  //sensors[] = { TBD }

  update_task_timer = new RtosTimer(this, &Collector::update, osTimerPeriodic);
}

//TODO: Determine FINAL process w/ transciever
void Translator::updateGestureMap(std::vector<AnalogButton>* updatedMapping) {
   sensors = *updatedMapping;
}

void Translator::gestureCheck() {
  working = 1;

  //Update sensor data

  for (int i = 0; i < GESTURE_COUNT; ++i) {

    /* Keyboard functionality */
    if (sensors[i].is_keyboard()) {
      keyboardData keyboard = sensors[i].get_keyboard_data();
      if (keyboard.changed && keyboard.valid) {
        if (keyboard.digital_value) {
          HIDinput.keyPress(keyboard.key);
        }
        else {
          //TODO: Ask Adrian for keyboard push/release functionality
          HIDinput.keyRelease();
        }
      }
    }//keyboard

    /* Mouse functionality */
    else if (sensors[i].is_mouse()) {
      mouseData mouse = sensors[i].get_mouse_data(); //Grab mouse data
      //if (changed == 1)
      if (mouse.changed && mouse.valid) {

        /* Left click */
        if (mouse.part == LBUTTON) {
          if (mouse.digital_value) {
            HIDinput.setMouseButton(LEFT, DOWN);
          }
          else {
            HIDinput.setMouseButton(LEFT, UP);
          }
        }

        /* Right click */
        else if (mouse.part == RBUTTON) {
          if (mouse.digital_value) {
            HIDinput.setMouseButton(RIGHT, DOWN);
          }
          else {
            HIDinput.setMouseButton(RIGHT, UP);
          }
        }

        /* Scroll functionality */
        else if (mouse.part == SCROLL) {
          HIDinput.setMouseScroll(mouse.speed);
        }

        /* X-axis functionality */
        else if (mouse.part == XAXIS) {
          HIDinput.setMouseSpeedX(mouse.speed);
        }

        /* Y-axis functionality */
        else if (mouse.part == YAXIS) {
          HIDinput.setMouseSpeedY(mouse.speed);
        }
      }//for
    }//mouse
    //TODO: Joystick implementation
    //else if (sensors[i].is_joystick()) {
      //getJoystickData()
      //if (changed == 1)
    //}//joystick
  }//for
  working = 0;
}

void Translator::startUpdateTask(uint32_t ms) {
  update_task_timer->start(ms);
}
void Translator::stopUpdateTask() {
  update_task_timer->stop();
}
