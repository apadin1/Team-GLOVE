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

Translator::translator() {
  //TODO: Determine default glove mapping
  //sensors[] = { TBD }

  update_task_timer = new RtosTimer(this, &Collector::update, osTimerPeriodic);
}

//TODO: Determine process w/ transciever
void Translator::updateGestureMap() {}

void Translator::gestureCheck() {
  working = 1;

  //Update sensor data

  for (int i = 0; i < GESTURE_COUNT; ++i) {
    //if keyboard
      //getKeyData()
      //if (changed == 1)
        //write digital value to key via HID
    //else if mouse
      //getMouseData()
      //if (changed == 1)
        //if (CLICK_TYPE != NONE)
          //write click as mouse via HID
        //else if (xspeed != 0)
          //write cursor move in x direction
        //else if (yspeed != 0)
          //write cursor move in y direction
        //else (scrollspeed)
          //write scroll for move
  }
  working = 0;
}

void Translator::startUpdateTask(uint32_t ms) {
  update_task_timer->start(ms);
}
void Translator::stopUpdateTask() {
  update_task_timer->stop();
}
