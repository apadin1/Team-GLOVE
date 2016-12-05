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

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

Translator::Translator(glove_sensors_raw_t* _glove, KeyboardMouse* _input)
    : glove_data(_glove), HIDinput(_input), working(TRANSLATOR_DEBUG_PIN) {

    /* Left Glove Setup */

    /* FLEX1 */
    flexToHID flex1(&(glove_data->flex_sensors[0]), 300, 800,
                                      0.05, true);
    flex_sensors[FLEX1] = &flex1;

    /* FLEX2 */
    flexToHID flex2(&(glove_data->flex_sensors[1]), 300, 800, 20,
                                      false);
    flex_sensors[FLEX2] = &flex2;

    /* FLEX3 */
    flexToHID flex3(&(glove_data->flex_sensors[2]), 300, 800, 20,
                                      false);
    flex_sensors[FLEX3] = &flex3;

    /* FLEX4 */
    flexToHID flex4(&(glove_data->flex_sensors[3]), 300, 800, 20,
                                      false);
    flex_sensors[FLEX4] = &flex4;

    /* TOUCH1 */
    touchToHID touch1(&(glove_data->touch_sensor.a));
    touch_sensors[TOUCH1] = &touch1;

    /* TOUCH2 */
    touchToHID touch2(&(glove_data->touch_sensor.b));
    touch_sensors[TOUCH2] = &touch2;

    /* TOUCH3 */
    touchToHID touch3(&(glove_data->touch_sensor.c));
    touch_sensors[TOUCH3] = &touch3;

    /* TOUCH4 */
    touchToHID touch4(&(glove_data->touch_sensor.d));
    touch_sensors[TOUCH4] = &touch4;

    /* PITCHUP */
    imuToHID pitchup(&(glove_data->imu.orient_pitch), 0, 35, 0.15, false);
    imu_axis[PITCHUP] = &pitchup;

    /* PITCHDOWN */
    imuToHID pitchdown(&(glove_data->imu.orient_pitch), -35, 0, 0.15, true);
    imu_axis[PITCHDOWN] = &pitchdown;

    /* ROLLLEFT */
    imuToHID rollleft(&(glove_data->imu.orient_roll), 0, 45, 0.15, true);
    imu_axis[ROLLLEFT] = &rollleft;

    /* ROLLRIGHT */
    imuToHID rollright(&(glove_data->imu.orient_roll), -45, 0, 0.15, false);
    imu_axis[ROLLRIGHT] = &rollright;

    /* YAWLEFT */
    imuToHID yawleft(&(glove_data->imu.orient_yaw), 0, 20, 0.2, true);
    imu_axis[YAWLEFT] = &yawleft;

    /* YAWRIGHT */
    imuToHID yawright(&(glove_data->imu.orient_yaw), -20, 0, 0.2, false);
    imu_axis[YAWRIGHT] = &yawright;

    /* BUTTON MAPPING */
    flex_sensors[FLEX1]->change_hid_profile(DISABLED);
    flex_sensors[FLEX2]->change_hid_profile(DISABLED);
    flex_sensors[FLEX3]->change_hid_profile(DISABLED);
    flex_sensors[FLEX4]->change_hid_profile(DISABLED);
    touch_sensors[TOUCH1]->change_hid_profile(KEYBOARD, 'b');
    touch_sensors[TOUCH2]->change_hid_profile(DISABLED);
    touch_sensors[TOUCH3]->change_hid_profile(DISABLED);
    touch_sensors[TOUCH4]->change_hid_profile(DISABLED);
    imu_axis[PITCHUP]->change_hid_profile(DISABLED);
    imu_axis[PITCHDOWN]->change_hid_profile(DISABLED);
    imu_axis[ROLLLEFT]->change_hid_profile(DISABLED);
    imu_axis[ROLLRIGHT]->change_hid_profile(DISABLED);
    imu_axis[YAWLEFT]->change_hid_profile(DISABLED);
    imu_axis[YAWRIGHT]->change_hid_profile(DISABLED);
}

//TODO: Change protocol
void Translator::updateGestureMap(uint8_t* config) {

  /* Left Glove Configuration */

  /* Flex Sensor Configuration */
  for (int i = 0; i < FLEX_COUNT; ++i) {
    if (config[i] == 0) {
      flex_sensors[i]->change_hid_profile(DISABLED);
    }
    else if (config[i] < 200) {
      flex_sensors[i]->change_hid_profile(KEYBOARD, config[i]);
    }
    else {
      flex_sensors[i]->change_hid_profile(MOUSE, config[i]);
    }
  }

  /* Touch Sensor Configuration */
  for (int i = 4; i < TOUCH_COUNT + 4; ++i) {
    if (config[i] == 0) {
      touch_sensors[i-4]->change_hid_profile(DISABLED);
    }
    else if (config[i] < 200) {
      touch_sensors[i-4]->change_hid_profile(KEYBOARD, config[i]);
    }
    else {
      touch_sensors[i-4]->change_hid_profile(MOUSE, config[i]);
    }
  }

  /* IMU Configuration */
  for (int i = 8; i < IMU_COUNT + 8; ++i) {
    if (config[i] == 0) {
      imu_axis[i-8]->change_hid_profile(DISABLED);
    }
    else if (config[i] < 200) {
      imu_axis[i-8]->change_hid_profile(KEYBOARD, config[i]);
    }
    else {
      imu_axis[i-8]->change_hid_profile(MOUSE, config[i]);
    }
  }
}

