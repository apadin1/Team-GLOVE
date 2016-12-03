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

Translator::Translator(glove_sensors_raw_t* _left, glove_sensors_raw_t* _right,
                       KeyboardMouse* _input)
    : glove_dataL(_left), glove_dataR(_right), HIDinput(_input) {

    /* Left Glove Setup */

    /* FLEX1 */
    flexToHID flex1L(&glove_dataL->flex_sensors[0], 300, 800,
                                      0.05, true);
    flex_sensorsL[FLEX1] = &flex1L;

    /* FLEX2 */
    flexToHID flex2L(&glove_dataL->flex_sensors[1], 300, 800, 20,
                                      false);
    flex_sensorsL[FLEX2] = &flex2L;

    /* FLEX3 */
    flexToHID flex3L(&glove_dataL->flex_sensors[2], 300, 800, 20,
                                      false);
    flex_sensorsL[FLEX3] = &flex3L;

    /* FLEX4 */
    flexToHID flex4L(&glove_dataL->flex_sensors[3], 300, 800, 20,
                                      false);
    flex_sensorsL[FLEX4] = &flex4L;

    /* TOUCH1 */
    touchToHID touch1L(&glove_dataL->touch_sensor.a);
    touch_sensorsL[TOUCH1] = &touch1L;

    /* TOUCH2 */
    touchToHID touch2L(&glove_dataL->touch_sensor.b);
    touch_sensorsL[TOUCH2] = &touch2L;

    /* TOUCH3 */
    touchToHID touch3L(&glove_dataL->touch_sensor.c);
    touch_sensorsL[TOUCH3] = &touch3L;

    /* TOUCH4 */
    touchToHID touch4L(&glove_dataL->touch_sensor.d);
    touch_sensorsL[TOUCH4] = &touch4L;

    /* PITCHUP */
    imuToHID pitchupL(&glove_dataL->imu.orient_pitch, 0, 35, 0.15, false);
    imu_axisL[PITCHUP] = &pitchupL;

    /* PITCHDOWN */
    imuToHID pitchdownL(&glove_dataL->imu.orient_pitch, -35, 0, 0.15, true);
    imu_axisL[PITCHDOWN] = &pitchdownL;

    /* ROLLLEFT */
    imuToHID rollleftL(&glove_dataL->imu.orient_roll, 0, 45, 0.15, true);
    imu_axisL[ROLLLEFT] = &rollleftL;

    /* ROLLRIGHT */
    imuToHID rollrightL(&glove_dataL->imu.orient_roll, -45, 0, 0.15, false);
    imu_axisL[ROLLRIGHT] = &rollrightL;

    /* YAWLEFT */
    imuToHID yawleftL(&glove_dataL->imu.orient_yaw, 0, 20, 0.2, true);
    imu_axisL[YAWLEFT] = &yawleftL;

    /* YAWRIGHT */
    imuToHID yawrightL(&glove_dataL->imu.orient_yaw, -20, 0, 0.2, false);
    imu_axisL[YAWRIGHT] = &yawrightL;

    /* Right Glove Setup */

    /* FLEX1 */
    //flexToHID flex1R(&glove_dataR->flex_sensors[0], 300, 800,
    //                                  0.05, true);
    //flex_sensorsR[FLEX1] = &flex1R;

    /* FLEX2 */
    //flexToHID flex2R(&glove_dataR->flex_sensors[1], 300, 800, 20,
    //                                  false);
    //flex_sensorsR[FLEX2] = &flex2R;

    /* FLEX3 */
    //flexToHID flex3R(&glove_dataR->flex_sensors[2], 300, 800, 20,
    //                                  false);
    //flex_sensorsR[FLEX3] = &flex3R;

    /* FLEX4 */
    //flexToHID flex4R(&glove_dataR->flex_sensors[3], 300, 800, 20,
    //                                  false);
    //flex_sensorsR[FLEX4] = &flex4R;

    /* TOUCH1 */
    //touchToHID touch1R(&glove_dataR->touch_sensor.a);
    //touch_sensorsR[TOUCH1] = &touch1R;

    /* TOUCH2 */
    //touchToHID touch2R(&glove_dataR->touch_sensor.b);
    //touch_sensorsR[TOUCH2] = &touch2R;

    /* TOUCH3 */
    //touchToHID touch3R(&glove_dataR->touch_sensor.c);
    //touch_sensorsR[TOUCH3] = &touch3R;

    /* TOUCH4 */
    //touchToHID touch4R(&glove_dataR->touch_sensor.d);
    //touch_sensorsR[TOUCH4] = &touch4R;

    /* PITCHUP */
    //imuToHID pitchupR(&glove_dataR->imu.orient_pitch, 0, 35, 0.15, false);
    //imu_axisR[PITCHUP] = &pitchupR;

    /* PITCHDOWN */
    //imuToHID pitchdownR(&glove_dataR->imu.orient_pitch, -35, 0, 0.15, true);
    //imu_axisR[PITCHDOWN] = &pitchdownR;

    /* ROLLLEFT */
    //imuToHID rollleftR(&glove_dataR->imu.orient_roll, 0, 45, 0.15, true);
    //imu_axisR[ROLLLEFT] = &rollleftR;

    /* ROLLRIGHT */
    //imuToHID rollrightR(&glove_dataR->imu.orient_roll, -45, 0, 0.15, false);
    //imu_axisR[ROLLRIGHT] = &rollrightR;

    /* YAWLEFT */
    //imuToHID yawleftR(&glove_dataR->imu.orient_yaw, 0, 20, 0.2, true);
    //imu_axisR[YAWLEFT] = &yawleftR;

    /* YAWRIGHT */
    //imuToHID yawrightR(&glove_dataR->imu.orient_yaw, -20, 0, 0.2, false);
    //imu_axisR[YAWRIGHT] = &yawrightR;

    /* BUTTON MAPPING (Left Glove) */
    flex_sensorsL[FLEX1]->change_hid_profile(DISABLED);
    flex_sensorsL[FLEX2]->change_hid_profile(DISABLED);
    flex_sensorsL[FLEX3]->change_hid_profile(DISABLED);
    flex_sensorsL[FLEX4]->change_hid_profile(DISABLED);
    touch_sensorsL[TOUCH1]->change_hid_profile(DISABLED);
    touch_sensorsL[TOUCH2]->change_hid_profile(DISABLED);
    touch_sensorsL[TOUCH3]->change_hid_profile(DISABLED);
    touch_sensorsL[TOUCH4]->change_hid_profile(DISABLED);
    imu_axisL[PITCHUP]->change_hid_profile(DISABLED);
    imu_axisL[PITCHDOWN]->change_hid_profile(DISABLED);
    imu_axisL[ROLLLEFT]->change_hid_profile(DISABLED);
    imu_axisL[ROLLRIGHT]->change_hid_profile(DISABLED);
    imu_axisL[YAWLEFT]->change_hid_profile(DISABLED);
    imu_axisL[YAWRIGHT]->change_hid_profile(DISABLED);


    /* BUTTON MAPPING (Right Glove) */
    /*flex_sensorsR[FLEX1]->change_hid_profile(DISABLED);
    flex_sensorsR[FLEX2]->change_hid_profile(DISABLED);
    flex_sensorsR[FLEX3]->change_hid_profile(DISABLED);
    flex_sensorsR[FLEX4]->change_hid_profile(DISABLED);
    touch_sensorsR[TOUCH1]->change_hid_profile(KEYBOARD, 'a');
    touch_sensorsR[TOUCH2]->change_hid_profile(DISABLED);
    touch_sensorsR[TOUCH3]->change_hid_profile(DISABLED);
    touch_sensorsR[TOUCH4]->change_hid_profile(DISABLED);
    imu_axisR[PITCHUP]->change_hid_profile(DISABLED);
    imu_axisR[PITCHDOWN]->change_hid_profile(DISABLED);
    imu_axisR[ROLLLEFT]->change_hid_profile(DISABLED);
    imu_axisR[ROLLRIGHT]->change_hid_profile(DISABLED);
    imu_axisR[YAWLEFT]->change_hid_profile(DISABLED);
    imu_axisR[YAWRIGHT]->change_hid_profile(DISABLED);*/
}

