/*
 * Filename:  translator.h
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
 *  Translator to interpret glove data as HID input
 */

 #ifndef TRANSLATOR_H_
 #define TRANSLATOR_H_

#include "mbed.h"
#include <vector>

enum class GESTURE {
                     FLEX1,
                     FLEX2,
                     FLEX3,
                     FLEX4,
                     TOUCH1,
                     TOUCH2,
                     TOUCH3,
                     TOUCH4,
                     PITCHUP,
                     PITCHDOWN,
                     ROLLLEFT,
                     ROLLRIGHT,
                     YAWLEFT,
                     YAWRIGHT
                   };

class translator {
  std::vector<string> HIDinput;
  std::vector<uint8_t> isPressed;
  *glove_raw_data GLOVE;
public:
  translator(*glove_raw_data gloveptr);

  void updateGestureMap(std::vector<string>* newMap);

  void gestureCheck();
};

 #endif /* TRANSLATOR_H_ */