void Translator::gestureCheck() {

    //if (!HIDinput->isConnected()) {
        //return;
    //}

    /* Glove Functionality */

    /* Flex Sensor functionality */
    for (int i = 0; i < FLEX_COUNT; ++i) {

      /* Keyboard functionality */
      if (flex_sensors[i]->is_keyboard()) {
          keyboardData keyboard = flex_sensors[i]->get_keyboard_data();
          if (keyboard.valid) {
              if (keyboard.value) {
                  HIDinput->keyPress(keyboard.key);
              } else {
                  HIDinput->keyRelease(keyboard.key);
              }
          }
      }  // keyboard

      /* Mouse functionality */
      else if (flex_sensors[i]->is_mouse()) {
          mouseData mouse =
                flex_sensors[i]->get_mouse_data();  // Grab mouse data
          if (mouse.valid) {

              /* Left click */
              if (mouse.part == LBUTTON) {
                  if (mouse.value) {
                      HIDinput->setMouseButton(LEFT, DOWN);
                  } else {
                      HIDinput->setMouseButton(LEFT, UP);
                  }
              }
              /* Right click */
              else if (mouse.part == RBUTTON) {
                  if (mouse.value) {
                      HIDinput->setMouseButton(RIGHT, DOWN);
                  } else {
                      HIDinput->setMouseButton(RIGHT, UP);
                  }
              }

              /* Middle click */
              else if (mouse.part == MIDDLECLICK) {
                if (mouse.value) {
                    HIDinput->setMouseButton(MIDDLE, DOWN);
                } else {
                    HIDinput->setMouseButton(MIDDLE, UP);
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
            }  // for
        }  // mouse
    }  // for

    /* Touch Sensor functionality */
    for (int i = 0; i < TOUCH_COUNT; ++i) {
        /* Keyboard functionality */
        if (touch_sensors[i]->is_keyboard()) {
            keyboardData keyboard = touch_sensors[i]->get_keyboard_data();
            if (keyboard.valid) {
                if (keyboard.value) {
                    HIDinput->keyPress(keyboard.key);
                } else {
                    HIDinput->keyRelease(keyboard.key);
                }
            }

        /* Mouse functionality */
        else if (touch_sensors[i]->is_mouse()) {
            mouseData mouse =
                  touch_sensors[i]->get_mouse_data();  // Grab mouse data
            if (mouse.valid) {

                /* Left click */
                if (mouse.part == LBUTTON) {
                    if (mouse.value) {
                        HIDinput->setMouseButton(LEFT, DOWN);
                    } else {
                        HIDinput->setMouseButton(LEFT, UP);
                    }
                }

                /* Right click */
                else if (mouse.part == RBUTTON) {
                    if (mouse.value) {
                        HIDinput->setMouseButton(RIGHT, DOWN);
                    } else {
                        HIDinput->setMouseButton(RIGHT, UP);
                    }
                }

                /* Middle click */
                else if (mouse.part == MIDDLECLICK) {
                  if (mouse.value) {
                      HIDinput->setMouseButton(MIDDLE, DOWN);
                  } else {
                      HIDinput->setMouseButton(MIDDLE, UP);
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
            }
          }  // mouse
        }
      }  // for

    /* IMU functionality */
    for (int i = 0; i < 4; i++) {

      /* Keyboard functionality */
      if (imu_axis[i]->is_keyboard()) {
         keyboardData keyboard = imu_axis[i]->get_keyboard_data();
            if (keyboard.valid) {
                if (keyboard.value) {
                    HIDinput->keyPress(keyboard.key);
                } else {
                    HIDinput->keyRelease(keyboard.key);
                }
            }
        }  // keyboard

        /* Mouse functionality */
        else if (imu_axis[i]->is_mouse()) {
            mouseData mouse = imu_axis[i]->get_mouse_data();  // Grab mouse data
            if (mouse.valid) {

                /* Left click */
                if (mouse.part == LBUTTON) {
                    if (mouse.value) {
                        HIDinput->setMouseButton(LEFT, DOWN);
                    } else {
                        HIDinput->setMouseButton(LEFT, UP);
                   }
                }

                /* Right click */
                else if (mouse.part == RBUTTON) {
                    if (mouse.value) {
                        HIDinput->setMouseButton(RIGHT, DOWN);
                    } else {
                        HIDinput->setMouseButton(RIGHT, UP);
                    }
                }

                  /* Middle click */
                else if (mouse.part == MIDDLECLICK) {
                    if (mouse.value) {
                        HIDinput->setMouseButton(MIDDLE, DOWN);
                    } else {
                        HIDinput->setMouseButton(MIDDLE, UP);
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
            }  // if
        }  // mouse
    }  // for

    /* Send HID inputs */
    HIDinput->sendKeyboard();
    HIDinput->waitForEvent();

    //HIDinput->sendMouse();
    //HIDinput->waitForEvent();

    return;
}
