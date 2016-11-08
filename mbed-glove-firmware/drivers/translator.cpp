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

Translator::translator(FlexSensors* _flex, IMU_BNO055* _imu, TouchSensor* _touch)
: flex(_flex), imu(_imu), touch(_touch) {
  //TODO: Determine default glove mapping
  //gestureHID[] = { TBD };

  update_task_timer = new RtosTimer(this, &Collector::update, osTimerPeriodic);
}

Translator::updateGestureMap(std::vector<string>* newMap) {
  gestureHID = *newMap;
}

//TODO: Speak with Adrian about HID input
//TODO: Determine what registers as "press" versus "not pressed"
//TODO: Review/Revise updated collector files
Translator::gestureCheck() {
  working = 1;

  /* Update Sensors */
  flex->writeSensors(glove_data.flex_sensors);
  touch->writeKeys(&glove_data.touch_sensor);
  imu->writeSensors(&glove_data.imu);

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

  //Evaluate data and output to HID

  working = 0;
}

void Translator::startUpdateTask(uint32_t ms) {
    update_task_timer->start(ms);
}

void Translator::stopUpdateTask() {
    update_task_timer->stop();
}
