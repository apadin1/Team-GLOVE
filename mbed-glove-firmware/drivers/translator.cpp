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

Translator::translator(collector* gloveptr) :gestureHID(GESTURE_COUNT,0), isPressed(GESTURE_COUNT, 0), GLOVE(gloveptr) {
  //TODO: Determine default glove mapping
  //gestureHID[] = { TBD };
}

Translator::updateGestureMap(std::vector<string>* newMap) {
  gestureHID = *newMap;
}

//TODO: Speak with Adrian about HID input
//TODO: Determine what registers as "press" versus "not pressed"
//TODO: Review/Revise updated collector files
Translator::gestureCheck() {
  /* Flex Sensors */
  isPressed[FLEX1] = GLOVE->readFlex(0);
  isPressed[FLEX2] = GLOVE->readFlex(1);
  isPressed[FLEX3] = GLOVE->readFlex(2);
  isPressed[FLEX4] = GLOVE->readFlex(3);

  /* Touch Sensors */
  isPressed[TOUCH1] = GLOVE->readTouch().a;
  isPressed[TOUCH2] = GLOVE->readTouch().b;
  isPressed[TOUCH3] = GLOVE->readTouch().c;
  isPressed[TOUCH4] = GLOVE->readTouch().d;

  /* IMU */
  isPressed[PITCHUP] = 0;
  isPressed[PITCHDOWN] = 0;
  isPressed[ROLLLEFT] = 0;
  isPressed[ROLLRIGHT] = 0;
  isPressed[YAWLEFT] = 0;
  isPressed[YAWRIGHT] = 0;
}
