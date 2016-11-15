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

const PinName TRANSLATOR_DEBUG_PIN = p26;

Translator::Translator(FlexSensors* _flex, IMU_BNO055* _imu,
        TouchSensor* _touch, KeyboardMouse* _input)
     : flex(_flex), imu(_imu), touch(_touch), HIDinput(_input), working(TRANSLATOR_DEBUG_PIN) {

  /* FLEX1 */
  AnalogButton<flex_sensor_t> flex1(&glove_data.flex_sensors[0], 300, 800, 0.2, false, false, false);
  flex_sensors[FLEX1] = &flex1;

  /* FLEX2 */
  AnalogButton<flex_sensor_t> flex2(&glove_data.flex_sensors[1], 300, 800, 20, false, false);
  flex_sensors[FLEX2] = &flex2;

  /* FLEX3 */
  AnalogButton<flex_sensor_t> flex3(&glove_data.flex_sensors[2], 300, 800, 20, false, false);
  flex_sensors[FLEX3] = &flex3;

  /* FLEX4 */
  AnalogButton<flex_sensor_t> flex4(&glove_data.flex_sensors[3], 300, 800, 20, false, false);
  flex_sensors[FLEX4] = &flex4;

  /* TOUCH1 */
  AnalogButton<uint8_t> touch1(&glove_data.touch_sensor.a);
  touch_sensors[TOUCH1] = &touch1;

  /* TOUCH2 */
  AnalogButton<uint8_t> touch2(&glove_data.touch_sensor.b);
  touch_sensors[TOUCH2] = &touch2;

  /* TOUCH3 */
  AnalogButton<uint8_t> touch3(&glove_data.touch_sensor.c);
  touch_sensors[TOUCH3] = &touch3;

  /* TOUCH4 */
  AnalogButton<uint8_t> touch4(&glove_data.touch_sensor.d);
  touch_sensors[TOUCH4] = &touch4;

  /* PITCHUP */
  AnalogButton<float> pitchup(&glove_data.imu.orient_pitch, 0, 15, 20, true, true);
  imu_axis[PITCHUP] = &pitchup;

  /* PITCHDOWN */
  AnalogButton<float> pitchdown(&glove_data.imu.orient_pitch, -15, 0, 20, true, false);
  imu_axis[PITCHDOWN] = &pitchdown;

  /* ROLLLEFT */
  AnalogButton<float> rollleft(&glove_data.imu.orient_roll, 0, 20, 20, true, true);
  imu_axis[ROLLLEFT] = &rollleft;

  /* ROLLRIGHT */
  AnalogButton<float> rollright(&glove_data.imu.orient_roll, -20, 0, 20, true, false);
  imu_axis[ROLLRIGHT] = &rollright;

  /* YAWLEFT */
  AnalogButton<float> yawleft(&glove_data.imu.orient_yaw, 0, 20, 20, true, true);
  imu_axis[YAWLEFT] = &yawleft;

  /* YAWRIGHT */
  AnalogButton<float> yawright(&glove_data.imu.orient_yaw, -20, 0, 0, true, false);
  imu_axis[YAWRIGHT] = &yawright;

  /* BUTTON MAPPING */
  //flex_sensors[FLEX1]->change_hid_profile(KEYBOARD, 'a');
  //AnalogButton[FLEX2].change_hid_profile();
  //AnalogButton[FLEX3].change_hid_profile();
  //AnalogButton[FLEX4].change_hid_profile();
  //AnalogButton[TOUCH1].change_hid_profile();
  //AnalogButton[TOUCH2].change_hid_profile();
  //AnalogButton[TOUCH3].change_hid_profile();
  //AnalogButton[TOUCH4].change_hid_profile();
  //AnalogButton[PITCHUP].change_hid_profile();
  //AnalogButton[PITCHDOWN].change_hid_profile();
  //AnalogButton[ROLLLEFT].change_hid_profile();
  //AnalogButton[ROLLRIGHT].change_hid_profile();
  //AnalogButton[YAWLEFT].change_hid_profile();
  //AnalogButton[YAWRIGHT].change_hid_profile();

  update_task_timer = new RtosTimer(this, &Translator::gestureCheck, osTimerPeriodic);
}

//TODO: Determine FINAL process w/ transciever
//void Translator::updateGestureMap(std::vector<AnalogButton>* updatedMapping) {
   //sensors = *updatedMapping;
//}

