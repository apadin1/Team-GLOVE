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

Translator::gestureCheck() {
  //check to see which gestures are active
  //relay to HID input
}
