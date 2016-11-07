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

//TODO: Include #defines for sensor analysis

/* GESTURE
 *
 * enum used to index vectors contained
 * within Translator class. Also used to
 * provide consistency between data structures
 * used both in gloves and transciever
 */
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

/* Translator
 *
 * Single class to handle data interpretation between
 * glove data and HID input. Methods are written
 * to update internally, and to write all methods into a
 * data structure
 */
class Translator {
public:
  /*
   * Constructor for collector
   */
  Translator(collector* gloveptr);

  /*
   * Update gesture mapping via new configuration vector
   */
  void updateGestureMap(std::vector<string>* newMap);

  /*
   * Analyze collector data, and send HID input
   */
  void gestureCheck();

private:
  //NOTE: Vectors indexed by GESTURE enum
  std::vector<string> gestureHID; //Contains gesture to HID mapping
  std::vector<uint8_t> isPressed; //Contains HID input
  //TODO: Determine best way to access raw collector data
  collector* GLOVE; //Pointer to collector instance
};

#endif /* TRANSLATOR_H_ */
