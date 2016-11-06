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

translator::translator(*glove_raw_data gloveptr) {
  GLOVE = gloveptr;
}

translator::updateGestureMap(std::vector<string>* newMap) {
  HIDinput = *newMap;
}

translator::gestureCheck() {
  //check to see which gestures are active
  //relay to HID input
}