void Translator::updateGestureMap(uint8_t* config) {

  /* Left Glove Configuration */

  /* Flex Sensor Configuration */
  for (int i = 0; i < FLEX_COUNT; ++i) {
    if (config[i] == 0) {
      flex_sensorsL[i]->change_hid_profile(DISABLED);
    }
    else if (config[i] < 200) {
      flex_sensorsL[i]->change_hid_profile(KEYBOARD, config[i]);
    }
    else {
      flex_sensorsL[i]->change_hid_profile(MOUSE, config[i]);
    }
  }

  /* Touch Sensor Configuration */
  for (int i = 4; i < TOUCH_COUNT + 4; ++i) {
    if (config[i] == 0) {
      touch_sensorsL[i-4]->change_hid_profile(DISABLED);
    }
    else if (config[i] < 200) {
      touch_sensorsL[i-4]->change_hid_profile(KEYBOARD, config[i]);
    }
    else {
      touch_sensorsL[i-4]->change_hid_profile(MOUSE, config[i]);
    }
  }

  /* IMU Configuration */
  for (int i = 8; i < IMU_COUNT + 8; ++i) {
    if (config[i] == 0) {
      imu_axisL[i-8]->change_hid_profile(DISABLED);
    }
    else if (config[i] < 200) {
      imu_axisL[i-8]->change_hid_profile(KEYBOARD, config[i]);
    }
    else {
      imu_axisL[i-8]->change_hid_profile(MOUSE, config[i]);
    }
  }

  /* Right Glove Configuration */

  /* Flex Sensor Configuration */
  //for (int i = 14; i < FLEX_COUNT + 14; ++i) {
    //if (config[i] == 0) {
      //flex_sensorsR[i-14]->change_hid_profile(DISABLED);
    //}
    //else if (config[i] < 200) {
      //flex_sensorsR[i-14]->change_hid_profile(KEYBOARD, config[i]);
    //}
    //else {
      //flex_sensorsR[i-14]->change_hid_profile(MOUSE, config[i]);
    //}
  //}

  /* Touch Sensor Configuration */
  //for (int i = 18; i < TOUCH_COUNT + 18; ++i) {
    //if (config[i] == 0) {
      //touch_sensorsR[i-18]->change_hid_profile(DISABLED);
    //}
    //else if (config[i] < 200) {
      //touch_sensorsR[i-18]->change_hid_profile(KEYBOARD, config[i]);
    //}
    //else {
      //touch_sensorsR[i-18]->change_hid_profile(MOUSE, config[i]);
    //}
  //}

  /* IMU Configuration */
  //for (int i = 22; i < IMU_COUNT + 22; ++i) {
    //if (config[i] == 0) {
      //imu_axisR[i-22]->change_hid_profile(DISABLED);
    //}
    //else if (config[i] < 200) {
      //imu_axisR[i-22]->change_hid_profile(KEYBOARD, config[i]);
    //}
    //else {
      //imu_axisR[i-22]->change_hid_profile(MOUSE, config[i]);
    //}
  //}
}

