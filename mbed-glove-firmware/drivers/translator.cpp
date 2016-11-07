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

Translator::translator(collector* gloveptr) {
  GLOVE = gloveptr;
  //TODO: Determine default glove mapping
  //gestureHID[] = { TBD };
}

Translator::updateGestureMap(std::vector<string>* newMap) {
  gestureHID = *newMap;
}

//TODO: Speak with Adrian about HID input
//TODO: Determine what registers as "press" versus "not pressed"
Translator::gestureCheck() {
  /* Flex Sensors */
  isPressed[FLEX1] = GLOVE.glove_data.flex_sensors[0];
  isPressed[FLEX2] = GLOVE.glove_data.flex_sensors[1];
  isPressed[FLEX3] = GLOVE.glove_data.flex_sensors[2];
  isPressed[FLEX4] = GLOVE.glove_data.flex_sensors[3];

  /* Touch Sensors */
  isPressed[TOUCH1] = GLOVE.glove_data.keys.a;
  isPressed[TOUCH2] = GLOVE.glove_data.keys.b;
  isPressed[TOUCH3] = GLOVE.glove_data.keys.c;
  isPressed[TOUCH4] = GLOVE.glove_data.keys.d;

  /* IMU */
  isPressed[PITCHUP] = 0;
  isPressed[PITCHDOWN] = 0;
  isPressed[ROLLLEFT] = 0;
  isPressed[ROLLRIGHT] = 0;
  isPressed[YAWLEFT] = 0;
  isPressed[YAWRIGHT] = 0;
}
