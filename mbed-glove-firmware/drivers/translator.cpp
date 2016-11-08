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

Translator::translator(FlexSensors* flex, IMU_BNO055* imu, TouchSensor* touch) {
  //TODO: Initialization
  //TODO: Determine default glove mapping (Minecraft?)

  /* FLEX1 */
  //AnalogButton flex1;
  //sensors.push_back(flex1);

  /* FLEX2 */
  //AnalogButton flex2;
  //sensors.push_back(flex2);

  /* FLEX3 */
  //AnalogButton flex3;
  //sensors.push_back(flex3);

  /* FLEX4 */
  //AnalogButton flex4;
  //sensors.push_back(flex4);

  /* TOUCH1 */
  //AnalogButton touch1;
  //sensors.push_back(touch1);

  /* TOUCH2 */
  //AnalogButton touch2;
  //sensors.push_back(touch2);

  /* TOUCH3 */
  //AnalogButton touch3;
  //sensors.push_back(touch3);

  /* TOUCH4 */
  //AnalogButton touch4;
  //sensors.push_back(touch4);

  /* PITCHUP */
  //AnalogButton pitchup;
  //sensors.push_back(pitchup);

  /* PITCHDOWN */
  //AnalogButton pitchdown;
  //sensors.push_back(pitchdown);

  /* ROLLLEFT */
  //AnalogButton rollleft;
  //sensors.push_back(rollleft);

  /* ROLLRIGHT */
  //AnalogButton rollright;
  //sensors.push_back(rollright);

  /* YAWLEFT */
  //AnalogButton yawleft;
  //sensors.push_back(yawleft);

  /* YAWRIGHT */
  //AnalogButton yawright;
  //sensors.push_back(yawright);

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