void Translator::gestureCheck() {

  /* Update Sensor Data */
  flex->updateAndWriteSensors(&glove_data.flex_sensors);

  /* Flex Sensor functionality */
  for (int i = 0; i < FLEX_COUNT; ++i) {

    /* Keyboard functionality */
    if (flex_sensors[i]->is_keyboard()) {
      keyboardData keyboard = flex_sensors[i]->get_keyboard_data();
      if (keyboard.changed && keyboard.valid) {
        if (keyboard.value) {
          HIDinput->keyPress(keyboard.key);
        }
        else {
          HIDinput->keyRelease(keyboard.key);
        }
      }
    }//keyboard

    /* Mouse functionality */
    else if (flex_sensors[i]->is_mouse()) {
      mouseData mouse = flex_sensors[i]->get_mouse_data(); //Grab mouse data
      if (mouse.changed && mouse.valid) {

        /* Left click */
        if (mouse.part == LBUTTON) {
          if (mouse.value) {
            HIDinput->setMouseButton(LEFT, DOWN);
          }
          else {
            HIDinput->setMouseButton(LEFT, UP);
          }
        }

        /* Right click */
        else if (mouse.part == RBUTTON) {
          if (mouse.value) {
            HIDinput->setMouseButton(RIGHT, DOWN);
          }
          else {
            HIDinput->setMouseButton(RIGHT, UP);
          }
        }

        /* Scroll functionality */
        else if (mouse.part == SCROLLAXIS) {
          HIDinput->setMouseScroll(mouse.value);
        }

        /* X-axis functionality */
        else if (mouse.part == XAXIS) {
          HIDinput->setMouseSpeedX(mouse.value);
        }

        /* Y-axis functionality */
        else if (mouse.part == YAXIS) {
          HIDinput->setMouseSpeedY(mouse.value);
        }
      }//for
    }//mouse
  }//for

  /* Grab Touch Sensor values */
  touch->writeKeys(&glove_data.touch_sensor);

  /* Touch Sensor functionality */
  for (int i = 0; i < TOUCH_COUNT; ++i) {

    /* Keyboard functionality */
    if (touch_sensors[i]->is_keyboard()) {
      keyboardData keyboard = touch_sensors[i]->get_keyboard_data();
      if (keyboard.changed && keyboard.valid) {
        if (keyboard.value) {
          HIDinput->keyPress(keyboard.key);
        }
        else {
          HIDinput->keyRelease(keyboard.key);
        }
      }
    }//keyboard

    /* Mouse functionality */
    else if (touch_sensors[i]->is_mouse()) {
      mouseData mouse = touch_sensors[i]->get_mouse_data(); //Grab mouse data
      if (mouse.changed && mouse.valid) {

        /* Left click */
        if (mouse.part == LBUTTON) {
          if (mouse.value) {
            HIDinput->setMouseButton(LEFT, DOWN);
          }
          else {
            HIDinput->setMouseButton(LEFT, UP);
          }
        }

        /* Right click */
        else if (mouse.part == RBUTTON) {
          if (mouse.value) {
            HIDinput->setMouseButton(RIGHT, DOWN);
          }
          else {
            HIDinput->setMouseButton(RIGHT, UP);
          }
        }

        /* Scroll functionality */
        else if (mouse.part == SCROLLAXIS) {
          HIDinput->setMouseScroll(mouse.value);
        }

        /* X-axis functionality */
        else if (mouse.part == XAXIS) {
          HIDinput->setMouseSpeedX(mouse.value);
        }

        /* Y-axis functionality */
        else if (mouse.part == YAXIS) {
          HIDinput->setMouseSpeedY(mouse.value);
        }
      }//for
    }//mouse
  }//for

  /* Update IMU values */
  imu->updateAndWriteSensors(&glove_data.imu);

  /* IMU functionality */
  for (int i = 0; i < IMU_COUNT; ++i) {

    /* Keyboard functionality */
    if (imu_axis[i]->is_keyboard()) {
      keyboardData keyboard = imu_axis[i]->get_keyboard_data();
      if (keyboard.changed && keyboard.valid) {
        if (keyboard.value) {
          HIDinput->keyPress(keyboard.key);
        }
        else {
          HIDinput->keyRelease(keyboard.key);
        }
      }
    }//keyboard

    /* Mouse functionality */
    else if (imu_axis[i]->is_mouse()) {
      mouseData mouse = imu_axis[i]->get_mouse_data(); //Grab mouse data
      if (mouse.changed && mouse.valid) {

        /* Left click */
        if (mouse.part == LBUTTON) {
          if (mouse.value) {
            HIDinput->setMouseButton(LEFT, DOWN);
          }
          else {
            HIDinput->setMouseButton(LEFT, UP);
          }
        }

        /* Right click */
        else if (mouse.part == RBUTTON) {
          if (mouse.value) {
            HIDinput->setMouseButton(RIGHT, DOWN);
          }
          else {
            HIDinput->setMouseButton(RIGHT, UP);
          }
        }

        /* Scroll functionality */
        else if (mouse.part == SCROLLAXIS) {
          HIDinput->setMouseScroll(mouse.value);
        }

        /* X-axis functionality */
        else if (mouse.part == XAXIS) {
          HIDinput->setMouseSpeedX(mouse.value);
        }

        /* Y-axis functionality */
        else if (mouse.part == YAXIS) {
          HIDinput->setMouseSpeedY(mouse.value);
        }
      }//for
    }//mouse
  }//for

  /* Send HID inputs */
  HIDinput->sendKeyboard();
  HIDinput->sendMouse();

  return;
}

void Translator::startUpdateTask(uint32_t ms) {
  update_task_timer->start(ms);
}
void Translator::stopUpdateTask() {
  update_task_timer->stop();
}