void Translator::gestureCheck() {

    // Check if keyboard is connected before sending
    if (!HIDinput->isConnected()) {
        return;
    }

    /* Left Glove Functionality */

    /* Flex Sensor functionality */
    for (int i = 0; i < FLEX_COUNT; ++i) {

      /* Keyboard functionality */
      if (flex_sensorsL[i]->is_keyboard()) {
          keyboardData keyboard = flex_sensorsL[i]->get_keyboard_data();
          if (keyboard.valid) {
              if (keyboard.value) {
                  HIDinput->keyPress(keyboard.key);
              } else {
                  HIDinput->keyRelease(keyboard.key);
              }
          }
      }  // keyboard

      /* Mouse functionality */
      else if (flex_sensorsL[i]->is_mouse()) {
          mouseData mouse =
                flex_sensorsL[i]->get_mouse_data();  // Grab mouse data
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
        if (touch_sensorsL[i]->is_keyboard()) {
            keyboardData keyboard = touch_sensorsL[i]->get_keyboard_data();
            if (keyboard.valid) {
                if (keyboard.value) {
                    HIDinput->keyPress(keyboard.key);
                } else {
                    HIDinput->keyRelease(keyboard.key);
                }
            }
        }  // keyboard

        /* Mouse functionality */
        else if (touch_sensorsL[i]->is_mouse()) {
            mouseData mouse =
                  touch_sensorsL[i]->get_mouse_data();  // Grab mouse data
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

    /* IMU functionality */
    for (int i = 0; i < 4; i++) {
        /* Keyboard functionality */

       //uint32_t i = 0;//DEBUG
       if (imu_axisL[i]->is_keyboard()) {
         keyboardData keyboard = imu_axisL[i]->get_keyboard_data();
            if (keyboard.valid) {
                if (keyboard.value) {
                    HIDinput->keyPress(keyboard.key);
                } else {
                    HIDinput->keyRelease(keyboard.key);
                }
            }
        }  // keyboard
    }//FOR

//        /* Mouse functionality */
//        else if (imu_axisL[i]->is_mouse()) {
//            mouseData mouse = imu_axis[i]->get_mouse_data();  // Grab mouse data
//            if (mouse.valid) {
//
//                /* Left click */
//                if (mouse.part == LBUTTON) {
//                    if (mouse.value) {
//                        HIDinput->setMouseButton(LEFT, DOWN);
//                    } else {
//                        HIDinput->setMouseButton(LEFT, UP);
//                   }
//                }
//
//                /* Right click */
//                else if (mouse.part == RBUTTON) {
//                    if (mouse.value) {
//                        HIDinput->setMouseButton(RIGHT, DOWN);
//                    } else {
//                        HIDinput->setMouseButton(RIGHT, UP);
//                    }
//                }

                  /* Middle click */
//                else if (mouse.part == MIDDLECLICK) {
//                    if (mouse.value) {
//                        HIDinput->setMouseButton(MIDDLE, DOWN);
//                    } else {
//                        HIDinput->setMouseButton(MIDDLE, UP);
//                    }
//                }
//
//                /* Scroll functionality */
//                else if (mouse.part == SCROLLAXIS) {
//                    HIDinput->setMouseScroll(mouse.value);
//                }
//
//                /* X-axis functionality */
//                else if (mouse.part == XAXIS) {
//                    HIDinput->setMouseSpeedX(mouse.value);
//                }
//
//                /* Y-axis functionality */
//                else if (mouse.part == YAXIS) {
//                    HIDinput->setMouseSpeedY(mouse.value);
//                }
//            }  // if
//        }  // mouse
//    }  // for

    /* Right Glove Functionality */

    /* Flex Sensor functionality */
    //for (int i = 0; i < FLEX_COUNT; ++i) {

      /* Keyboard functionality */
      //if (flex_sensorsR[i]->is_keyboard()) {
          //keyboardData keyboard = flex_sensorsR[i]->get_keyboard_data();
          //if (keyboard.valid) {
              //if (keyboard.value) {
                  //HIDinput->keyPress(keyboard.key);
              //} else {
                  //HIDinput->keyRelease(keyboard.key);
              //}
          //}
      //}  // keyboard

      /* Mouse functionality */
      //else if (flex_sensorsR[i]->is_mouse()) {
          //mouseData mouse =
                //flex_sensorsR[i]->get_mouse_data();  // Grab mouse data
          //if (mouse.valid) {

              /* Left click */
              //if (mouse.part == LBUTTON) {
                  //if (mouse.value) {
                      //HIDinput->setMouseButton(LEFT, DOWN);
                  //} else {
                      //HIDinput->setMouseButton(LEFT, UP);
                  //}
              //}

              /* Right click */
              //else if (mouse.part == RBUTTON) {
                  //if (mouse.value) {
                      //HIDinput->setMouseButton(RIGHT, DOWN);
                  //} else {
                      //HIDinput->setMouseButton(RIGHT, UP);
                  //}
              //}

              /* Middle click */
              //else if (mouse.part == MIDDLECLICK) {
                //if (mouse.value) {
                    //HIDinput->setMouseButton(MIDDLE, DOWN);
                //} else {
                    //HIDinput->setMouseButton(MIDDLE, UP);
                //}
              //}

              /* Scroll functionality */
              //else if (mouse.part == SCROLLAXIS) {
                  //HIDinput->setMouseScroll(mouse.value);
              //}

              /* X-axis functionality */
              //else if (mouse.part == XAXIS) {
                  //HIDinput->setMouseSpeedX(mouse.value);
              //}

                /* Y-axis functionality */
                //else if (mouse.part == YAXIS) {
                    //HIDinput->setMouseSpeedY(mouse.value);
                //}
            //}  // for
        //}  // mouse
    //}  // for

    /* Touch Sensor functionality */
    //for (int i = 0; i < TOUCH_COUNT; ++i) {

        /* Keyboard functionality */
        //if (touch_sensorsR[i]->is_keyboard()) {
            //keyboardData keyboard = touch_sensorsR[i]->get_keyboard_data();
            //if (keyboard.valid) {
                //if (keyboard.value) {
                    //HIDinput->keyPress(keyboard.key);
                //} else {
                    //HIDinput->keyRelease(keyboard.key);
                //}
            //}
        //}  // keyboard

        /* Mouse functionality */
        //else if (touch_sensorsR[i]->is_mouse()) {
            //mouseData mouse =
                  //touch_sensorsR[i]->get_mouse_data();  // Grab mouse data
            //if (mouse.valid) {

                /* Left click */
                //if (mouse.part == LBUTTON) {
                    //if (mouse.value) {
                        //HIDinput->setMouseButton(LEFT, DOWN);
                    //} else {
                        //HIDinput->setMouseButton(LEFT, UP);
                    //}
                //}

                /* Right click */
                //else if (mouse.part == RBUTTON) {
                    //if (mouse.value) {
                        //HIDinput->setMouseButton(RIGHT, DOWN);
                    //} else {
                        //HIDinput->setMouseButton(RIGHT, UP);
                    //}
                //}

                /* Middle click */
                //else if (mouse.part == MIDDLECLICK) {
                  //if (mouse.value) {
                      //HIDinput->setMouseButton(MIDDLE, DOWN);
                  //} else {
                      //HIDinput->setMouseButton(MIDDLE, UP);
                  //}
                //}

                /* Scroll functionality */
                //else if (mouse.part == SCROLLAXIS) {
                    //HIDinput->setMouseScroll(mouse.value);
                //}

                /* X-axis functionality */
                //else if (mouse.part == XAXIS) {
                    //HIDinput->setMouseSpeedX(mouse.value);
                //}

                /* Y-axis functionality */
                //else if (mouse.part == YAXIS) {
                    //HIDinput->setMouseSpeedY(mouse.value);
                //}
            //}  // for
        //}  // mouse
    //}  // for

    /* IMU functionality */
    //for (int i = 0; i < 4; i++) {
        /* Keyboard functionality */

       //uint32_t i = 0;//DEBUG
       //if (imu_axisR[i]->is_keyboard()) {
         //keyboardData keyboard = imu_axisR[i]->get_keyboard_data();
            //if (keyboard.valid) {
                //if (keyboard.value) {
                    //HIDinput->keyPress(keyboard.key);
                //} else {
                    //HIDinput->keyRelease(keyboard.key);
                //}
            //}
        //}  // keyboard
    //}//FOR

//        /* Mouse functionality */
//        else if (imu_axisR[i]->is_mouse()) {
//            mouseData mouse = imu_axisR[i]->get_mouse_data();  // Grab mouse data
//            if (mouse.valid) {
//
//                /* Left click */
//                if (mouse.part == LBUTTON) {
//                    if (mouse.value) {
//                        HIDinput->setMouseButton(LEFT, DOWN);
//                    } else {
//                        HIDinput->setMouseButton(LEFT, UP);
//                   }
//                }
//
//                /* Right click */
//                else if (mouse.part == RBUTTON) {
//                    if (mouse.value) {
//                        HIDinput->setMouseButton(RIGHT, DOWN);
//                    } else {
//                        HIDinput->setMouseButton(RIGHT, UP);
//                    }
//                }

                  /* Middle click */
//                else if (mouse.part == MIDDLECLICK) {
//                    if (mouse.value) {
//                        HIDinput->setMouseButton(MIDDLE, DOWN);
//                    } else {
//                        HIDinput->setMouseButton(MIDDLE, UP);
//                    }
//                }
//
//                /* Scroll functionality */
//                else if (mouse.part == SCROLLAXIS) {
//                    HIDinput->setMouseScroll(mouse.value);
//                }
//
//                /* X-axis functionality */
//                else if (mouse.part == XAXIS) {
//                    HIDinput->setMouseSpeedX(mouse.value);
//                }
//
//                /* Y-axis functionality */
//                else if (mouse.part == YAXIS) {
//                    HIDinput->setMouseSpeedY(mouse.value);
//                }
//            }  // if
//        }  // mouse
//    }  // for

    /* Send HID inputs */
    HIDinput->sendKeyboard();
    HIDinput->sendMouse();

    //return;
}


