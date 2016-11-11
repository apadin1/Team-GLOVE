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

Translator::translator(FlexSensors* _flex, IMU_BNO055* _imu,
        TouchSensor* _touch)
     : flex(_flex), imu(_imu), touch(_touch) {
  //TODO: Determine default glove mapping (Minecraft?)

  /* FLEX1 */
  AnalogButton flex1(&glove_data.flex_sensors[0], 300, 800, 20, false, false);
  sensors.push_back(&flex1);

  /* FLEX2 */
  AnalogButton flex2(&glove_data.flex_sensors[1], 300, 800, 20, false, false);
  sensors.push_back(&flex2);

  /* FLEX3 */
  AnalogButton flex3(&glove_data.flex_sensors[2], 300, 800, 20, false, false);
  sensors.push_back(&flex3);

  /* FLEX4 */
  AnalogButton flex4(&glove_data.flex_sensors[3], 300, 800, 20, false, false);
  sensors.push_back(&flex4);

  /* TOUCH1 */
  AnalogButton touch1(&glove_data.touch_sensor.a);
  sensors.push_back(&touch1);

  /* TOUCH2 */
  AnalogButton touch2(&glove_data.touch_sensor.b);
  sensors.push_back(&touch2);

  /* TOUCH3 */
  AnalogButton touch3(&glove_data.touch_sensor.c);
  sensors.push_back(&touch3);

  /* TOUCH4 */
  AnalogButton touch4(&glove_data.touch_sensor.d);
  sensors.push_back(&touch4);

  /* PITCHUP */
  AnalogButton pitchup(&glove_data.imu.orient_pitch, 0, 15, 20, true, true);
  sensors.push_back(&pitchup);

  /* PITCHDOWN */
  AnalogButton pitchdown(&glove_data.imu.orient_pitch, -15, 0, 20, true, false);
  sensors.push_back(&pitchdown);

  /* ROLLLEFT */
  AnalogButton rollleft(&glove_data.imu.orient_roll, 0, 20, 20, true, true);
  sensors.push_back(&rollleft);

  /* ROLLRIGHT */
  AnalogButton rollright(&glove_data.imu.orient_roll, -20, 0, 20, true, false);
  sensors.push_back(&rollright);

  /* YAWLEFT */
  AnalogButton yawleft(&glove_data.imu.orient_yaw, 0, 20, 20, true, true);
  sensors.push_back(&yawleft);

  /* YAWRIGHT */
  AnalogButton yawright(&glove_data.imu.orient_yaw, -20, 0, 0, true, false);
  sensors.push_back(&yawright);

  /* BUTTON MAPPING */
  //AnalogButton[FLEX1].change_hid_profile();
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

  update_task_timer = new RtosTimer(this, &Collector::update, osTimerPeriodic);
}

//TODO: Determine FINAL process w/ transciever
//void Translator::updateGestureMap(std::vector<AnalogButton>* updatedMapping) {
   //sensors = *updatedMapping;
//}

void Translator::gestureCheck() {
  working = 1;

  /* Update Sensor Data */
  flex->writeSensors(glove_data.flex_sensors);
  touch->writeKeys(&glove_data.touch_sensor);
  imu->writeSensors(&glove_data.imu);

  for (int i = 0; i < GESTURE_COUNT; ++i) {

    /* Keyboard functionality */
    if (sensors[i].is_keyboard()) {
      keyboardData keyboard = sensors[i].get_keyboard_data();
      if (keyboard.changed && keyboard.valid) {
        if (keyboard.value) {
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
          if (mouse.value) {
            HIDinput.setMouseButton(LEFT, DOWN);
          }
          else {
            HIDinput.setMouseButton(LEFT, UP);
          }
        }

        /* Right click */
        else if (mouse.part == RBUTTON) {
          if (mouse.value) {
            HIDinput.setMouseButton(RIGHT, DOWN);
          }
          else {
            HIDinput.setMouseButton(RIGHT, UP);
          }
        }

        /* Scroll functionality */
        else if (mouse.part == SCROLL) {
          HIDinput.setMouseScroll(mouse.value);
        }

        /* X-axis functionality */
        else if (mouse.part == XAXIS) {
          HIDinput.setMouseSpeedX(mouse.value);
        }

        /* Y-axis functionality */
        else if (mouse.part == YAXIS) {
          HIDinput.setMouseSpeedY(mouse.value);
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
