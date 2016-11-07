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
#include <cstdint>
#include <string>

//TODO: Include #defines for sensor analysis
#define GESTURE_COUNT 14
/* GESTURE
 *
 * enum used to index vectors contained
 * within Translator class. Also used to
 * provide consistency between data structures
 * used both in gloves and transciever
 */
enum GESTURE : uint8_t {
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
   * Constructor for translator. Takes pointer to collector type
   * for access to the glove_raw data.
   *
   */
  Translator(collector* gloveptr);
  /*
   * Update gesture mapping via new configuration vector.
   * Transciever to send the new Vector to bluetooth class,
   * which should then call this function
   */
  void updateGestureMap(std::vector<string>* newMap);

  /*
   * Analyze current glove_raw data to determine if gesture
   * is occuring. If so, generate proper HID data to be sent to HID class.
   * This function designed to be set up as a periodic task.
   */
  void gestureCheck();

private:
  //NOTE: Vectors indexed by GESTURE enum
  std::vector<string> gestureHID; //Contains gesture to HID mapping
  std::vector<uint8_t> isPressed; //Contains HID input
  collector* GLOVE; //Pointer to collector instance
};

#endif /* TRANSLATOR_H_ */
