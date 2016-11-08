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

#include "analog_button.h"
#include "keyboard_mouse.h"
#include "mbed.h"
#include <vector>

#define GESTURE_COUNT 14

/*
 * Update Period (in milliseconds)
 */
const uint32_t COLLECTOR_UPDATE_PERIOD = 10;

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
   * Constructor for translator
   *
   */
  Translator();

  /*
   * Update gesture mapping via new configuration vector.
   * Transciever to send the new Vector to bluetooth class,
   * which should then call this function
   */
  void updateGestureMap(std::vector<AnalogButton>* updatedMapping);

  /*
   * Analyze sensors to determine if gesture
   * is occuring. If so, generate proper HID data to be sent to HID class.
   * This function designed to be set up as a periodic task.
   */
  void gestureCheck();

  /*
   * Calls the start() method on the periodic update task,
   * an internal timer is set up in the constructor
   */
  void startUpdateTask(uint32_t ms=COLLECTOR_UPDATE_PERIOD);

  /*
   * Calls the stop() method on the periodic update timer,
   */
  void stopUpdateTask();

private:
  //NOTE: Vector indexed by GESTURE enum
  std::vector<AnalogButton> sensors; //Sensor data/HID mapping
  KeyboardMouse HIDinput; //KeyboardMouse object
  RtosTimer* update_task_timer;
  DigitalOut working;
};

#endif /* TRANSLATOR_H_ */